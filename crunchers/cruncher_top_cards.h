/*******************************************************************************
 The MIT License (MIT)

 Copyright (c) 2016 Grigory Nikolaenko <nikolaenko.grigory@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 *******************************************************************************/

#ifndef CRUNCHER_TOP_CARDS_H
#define CRUNCHER_TOP_CARDS_H

#include <deque>
#include <vector>
#include <unordered_map>

#include "../../Current/Blocks/HTTP/api.h"
#include "../../Current/Blocks/MMQ/mmq.h"

#include "../util.h"

#include "cruncher.h"
#include "schema.h"

namespace CTFO {

CURRENT_STRUCT(CardCounters) {
  CURRENT_FIELD(ctfo, uint64_t, 0);
  CURRENT_FIELD(skip, uint64_t, 0);
  CURRENT_FIELD(tfu, uint64_t, 0);
  CURRENT_FIELD(fav, uint64_t, 0);
  CURRENT_FIELD(unfav, uint64_t, 0);
  CURRENT_FIELD(seen, uint64_t, 0);

  bool Empty() const { return ctfo == 0 && skip == 0 && tfu == 0 && fav == 0 && unfav == 0 && seen == 0; }
};

CURRENT_STRUCT(TopCardResponseItem, CardCounters) {
  CURRENT_FIELD(cid, uint64_t, 0);
  CURRENT_FIELD(rate, double, 0.0);
};

// Output of the `TopCardsCruncher` - list of top cards with their ratings.
using TopCardsCruncherResponse = std::vector<TopCardResponseItem>;

// Input of the `TopCardsCruncher` - time window, default top list size and
// a function to calculate the card rating from its counters.
struct TopCardsCruncherArgs final {
  using rate_callback_t = std::function<double(const CardCounters&)>;

  std::chrono::microseconds interval;
  uint64_t top_size;
  rate_callback_t rate_calculator;

  TopCardsCruncherArgs(std::chrono::microseconds interval, uint64_t size, rate_callback_t rate_calculator)
      : interval(interval), top_size(size), rate_calculator(rate_calculator) {}
};

template <typename NAMESPACE>
struct TopCardsCruncherImpl {
  using Transaction_Z = typename NAMESPACE::Transaction_T9220981828355492272;
  using EventLogEntry = typename NAMESPACE::EventLogEntry;
  using CardDeleted = typename NAMESPACE::Persisted_CardDeleted;
  using iOSGenericEvent = typename NAMESPACE::iOSGenericEvent;
  using CID = typename NAMESPACE::CID;
  using event_t = typename NAMESPACE::CTFOLogEntry;
  using card_t = TopCardResponseItem;
  using value_t = TopCardsCruncherResponse;

  TopCardsCruncherImpl(const TopCardsCruncherArgs& args)
      : current_us_(std::chrono::microseconds(0)), args_(args) {}
  virtual ~TopCardsCruncherImpl() = default;

  // Handle an incoming event
  void OnEvent(const event_t& e, idxts_t idxts) {
    // Treat timestamp of the last event as current time.
    current_us_ = idxts.us;
    if (Exists<Transaction_Z>(e)) {
      const auto& transaction = Value<Transaction_Z>(e);
      for (const auto& mutation : transaction.mutations) {
        if (Exists<CardDeleted>(mutation)) {
          OnCardDeleted(Value<CardDeleted>(mutation));
        }
      }
    } else if (Exists<EventLogEntry>(e)) {
      OnEventLogEntry(Value<EventLogEntry>(e));
    }
    // Remove events that go beyond specified time window.
    while (!events_list_.empty() && events_list_.back().us + args_.interval <= current_us_) {
      ApplyCardEvent(events_list_.back(), Direction::Leave);
      events_list_.pop_back();
    }
  }

  // Collect and return a list of N cards with top ratings.
  value_t GetValue(size_t n = 0) const {
    if (!n) {
      n = args_.top_size;
    }
    value_t result;
    result.reserve(n);
    for (const auto& bucket : top_cards_map_) {
      for (const auto cid : bucket.second) {
        if (!deleted_cards_.count(cid)) {
          result.push_back(cards_map_.at(cid));
          if (result.size() >= n) {
            return result;
          }
        }
      }
    }
    return result;
  }

