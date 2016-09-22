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

#ifndef CRUNCHER_H
#define CRUNCHER_H

#include "../../Current/Blocks/HTTP/api.h"
#include "../../Current/Blocks/MMQ/mmq.h"
#include "../../Current/Blocks/SS/ss.h"

namespace CTFO {

// A simple wrapper for the StreamSubscriber interface, that reduces it to a single callback
template <typename ENTRY>
struct IntermediateSubscriberImpl {
  using EntryResponse = current::ss::EntryResponse;
  using TerminationResponse = current::ss::TerminationResponse;
  using entry_t = ENTRY;
  using subscriber_callback_t = std::function<void(entry_t&&, idxts_t)>;

  IntermediateSubscriberImpl(subscriber_callback_t callback) : actual_subscriber_(callback) {}
  virtual ~IntermediateSubscriberImpl() {}

  EntryResponse operator()(entry_t&& entry, idxts_t current, idxts_t) {
    actual_subscriber_(std::move(entry), current);
    return EntryResponse::More;
  }

  EntryResponse EntryResponseIfNoMorePassTypeFilter() const { return EntryResponse::More; }
  TerminationResponse Terminate() const { return TerminationResponse::Terminate; }

 private:
  subscriber_callback_t actual_subscriber_;
};

template <typename ENTRY>
using IntermediateSubscriber = current::ss::StreamSubscriber<IntermediateSubscriberImpl<ENTRY>, ENTRY>;

// A wrapper for data crunchers of any kind,
// which also receives HTTP requests and forward them to the implementation (IMPL).
// The IMPL should declare the type of incoming events as `event_t` and implement
// `OnEvent()` and `OnRequest()` methods.
// The IMPL class methods are invoked from a single thread, which is achieved
// by using the MMQ for all incoming events and requests.
template <typename IMPL, size_t BUFFER_SIZE>
struct GenericCruncherImpl : public IMPL {
  using EntryResponse = current::ss::EntryResponse;
  using TerminationResponse = current::ss::TerminationResponse;
  using event_t = typename IMPL::event_t;

  class Message {
   public:
    virtual ~Message() = default;
    virtual void Handle(IMPL& cruncher) = 0;
  };

  class EventMessage final : public Message {
   public:
    EventMessage(event_t&& e, idxts_t idxts) : event_(std::move(e)), idxts_(idxts) {}
    EventMessage(const event_t& e, idxts_t idxts) : event_(e), idxts_(idxts) {}
    void Handle(IMPL& cruncher) override { cruncher.OnEvent(std::move(event_), idxts_); }

   private:
    event_t event_;
    const idxts_t idxts_;
  };

  class RequestMessage final : public Message {
   public:
    RequestMessage(Request&& r) : request_(std::move(r)) {}
    void Handle(IMPL& cruncher) override { cruncher.OnRequest(std::move(request_)); }

   private:
    Request request_;
  };

  using mmq_message_t = std::unique_ptr<Message>;
  using mmq_t = current::mmq::MMQ<mmq_message_t, IntermediateSubscriber<mmq_message_t>, BUFFER_SIZE>;

  template <typename... ARGS>
  GenericCruncherImpl(uint16_t port, const std::string& route, ARGS&&... args)
      : IMPL(std::forward<ARGS>(args)...),
        port_(port),
        mmq_worker_([this](mmq_message_t&& message, idxts_t) { message->Handle(*this); }),
        mmq_(mmq_worker_, BUFFER_SIZE) {
    scoped_http_routes_ += HTTP(port).Register(route + "/healthz", [](Request r) { r("OK\n"); }) +
                           HTTP(port).Register(route + "/data",
                                               [this](Request r) {
                                                 mmq_.Publish(std::make_unique<RequestMessage>(std::move(r)));
                                               });
  }
  virtual ~GenericCruncherImpl() = default;

  void Join() { HTTP(port_).Join(); }

  EntryResponse operator()(const event_t& event, idxts_t current, idxts_t) {
    mmq_.Publish(std::make_unique<EventMessage>(event, current));
    return EntryResponse::More;
  }

