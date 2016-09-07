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

#include "../../Current/Blocks/SS/ss.h"

#include "../schema.h"

namespace CTFO {

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

template <typename IMPL>
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
  using mmq_t = current::mmq::MMQ<mmq_message_t, IntermediateSubscriber<mmq_message_t>, 1024 * 1024>;

  template <typename... ARGS>
  GenericCruncherImpl(uint16_t port, const std::string& route, ARGS&&... args)
      : IMPL(std::forward<ARGS>(args)...),
        port_(port),
        mmq_worker_([this](mmq_message_t&& message, idxts_t) { message->Handle(*this); }),
        mmq_(mmq_worker_, 1024 * 1024) {
    scoped_http_routes_ += HTTP(port).Register(route + "/healthz", [](Request r) { r("OK\n"); }) +
                           HTTP(port).Register(route + "/data",
                                               [this](Request r) {
                                                 mmq_.Publish(std::make_unique<RequestMessage>(std::move(r)));
                                               }) +
                           HTTP(port).Register(route + "/html", [this](Request r) {
                             RenderExampleHTML(std::move(r));
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

 private:
  static std::string CardAsHTML(const CTFO::Card card) {
    return current::strings::Printf(
      "<div class='ctfo-card' style='background-color: rgb(%d, %d, %d);'><div class='ctfo-card__text'>%s</div></div>",
      card.color.red,
      card.color.green,
      card.color.blue,
      card.text.c_str());  // TODO(...): Escape the string, at least the `<>` parts of it.
  }
  static void RenderExampleHTML(Request r) {
    CTFO::Card c1;
    c1.text = "Poverty is not a lack of character. It's a lack of cash.";
    c1.color.red = 182;
    c1.color.green = 105;
    c1.color.blue = 255;

    CTFO::Card c2;
    c2.text = "HTML used to work in Windows 3.1 times, and it's still here.";
    c2.color.red = 0;
    c2.color.green = 128;
    c2.color.blue = 128;

    std::ostringstream os;
    os << "<html lang='en'>\n  <head>\n    <title>CTFO Card</title>\n    <style>\n      .ctfo-card {\n        display: inline-block;\n        vertical-align: top;\n        -webkit-box-sizing: border-box;\n        -moz-box-sizing: border-box;\n        box-sizing: border-box;\n        width: 320px;\n        height: auto;\n        min-height: 460px;\n        margin: 0;\n        padding: 24px 20px;\n        background-color: rgb(0, 0, 0);\n        font-family: \'Arial\', sans-serif;\n        font-size: 32px;\n        line-height: 48px;\n        text-align: left;\n      }\n      .ctfo-card__text {\n        display: inline;\n        padding: 4px 6px;\n        background-color: rgb(0, 0, 0);\n        color: rgb(255, 255, 255);\n      }\n    </style>\n  </head>\n";
    os << "<body>\n";
    os << "<h1>Test</h1>\n";
    os << "<table>";
    os << "<tr><td>" << CardAsHTML(c1) << "</td><td><font size=+5>All this</font></td></tr>";
    os << "<tr><td>" << CardAsHTML(c2) << "</td><td><font size=+5>is awesome</font></td></tr>";
    os << "</table>";
    os << "</body>\n";
    os << "</html>";
    r(os.str(), HTTPResponseCode.OK, current::net::constants::kDefaultHTMLContentType);
  }

 private:
  const uint16_t port_;
  HTTPRoutesScope scoped_http_routes_;
  IntermediateSubscriber<mmq_message_t> mmq_worker_;
  mmq_t mmq_;
};

template <typename IMPL>
using StreamCruncher = current::ss::StreamSubscriber<GenericCruncherImpl<IMPL>, typename IMPL::event_t>;

CURRENT_STRUCT_T(CruncherResponse) {
  CURRENT_FIELD(comment, std::string);
  CURRENT_FIELD(timestamp, std::chrono::microseconds, std::chrono::microseconds(0));
  CURRENT_FIELD(value, T);
};

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

 private:
  std::chrono::microseconds last_event_us_;
  std::vector<std::unique_ptr<cruncher_t>> crunchers_;
};

}  // namespace CTFO

#endif  // CRUNCHER_H
