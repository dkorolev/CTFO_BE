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
  CURRENT_FIELD(rate, int64_t, 0);
};

using TopCardsCruncherResponse = std::vector<TopCardResponseItem>;

struct TopCardsCruncherArgs final {
  using rate_callback_t = std::function<int64_t(const CardCounters&)>;

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

  TopCardsCruncherImpl(const TopCardsCruncherArgs& args) : args_(args) {}
  virtual ~TopCardsCruncherImpl() = default;

  void OnEvent(const event_t& e, idxts_t idxts) {
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
    while (!events_list_.empty() && events_list_.back().us + args_.interval <= current_us_) {
      TimeWindowLeft();
    }
  }

  value_t GetValue(size_t n = 0) const {
    if (!n) {
      n = args_.top_size;
    }
    value_t result;
    result.reserve(n);
    for (const auto& bucket : top_cards_map_) {
      for (const auto cid : bucket.second) {
        const auto cit = cards_map_.find(cid);
        result.push_back(cit->second);
        if (result.size() >= n) {
          return result;
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
  using top_cards_map_t =
      std::map<int64_t, std::unordered_set<CID, current::CurrentHashFunction<CID>>, std::greater<uint64_t>>;

  void TimeWindowEntered(CardEvent&& e) {
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
    ApplyCardEvent(card, e.type, Delta::Enter);
    top_cards_map_[card.rate].insert(e.cid);
    events_list_.push_front(std::move(e));
  }

  void TimeWindowLeft() {
    const auto& e = events_list_.back();
    const auto cit = cards_map_.find(e.cid);
    if (cit != cards_map_.end()) {
      auto& card = cit->second;
      top_cards_map_[card.rate].erase(e.cid);
      ApplyCardEvent(card, e.type, Delta::Leave);
      if (!card.Empty()) {
        top_cards_map_[card.rate].insert(e.cid);
      } else {
        cards_map_.erase(e.cid);
      }
    }
    events_list_.pop_back();
  }

  enum class Delta : int { Enter = +1, Leave = -1 };
  void ApplyCardEvent(card_t& card, CTFO_EVENT event, Delta delta) {
    const std::map<CTFO_EVENT, uint64_t*> event_to_member_ptr = {
      {CTFO_EVENT::SEEN, &card.seen},
      {CTFO_EVENT::SKIP, &card.skip},
      {CTFO_EVENT::CTFO, &card.ctfo},
      {CTFO_EVENT::TFU, &card.tfu},
      {CTFO_EVENT::FAV_CARD, &card.fav},
      {CTFO_EVENT::UNFAV_CARD, &card.unfav}
    };
    *event_to_member_ptr.at(event) += static_cast<int>(delta);
    card.rate = args_.rate_calculator(card);
  }

  void OnCardDeleted(const CardDeleted& e) {
    const auto cit = cards_map_.find(e.key);
    if (cit != cards_map_.end()) {
      auto& top_bucket = top_cards_map_[cit->second.rate];
      top_bucket.erase(e.key);
      cards_map_.erase(cit);
      if (top_bucket.empty()) {
        top_cards_map_.erase(cit->second.rate);
      }
    }
  }

  void OnEventLogEntry(const EventLogEntry& e) {
    current_us_ = e.server_us;
    if (Exists<iOSGenericEvent>(e.event)) {
      OnIOSGenericEvent(Value<iOSGenericEvent>(e.event));
    }
  }

  void OnIOSGenericEvent(const iOSGenericEvent& e) {
    static std::map<std::string, CTFO_EVENT> supported_events = {{"SEEN", CTFO_EVENT::SEEN},
                                                                 {"CTFO", CTFO_EVENT::CTFO},
                                                                 {"TFU", CTFO_EVENT::TFU},
                                                                 {"SKIP", CTFO_EVENT::SKIP},
                                                                 {"FAV", CTFO_EVENT::FAV_CARD},
                                                                 {"FAV_CARD", CTFO_EVENT::FAV_CARD},
                                                                 {"UNFAV", CTFO_EVENT::UNFAV_CARD},
                                                                 {"UNFAV_CARD", CTFO_EVENT::UNFAV_CARD}};
    try {
      const CTFO_EVENT event = supported_events.at(e.event);
      const std::string cid_str = e.fields.count("cid") ? e.fields.at("cid") : "";
      TimeWindowEntered(CardEvent{static_cast<CID>(StringToCID(cid_str)), event, current_us_});
    } catch (const std::out_of_range&) {
      // ignore unsupported events
    }
  }

  event_list_t events_list_;
  cards_map_t cards_map_;
  top_cards_map_t top_cards_map_;
  std::chrono::microseconds current_us_;
  const TopCardsCruncherArgs args_;
};

template <typename NAMESPACE>
using TopCardsCruncher = CTFO::StreamCruncher<MultiCruncher<TopCardsCruncherImpl<NAMESPACE>>>;

}  // namespace CTFO

#endif  // CRUNCHER_TOP_CARDS_H