  EntryResponse operator()(event_t&& event, idxts_t current, idxts_t) {
    mmq_.Publish(std::make_unique<EventMessage>(std::move(event), current));
    return EntryResponse::More;
  }

  EntryResponse EntryResponseIfNoMorePassTypeFilter() const { return EntryResponse::More; }
  TerminationResponse Terminate() const { return TerminationResponse::Terminate; }

 protected:
  const uint16_t port_;
  HTTPRoutesScope scoped_http_routes_;
  IntermediateSubscriber<mmq_message_t> mmq_worker_;
  mmq_t mmq_;
};

template <typename IMPL, size_t BUFFER_SIZE = 1024 * 1024>
using StreamCruncher =
    current::ss::StreamSubscriber<GenericCruncherImpl<IMPL, BUFFER_SIZE>, typename IMPL::event_t>;

template <typename IMPL, size_t BUFFER_SIZE>
struct GenericTickCruncherImpl : public GenericCruncherImpl<IMPL, BUFFER_SIZE> {
  using base_t = GenericCruncherImpl<IMPL, BUFFER_SIZE>;
  using event_t = typename base_t::event_t;
  using EntryResponse = current::ss::EntryResponse;

  class TickMessage final : public base_t::Message {
   public:
    TickMessage(const std::chrono::microseconds& timestamp) : timestamp_(timestamp) {}
    void Handle(IMPL& cruncher) override { cruncher.OnTick(timestamp_); }

   private:
    const std::chrono::microseconds timestamp_;
  };

  template <typename... ARGS>
  GenericTickCruncherImpl(uint16_t port,
                          const std::string& route,
                          const std::chrono::microseconds tick_interval,
                          ARGS&&... args)
      : base_t(port, route, std::forward<ARGS>(args)...),
        tick_interval_(tick_interval),
        last_event_us_(std::chrono::microseconds(0)) {}
  virtual ~GenericTickCruncherImpl() = default;

  EntryResponse operator()(const event_t& event, idxts_t current, idxts_t last) {
    GenerateTickEvents(current.us);
    return base_t::operator()(event, current, last);
  }

  EntryResponse operator()(event_t&& event, idxts_t current, idxts_t last) {
    GenerateTickEvents(current.us);
    return base_t::operator()(std::move(event), current, last);
  }

 private:
  void GenerateTickEvents(std::chrono::microseconds us) {
    for (uint64_t i = last_event_us_.count() / tick_interval_.count(),
                  end = us.count() / tick_interval_.count();
         i < end;
         ++i) {
      mmq_.Publish(std::make_unique<TickMessage>(std::chrono::microseconds((i + 1) * tick_interval_.count())));
    }
    last_event_us_ = us;
  }

  using base_t::mmq_;
  const std::chrono::microseconds tick_interval_;
  std::chrono::microseconds last_event_us_;
};

template <typename IMPL, size_t BUFFER_SIZE = 1024 * 1024>
using StreamTickCruncher =
    current::ss::StreamSubscriber<GenericTickCruncherImpl<IMPL, BUFFER_SIZE>, typename IMPL::event_t>;

CURRENT_STRUCT_T(CruncherResponse) {
  CURRENT_FIELD(comment, std::string);
  CURRENT_FIELD(timestamp, std::chrono::microseconds, std::chrono::microseconds(0));
  CURRENT_FIELD(value, T);
};

// An aggregator for several crunchers of the same type.
// The CRUNCHER should declare the type of incoming events as `event_t`,
// the result type of its calculations as `value_t` and implement
// `OnEvent()` and `GetValue()` methods.
template <typename CRUNCHER>
class MultiCruncher {
 public:
  using cruncher_t = CRUNCHER;
  using event_t = typename cruncher_t::event_t;
  using value_t = typename cruncher_t::value_t;
  using value_list_t = std::vector<value_t>;
  using response_t = CruncherResponse<value_list_t>;
  using response_short_t = CruncherResponse<value_t>;

