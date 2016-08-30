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

#ifndef CRUNCHER_ACTIVE_USERS_H
#define CRUNCHER_ACTIVE_USERS_H

#include <list>
#include <unordered_map>

#include "../../Current/Blocks/MMQ/mmq.h"

#include "cruncher.h"
#include "schema.h"

namespace CTFO {

template <typename NAMESPACE>
struct ActiveUsersCruncherImpl {
  using entry_t = typename NAMESPACE::CTFOLogEntry;
  using EventLogEntry = typename NAMESPACE::EventLogEntry;
  using iOSBaseEvent = typename NAMESPACE::iOSBaseEvent;

  ActiveUsersCruncherImpl(std::chrono::microseconds interval) : interval_(interval), events_seen_(0) {}
  virtual ~ActiveUsersCruncherImpl() = default;

  void OnEvent(const entry_t& e, idxts_t idxts) {
    current_us_ = idxts.us;
    ++events_seen_;
    if (Exists<EventLogEntry>(e)) {
      OnEventLogEntry(Value<EventLogEntry>(e));
    }
    while (!users_list_.empty() && users_list_.back().us + interval_ <= current_us_) {
      users_map_.erase(users_list_.back().device_id);
      users_list_.pop_back();
    }
  }

  uint64_t Count() const { return users_list_.size(); }
  uint64_t EventsSeen() const { return events_seen_; }


 private:
  struct ActiveUser final {
    std::chrono::microseconds us;
    std::string device_id;
  };
  using user_list_t = std::list<ActiveUser>;
  using user_map_t = std::unordered_map<std::string, typename user_list_t::iterator>;

  void OnEventLogEntry(const EventLogEntry& e) {
    current_us_ = e.server_us;
    if (Exists<iOSBaseEvent>(e.event)) {
      OnIOSEvent(Value<iOSBaseEvent>(e.event));
    }
  }

  void OnIOSEvent(const iOSBaseEvent& e) {
    const auto cit = users_map_.find(e.device_id);
    if (cit != users_map_.end()) {
      if (cit->second != users_list_.begin()) {
        users_list_.splice(users_list_.begin(), users_list_, cit->second);
      }
      cit->second->us = current_us_;
    } else {
      users_list_.push_front(ActiveUser{current_us_, e.device_id});
      users_map_[e.device_id] = users_list_.begin();
    }
  }

  user_list_t users_list_;
  user_map_t users_map_;
  std::chrono::microseconds current_us_;
  std::chrono::microseconds interval_;
  uint64_t events_seen_;
};

template <typename NAMESPACE>
using ActiveUsersCruncher = CTFO::StreamCruncher<ActiveUsersCruncherImpl<NAMESPACE>>;

template <typename NAMESPACE>
struct ActiveUsersMultiCruncherImpl {
  using entry_t = typename NAMESPACE::CTFOLogEntry;
  using cruncher_t = ActiveUsersCruncherImpl<NAMESPACE>;
  using crunchers_list_t = std::vector<std::unique_ptr<cruncher_t>>;
  using duration_list_t = std::vector<std::chrono::microseconds>;
  using mmq_t = current::mmq::MMQ<entry_t, IntermediateSubscriber<entry_t>, 1024*1024>;

  ActiveUsersMultiCruncherImpl(const duration_list_t& intervals)
	: last_event_us_(std::chrono::microseconds(0))
    , events_seen_(0)
    , mmq_subscriber_([this](const entry_t& entry, idxts_t idxts){ OnEventInternal(entry, idxts); })
    , mmq_(mmq_subscriber_, 1024*1024)
    , intervals_(intervals) {
    crunchers_.reserve(intervals.size());
    for (const auto interval : intervals) {
      crunchers_.push_back(std::make_unique<cruncher_t>(interval));
    }
  }
  virtual ~ActiveUsersMultiCruncherImpl() = default;
  
  void OnEvent(const entry_t& e, idxts_t idxts) {
    mmq_.Publish(e, idxts.us);
  }
  
  uint64_t Count(uint64_t ind) {
    return crunchers_[ind]->Count();
  }

  uint64_t EventsSeen() {
    return events_seen_;
  }

private:
  void OnEventInternal(const entry_t& e, idxts_t idxts) {
    last_event_us_ = idxts.us;
    for (auto& cruncher : crunchers_) {
      cruncher->OnEvent(e, idxts);
    }
    ++events_seen_;
  }

  std::chrono::microseconds last_event_us_;
  uint64_t events_seen_;
  IntermediateSubscriber<entry_t> mmq_subscriber_;
  mmq_t mmq_;
  duration_list_t intervals_;
  crunchers_list_t crunchers_;
};
  
template <typename NAMESPACE>
using ActiveUsersMultiCruncher = CTFO::StreamCruncher<ActiveUsersMultiCruncherImpl<NAMESPACE>>;


}  // namespace CTFO

#endif  // CRUNCHER_ACTIVE_USERS_H