 private:
  enum class CTFO_EVENT : int { SEEN, SKIP, CTFO, TFU, FAV_CARD, UNFAV_CARD };

  struct CardEvent final {
    CID cid;
    CTFO_EVENT type;
    std::chrono::microseconds us;
  };
  using event_list_t = std::deque<CardEvent>;
  using cards_map_t = std::unordered_map<CID, card_t, current::CurrentHashFunction<CID>>;
  using cards_set_t = std::unordered_set<CID, current::CurrentHashFunction<CID>>;
  using top_cards_map_t = std::map<int64_t, cards_set_t, std::greater<uint64_t>>;

  // Apply an event by recalculating the corresponding card rating,
  // when the one moves in or out of the events queue (sliding window).
  enum class Direction : int { Enter = +1, Leave = -1 };
  void ApplyCardEvent(const CardEvent& e, Direction direction) {
    card_t& card = cards_map_[e.cid];
    if (card.Empty()) {
      card.cid = static_cast<uint64_t>(e.cid);
    } else {
      auto it = top_cards_map_.find(card.rate);
      it->second.erase(e.cid);
      if (it->second.empty()) {
        top_cards_map_.erase(it);
      }
    }

    static const std::map<CTFO_EVENT, uint64_t card_t::*> event_to_member_ptr = {
        {CTFO_EVENT::SEEN, &card_t::seen},
        {CTFO_EVENT::SKIP, &card_t::skip},
        {CTFO_EVENT::CTFO, &card_t::ctfo},
        {CTFO_EVENT::TFU, &card_t::tfu},
        {CTFO_EVENT::FAV_CARD, &card_t::fav},
        {CTFO_EVENT::UNFAV_CARD, &card_t::unfav}};
    card.*(event_to_member_ptr.at(e.type)) += static_cast<int>(direction);
    card.rate = args_.rate_calculator(card);

    if (!card.Empty()) {
      top_cards_map_[card.rate].insert(e.cid);
    } else {
      cards_map_.erase(e.cid);
    }
  }

  void OnCardDeleted(const CardDeleted& e) { deleted_cards_.insert(e.key); }

  void OnEventLogEntry(const EventLogEntry& e) {
    current_us_ = e.server_us;
    if (Exists<iOSGenericEvent>(e.event)) {
      OnIOSGenericEvent(Value<iOSGenericEvent>(e.event));
    }
  }

  void OnIOSGenericEvent(const iOSGenericEvent& e) {
    static const std::map<std::string, CTFO_EVENT> supported_events = {{"SEEN", CTFO_EVENT::SEEN},
                                                                       {"CTFO", CTFO_EVENT::CTFO},
                                                                       {"TFU", CTFO_EVENT::TFU},
                                                                       {"SKIP", CTFO_EVENT::SKIP},
                                                                       {"FAV", CTFO_EVENT::FAV_CARD},
                                                                       {"FAV_CARD", CTFO_EVENT::FAV_CARD},
                                                                       {"UNFAV", CTFO_EVENT::UNFAV_CARD},
                                                                       {"UNFAV_CARD", CTFO_EVENT::UNFAV_CARD}};
    const auto cit = supported_events.find(e.event);
    if (cit != supported_events.end()) {
      const std::string cid_str = e.fields.count("cid") ? e.fields.at("cid") : "";
      events_list_.push_front(CardEvent{static_cast<CID>(StringToCID(cid_str)), cit->second, current_us_});
      ApplyCardEvent(events_list_.front(), Direction::Enter);
    }
  }

  event_list_t events_list_;
  cards_map_t cards_map_;
  cards_set_t deleted_cards_;
  top_cards_map_t top_cards_map_;
  std::chrono::microseconds current_us_;
  const TopCardsCruncherArgs args_;
};

template <typename NAMESPACE, size_t BUFFER_SIZE = constants::kDefaultMMQBufferSize>
using TopCardsCruncher = CTFO::StreamCruncher<MultiCruncher<TopCardsCruncherImpl<NAMESPACE>>, BUFFER_SIZE>;

}  // namespace CTFO

#endif  // CRUNCHER_TOP_CARDS_H