  template <typename ARG>
  MultiCruncher(const std::vector<ARG>& args)
      : last_event_us_(std::chrono::microseconds(0)) {
    crunchers_.reserve(args.size());
    for (const auto& arg : args) {
      crunchers_.push_back(std::make_unique<cruncher_t>(arg));
    }
  }
  virtual ~MultiCruncher() = default;

  void OnEvent(event_t&& event, idxts_t idxts) {
    last_event_us_ = idxts.us;
    for (auto& cruncher : crunchers_) {
      cruncher->OnEvent(std::move(event), idxts);
    }
  }

  void OnTick(std::chrono::microseconds us) {
    last_event_us_ = us;
    for (auto& cruncher : crunchers_) {
      cruncher->OnTick(us);
    }
  }

  void OnRequest(Request&& r) {
    const std::string comment =
        Printf("Last processed event was %llu minutes ago",
               std::chrono::duration_cast<std::chrono::minutes>(current::time::Now() - last_event_us_).count());
    if (r.url.query.has("i")) {
      uint64_t ind = current::FromString<uint64_t>(r.url.query.get("i", "0"));
      if (ind < crunchers_.size()) {
        response_short_t response;
        response.comment = comment;
        response.timestamp = last_event_us_;
        response.value = crunchers_[ind]->GetValue();
        r(response);
      } else {
        r("OUT OF RANGE\n", HTTPResponseCode.BadRequest);
      }
    } else {
      response_t response;
      response.comment = comment;
      response.timestamp = last_event_us_;
      response.value.reserve(crunchers_.size());
      for (const auto& cruncher : crunchers_) {
        response.value.push_back(cruncher->GetValue());
      }
      r(response);
    }
  }

 protected:
  std::chrono::microseconds last_event_us_;
  std::vector<std::unique_ptr<cruncher_t>> crunchers_;
};

CURRENT_STRUCT(CruncherOutputStreamParams) {
  CURRENT_FIELD(interval, std::chrono::microseconds);
  CURRENT_FIELD(path, std::string);
  CURRENT_DEFAULT_CONSTRUCTOR(CruncherOutputStreamParams) {}
  CURRENT_CONSTRUCTOR(CruncherOutputStreamParams)(std::chrono::microseconds interval, const std::string& path)
      : interval(interval), path(path) {}
};

template <typename CRUNCHER, typename OUTPUT_STREAM>
class StreamedMultiCruncher : public MultiCruncher<CRUNCHER> {
 public:
  using base_t = MultiCruncher<CRUNCHER>;
  using event_t = typename base_t::event_t;
  using response_t = typename base_t::response_short_t;
  using params_list_t = std::vector<CruncherOutputStreamParams>;
  using streams_list_t = std::vector<OUTPUT_STREAM>;

  template <typename ARG>
  StreamedMultiCruncher(const std::vector<ARG>& args, const params_list_t& params)
      : base_t(args), params_(params) {
    assert(args.size() == params.size());
    streams_.reserve(params.size());
    for (const auto& param : params) {
      streams_.emplace_back(param.path);
    }
  }
  virtual ~StreamedMultiCruncher() = default;

  void OnTick(std::chrono::microseconds us) {
    std::chrono::microseconds last_us = last_event_us_;
    base_t::OnTick(us);
    for (size_t i = 0, sz = streams_.size(); i < sz; ++i) {
      uint64_t publish_interval = params_[i].interval.count();
      if (us.count() / publish_interval != last_us.count() / publish_interval) {
        response_t response;
        response.value = crunchers_[i]->GetValue();
        for (uint64_t j = last_us.count() / publish_interval, end = us.count() / publish_interval; j < end;
             ++j) {
          response.timestamp = std::chrono::microseconds((j + 1) * publish_interval);
          streams_[i].Publish(response);
        }
      }
    }
  }

  void OnEvent(event_t&& event, idxts_t idxts) {
    OnTick(idxts.us);
    base_t::OnEvent(std::move(event), idxts);
  }

  void OnRequest(Request&& r) { base_t::OnRequest(std::move(r)); }

 private:
  using base_t::crunchers_;
  using base_t::last_event_us_;
  const params_list_t params_;
  streams_list_t streams_;
};

}  // namespace CTFO

#endif  // CRUNCHER_H
