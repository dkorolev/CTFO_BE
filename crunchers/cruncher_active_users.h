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
  using entry_t = typename NAMESPACE::CTFOLogEntry;
  using EventLogEntry = typename NAMESPACE::EventLogEntry;
  using iOSBaseEvent = typename NAMESPACE::iOSBaseEvent;

  ActiveUsersCruncherImpl(std::chrono::microseconds interval) : interval_(interval) {}
  virtual ~ActiveUsersCruncherImpl() = default;

  void OnEvent(const entry_t& e, idxts_t idxts) {
    current_us_ = idxts.us;
    if (Exists<EventLogEntry>(e)) {
      OnEventLogEntry(Value<EventLogEntry>(e));
    }
    while (!users_list_.empty() && users_list_.back().us + interval_ <= current_us_) {
      users_map_.erase(users_list_.back().device_id);
      users_list_.pop_back();
    }
  }

  uint64_t Count() const { return users_list_.size(); }

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
using ActiveUsersCruncher = CTFO::StreamCruncher<ActiveUsersCruncherImpl<NAMESPACE>>;

CURRENT_STRUCT(ResponseGetActiveUsersShort) {
  CURRENT_FIELD(count, uint64_t, 0);
  CURRENT_FIELD(timestamp, std::chrono::microseconds, std::chrono::microseconds(0));
};

CURRENT_STRUCT(ResponseGetActiveUsers) {
  CURRENT_FIELD(comment, std::string);
  CURRENT_FIELD(timestamp, std::chrono::microseconds, std::chrono::microseconds(0));
  CURRENT_FIELD(active_users, std::vector<uint64_t>);
};

template <typename NAMESPACE>
struct ActiveUsersMultiCruncherImpl {
  using entry_t = typename NAMESPACE::CTFOLogEntry;
  using request_ptr_t = std::unique_ptr<Request>;

  struct Message final {
    Message() = default;
    Message(const entry_t& e, idxts_t idxts) : idxts(idxts), event(new entry_t(e)), type(Event) {}
    Message(entry_t&& e, idxts_t idxts) : idxts(idxts), event(new entry_t(std::move(e))), type(Event) {}
    explicit Message(Request&& r) : request(new Request(std::move(r))), type(GetData) {}
    idxts_t idxts;
    std::unique_ptr<entry_t> event;
    std::unique_ptr<Request> request;
    enum { Event, GetData } type;
  };

  using cruncher_t = ActiveUsersCruncherImpl<NAMESPACE>;
  using crunchers_list_t = std::vector<std::unique_ptr<cruncher_t>>;
  using duration_list_t = std::vector<std::chrono::microseconds>;
  using mmq_t = current::mmq::MMQ<Message, IntermediateSubscriber<Message>, 1024 * 1024>;

  ActiveUsersMultiCruncherImpl(const duration_list_t& intervals, uint16_t port, const std::string& route)
      : last_event_us_(std::chrono::microseconds(0)),
        mmq_subscriber_([this](Message&& message, idxts_t) {
          switch (message.type) {
            case Message::Event:
              OnEventInternal(std::move(*message.event), message.idxts);
              break;
            case Message::GetData:
              HandleGetDataInternal(std::move(*message.request));
              break;
          }
        }),
        mmq_(mmq_subscriber_, 1024 * 1024) {
    crunchers_.reserve(intervals.size());
    for (const auto interval : intervals) {
      crunchers_.push_back(std::make_unique<cruncher_t>(interval));
    }
    scoped_http_routes_ +=
        HTTP(port).Register(route + "/healthz", [](Request r) { r("OK\n"); }) +
        HTTP(port).Register(route + "/data", [this](Request r) { mmq_.Publish(Message(std::move(r))); });
  }
  virtual ~ActiveUsersMultiCruncherImpl() = default;

  void OnEvent(const entry_t& e, idxts_t idxts) { mmq_.Publish(Message(e, idxts), idxts.us); }
  void OnEvent(entry_t&& e, idxts_t idxts) { mmq_.Publish(Message(std::move(e), idxts)); }

 private:
  void OnEventInternal(entry_t&& e, idxts_t idxts) {
    for (auto& cruncher : crunchers_) {
      cruncher->OnEvent(e, idxts);
    }
    last_event_us_ = idxts.us;
  }

  void HandleGetDataInternal(Request&& r) {
    if (r.url.query.has("i")) {
      uint64_t ind = current::FromString<uint64_t>(r.url.query.get("i", "0"));
      if (ind < crunchers_.size()) {
        ResponseGetActiveUsersShort response;
        response.count = crunchers_[ind]->Count();
        response.timestamp = last_event_us_;
        r(response);
      } else {
        r("OUT OF RANGE\n", HTTPResponseCode.BadRequest);
      }
    } else {
      ResponseGetActiveUsers response;
      response.comment = Printf(
          "Last processed event was %llu minutes ago",
          std::chrono::duration_cast<std::chrono::minutes>(current::time::Now() - last_event_us_).count());
      response.timestamp = last_event_us_;
      response.active_users.reserve(crunchers_.size());
      for (const auto& cruncher : crunchers_) {
        response.active_users.push_back(cruncher->Count());
      }
      r(response);
    }
  }

  std::chrono::microseconds last_event_us_;
  IntermediateSubscriber<Message> mmq_subscriber_;
  mmq_t mmq_;
  crunchers_list_t crunchers_;
  HTTPRoutesScope scoped_http_routes_;
};

template <typename NAMESPACE>
using ActiveUsersMultiCruncher = CTFO::StreamCruncher<ActiveUsersMultiCruncherImpl<NAMESPACE>>;

}  // namespace CTFO

#endif  // CRUNCHER_ACTIVE_USERS_H
