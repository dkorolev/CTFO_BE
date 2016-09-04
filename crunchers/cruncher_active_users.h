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
#include <vector>
#include <unordered_map>

#include "../../Current/Blocks/HTTP/api.h"
#include "../../Current/Blocks/MMQ/mmq.h"

#include "cruncher.h"
#include "schema.h"

namespace CTFO {

template <typename NAMESPACE>
struct ActiveUsersCruncherImpl {
  using EventLogEntry = typename NAMESPACE::EventLogEntry;
  using iOSBaseEvent = typename NAMESPACE::iOSBaseEvent;
  using event_t = typename NAMESPACE::CTFOLogEntry;
  using value_t = uint64_t;

  ActiveUsersCruncherImpl(std::chrono::microseconds interval) : interval_(interval) {}
  virtual ~ActiveUsersCruncherImpl() = default;

  void OnEvent(const event_t& e, idxts_t idxts) {
    current_us_ = idxts.us;
    if (Exists<EventLogEntry>(e)) {
      OnEventLogEntry(Value<EventLogEntry>(e));
    }
    while (!users_list_.empty() && users_list_.back().us + interval_ <= current_us_) {
      users_map_.erase(users_list_.back().device_id);
      users_list_.pop_back();
    }
  }

  value_t GetValue() const { return users_list_.size(); }

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
};

template <typename NAMESPACE>
using ActiveUsersCruncher = CTFO::StreamCruncher<MultiCruncher<ActiveUsersCruncherImpl<NAMESPACE>>>;

}  // namespace CTFO

#endif  // CRUNCHER_ACTIVE_USERS_H
