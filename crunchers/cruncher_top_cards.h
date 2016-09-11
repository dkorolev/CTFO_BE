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
#include "../../Current/Bricks/util/singleton.h"

#include "cruncher.h"
#include "schema.h"

namespace CTFO {

struct TopCardsCruncherArgs final {
  using rate_callback_t = std::function<uint64_t(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t)>;

  std::chrono::microseconds interval;
  uint64_t top_size;
  rate_callback_t rate_calculator;

  TopCardsCruncherArgs(std::chrono::microseconds interval, uint64_t size, rate_callback_t rate_calculator)
      : interval(interval), top_size(size), rate_calculator(rate_calculator) {}
};

CURRENT_STRUCT(TopCardsCruncherResponseItem) {
  CURRENT_FIELD(cid, uint64_t, 0);
  CURRENT_FIELD(rate, uint64_t, 0);
  CURRENT_FIELD(ctfo_count, uint64_t, 0);
  CURRENT_FIELD(skip_count, uint64_t, 0);
  CURRENT_FIELD(tfu_count, uint64_t, 0);
  CURRENT_FIELD(fav_count, uint64_t, 0);
  CURRENT_FIELD(seen_count, uint64_t, 0);

  bool Empty() {
    return ctfo_count == 0 && skip_count == 0 && tfu_count == 0 && fav_count == 0 && seen_count == 0;
  }
};

using TopCardsCruncherResponse = std::vector<TopCardsCruncherResponseItem>;

template <typename NAMESPACE>
struct TopCardsCruncherImpl {
  using Transaction_Z = typename NAMESPACE::Transaction_T9220981828355492272;
  using EventLogEntry = typename NAMESPACE::EventLogEntry;
  using CardDeleted = typename NAMESPACE::Persisted_CardDeleted;
  using iOSGenericEvent = typename NAMESPACE::iOSGenericEvent;
  using CID = typename NAMESPACE::CID;
  using event_t = typename NAMESPACE::CTFOLogEntry;
  using card_t = TopCardsCruncherResponseItem;
  using value_t = std::vector<card_t>;

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
      TimeWindowLeft(events_list_.back());
      events_list_.pop_back();
    }
  }

  value_t GetValue(size_t n = 0) const {
    if (!n) n = args_.top_size;
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
      std::map<uint64_t, std::unordered_set<CID, current::CurrentHashFunction<CID>>, std::greater<uint64_t>>;

  void TimeWindowsEnter(CardEvent&& e) {
    const auto cit = cards_map_.find(e.cid);
    if (cit != cards_map_.end()) {
      card_t& card = cit->second;
      top_cards_map_[card.rate].erase(e.cid);
      ApplyCardEvent(card, e.type, false /*rollback*/);
      top_cards_map_[card.rate].insert(e.cid);
    } else {
      card_t& card = cards_map_[e.cid];
      card.cid = static_cast<uint64_t>(e.cid);
      ApplyCardEvent(card, e.type, false /*rollback*/);
      top_cards_map_[card.rate].insert(e.cid);
    }
    events_list_.push_front(std::move(e));
  }

  void TimeWindowLeft(const CardEvent& e) {
    const auto cit = cards_map_.find(e.cid);
    if (cit != cards_map_.end()) {
      auto& card = cit->second;
      top_cards_map_[card.rate].erase(e.cid);
      ApplyCardEvent(card, e.type, true /*rollback*/);
      if (!card.Empty()) {
        top_cards_map_[card.rate].insert(e.cid);
      } else {
        cards_map_.erase(e.cid);
      }
    }
  }

  void ApplyCardEvent(card_t& card, CTFO_EVENT event, bool rollback) {
    int sign = rollback ? -1 : 1;
    switch (event) {
      case CTFO_EVENT::SEEN:
        card.seen_count += sign;
        break;
      case CTFO_EVENT::SKIP:
        card.skip_count += sign;
        break;
      case CTFO_EVENT::CTFO:
        card.ctfo_count += sign;
        break;
      case CTFO_EVENT::TFU:
        card.tfu_count += sign;
        break;
      case CTFO_EVENT::FAV_CARD:
        card.fav_count += sign;
        break;
      case CTFO_EVENT::UNFAV_CARD:
        card.fav_count -= sign;
        break;
      default:
        return;
    }
    card.rate = args_.rate_calculator(
        card.ctfo_count, card.skip_count, card.tfu_count, card.fav_count, card.seen_count);
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
    auto& supported_events = current::ThreadLocalSingleton<std::map<std::string, CTFO_EVENT>>();
    if (supported_events.empty()) {
      supported_events.emplace("SEEN", CTFO_EVENT::SEEN);
      supported_events.emplace("CTFO", CTFO_EVENT::CTFO);
      supported_events.emplace("TFU", CTFO_EVENT::TFU);
      supported_events.emplace("SKIP", CTFO_EVENT::SKIP);
      supported_events.emplace("FAV", CTFO_EVENT::FAV_CARD);
      supported_events.emplace("FAV_CARD", CTFO_EVENT::FAV_CARD);
      supported_events.emplace("UNFAV", CTFO_EVENT::UNFAV_CARD);
      supported_events.emplace("UNFAV_CARD", CTFO_EVENT::UNFAV_CARD);
    }
    try {
      const CTFO_EVENT event = supported_events.at(e.event);
      const std::string cid_str = e.fields.count("cid") ? e.fields.at("cid") : "";
      TimeWindowsEnter(CardEvent{StringToCID(cid_str), event, current_us_});
    } catch (const std::out_of_range&) {
      // ignore unsupported events
    }
  }

  CID StringToCID(const std::string& s) {
    if (s.length() == 21 && s[0] == 'c') {  // 'c' + 20 digits of `uint64_t` decimal representation.
      return static_cast<CID>(current::FromString<uint64_t>(s.substr(1)));
    }
    return static_cast<CID>(0u);
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
