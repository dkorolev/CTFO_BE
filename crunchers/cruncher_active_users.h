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

#include "cruncher.h"
#include "schema.h"
#include <list>
#include <unordered_map>

namespace CTFO {

template <typename NAMESPACE>
struct ActiveUsersCruncherImpl {
  using entry_t = typename NAMESPACE::CTFOLogEntry;

  ActiveUsersCruncherImpl(std::chrono::seconds interval) : interval_(interval), skip_(0) {}
  virtual ~ActiveUsersCruncherImpl() = default;

  void OnIOSEvent(const typename NAMESPACE::iOSBaseEvent& e) {
    const auto cit = users_map_.find(e.device_id);
    if (cit != users_map_.end()) {
      if (cit->second != users_list_.begin()) users_list_.splice(users_list_.begin(), users_list_, cit->second);
      cit->second->us = current_time_;
    } else {
      users_list_.push_back(ActiveUser{current_time_, e.device_id});
      auto it = users_list_.end();
      users_map_[e.device_id] = --it;
    }
  }

  void operator()(const typename NAMESPACE::iOSGenericEvent& e) { OnIOSEvent(e); }

  void operator()(const typename NAMESPACE::iOSFocusEvent& e) { OnIOSEvent(e); }

  void operator()(const typename NAMESPACE::iOSIdentifyEvent& e) { OnIOSEvent(e); }

  void operator()(const typename NAMESPACE::iOSAppLaunchEvent& e) { OnIOSEvent(e); }

  void operator()(const typename NAMESPACE::iOSFirstLaunchEvent& e) { OnIOSEvent(e); }

  void operator()(const typename NAMESPACE::EventLogEntry& e) {
    current_time_ = e.server_us;
    e.event.Call(*this);
  }

  template <typename ANYTHING>
  void operator()(const ANYTHING&) {
    ++skip_;
  }

  void OnEvent(const entry_t& e, idxts_t idxts) {
    current_time_ = idxts.us;
    e.Call(*this);
    while (!users_list_.empty() && users_list_.front().us + interval_ < current_time_) {
      users_map_.erase(users_list_.front().device_id);
      users_list_.pop_front();
    }
  }

  uint64_t Count() const { return users_list_.size(); }
  uint64_t Skip() const { return skip_; }

 private:
  struct ActiveUser {
    std::chrono::microseconds us;
    std::string device_id;
  };
  using user_list_t = std::list<ActiveUser>;
  using user_map_t = std::unordered_map<std::string, typename user_list_t::iterator>;

  user_list_t users_list_;
  user_map_t users_map_;
  std::chrono::microseconds current_time_;
  std::chrono::seconds interval_;
  uint64_t skip_;
};

template <typename NAMESPACE>
using ActiveUsersCruncher = current::sherlock::StreamCruncher<ActiveUsersCruncherImpl<NAMESPACE>>;

}  // namespace CTFO

#endif  // CRUNCHER_ACTIVE_USERS_H
