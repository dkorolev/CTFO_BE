/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Maxim Zhurovich <zhurovich@gmail.com>
          (c) 2015 Dmitry "Dima" Korolev <dmitry.korolev@gmail.com>

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

#ifndef CTFO_SERVER_H
#define CTFO_SERVER_H

#include <cassert>
#include <cstdlib>
#include <functional>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "../Current/Bricks/file/file.h"
#include "../Current/Bricks/strings/strings.h"
#include "../Current/Bricks/util/random.h"

#include "../Current/Blocks/HTTP/api.h"
#include "../Current/Blocks/SelfModifyingConfig/config.h"

#include "../Current/Storage/api.h"
#ifdef USE_BASIC_HYPERMEDIA
#include "../Current/Storage/rest/hypermedia.h"
#else
#include "../Current/Storage/rest/advanced_hypermedia.h"
#endif

#include "storage.h"
#include "util.h"

// Structured iOS events.
#include "../Current/Midichlorians/Server/server.h"

// iOS notifications sender.
#include "../Current/Integrations/OneSignal/ios_notifications_sender.h"

// TODO(dkorolev): Make these constructor parameters of `CTFOServer`.
static const size_t FLAGS_blocks_to_ban = 5u;
static const size_t FLAGS_reports_to_ban = 10u;

namespace CTFO {

struct CTFOHypermedia
#ifdef USE_BASIC_HYPERMEDIA
    : current::storage::rest::Hypermedia {
  using super_t = current::storage::rest::Hypermedia;
#else
    : current::storage::rest::AdvancedHypermedia {
  using super_t = current::storage::rest::AdvancedHypermedia;
#endif

  template <class HTTP_VERB, typename PARTICULAR_FIELD, typename ENTRY, typename KEY>
  struct RESTful : super_t::RESTful<HTTP_VERB, PARTICULAR_FIELD, ENTRY, KEY> {
    using restful_super_t = super_t::RESTful<HTTP_VERB, PARTICULAR_FIELD, ENTRY, KEY>;
    using headers_list_t = std::vector<current::net::http::Header>;
    headers_list_t headers;

    template <typename F>
    void Enter(Request request, F&& next) {
      const char prefix[] = "X-";
      const size_t prefix_length = ::strlen(prefix);
      for (const auto& header : request.headers) {
        if (!header.header.compare(0, prefix_length, prefix)) {
          headers.push_back(header);
        }
      }
      this->restful_super_t::template Enter<F>(std::move(request), std::move(next));
    }

    template <class INPUT, bool IS_GET = std::is_same<HTTP_VERB, GET>::value>
    std::enable_if_t<!IS_GET, Response> Run(const INPUT& input) const {
      for (const auto& header : headers) {
        input.fields.SetTransactionMetaField(header.header, header.value);
      }
      return this->restful_super_t::template Run<INPUT>(input);
    }

    template <class INPUT, bool IS_GET = std::is_same<HTTP_VERB, GET>::value>
    std::enable_if_t<IS_GET, Response> Run(const INPUT& input) const {
      return this->restful_super_t::template Run<INPUT>(input);
    }
  };
};

CURRENT_STRUCT(CTFOServerParams) {
  CURRENT_FIELD(api_port, uint16_t);
  CURRENT_FIELD_DESCRIPTION(api_port, "Port to spawn CTFO API on.");
  CURRENT_FIELD(rest_port, uint16_t);
  CURRENT_FIELD_DESCRIPTION(rest_port, "Port to spawn RESTful server on.");
  CURRENT_FIELD(midichlorians_port, uint16_t);
  CURRENT_FIELD_DESCRIPTION(midichlorians_port, "Port to spawn midichlorians server on.");
  CURRENT_FIELD(storage_file, std::string);
  CURRENT_FIELD_DESCRIPTION(storage_file, "The file to store the snapshot of the database in.");
  CURRENT_FIELD(cards_file, std::string);
  CURRENT_FIELD_DESCRIPTION(cards_file, "Cards data file in JSON format.");
  CURRENT_FIELD(midichlorians_file, std::string);
  CURRENT_FIELD_DESCRIPTION(midichlorians_file, "Log file to store events received by midichlorians server.");
  CURRENT_FIELD(rest_url_prefix, std::string);
  CURRENT_FIELD_DESCRIPTION(rest_url_prefix, "Hypermedia route prefix to spawn RESTful server on.");
  CURRENT_FIELD(rest_url_path, std::string, "/ctfo/rest");
  CURRENT_FIELD_DESCRIPTION(rest_url_path, "The HTTP url path to expose RESTful server API on.")
  CURRENT_FIELD(raw_log_url_path, Optional<std::string>, "/raw_log");
  CURRENT_FIELD_DESCRIPTION(raw_log_url_path,
                            "The HTTP url path to expose the storage underlying stream raw log in.");
  CURRENT_FIELD(midichlorians_url_path, std::string, "/ctfo/log");
  CURRENT_FIELD_DESCRIPTION(midichlorians_url_path, "The HTTP url path for the midichlorians server.");
  CURRENT_FIELD(tick_interval_ms, std::chrono::milliseconds);
  CURRENT_FIELD_DESCRIPTION(tick_interval_ms, "Maximum interval between event entries.");
  CURRENT_FIELD(debug_print_to_stderr, bool, false);
  CURRENT_FIELD_DESCRIPTION(debug_print_to_stderr, "Print debug info to stderr.");

  CURRENT_FIELD(onesignal_app_id, std::string, "");
  CURRENT_FIELD_DESCRIPTION(onesignal_app_id, "OneSignal AppID to send iOS push notifications.");

  CURRENT_FIELD(
      onesignal_app_port, uint16_t, current::integrations::onesignal::kDefaultOneSignalIntegrationPort);
  CURRENT_FIELD_DESCRIPTION(onesignal_app_port, "The local port routed via nginx to connect to OneSignal API.");

  CURRENT_DEFAULT_CONSTRUCTOR(CTFOServerParams) {}

  CTFOServerParams& SetAPIPort(uint16_t port) {
    api_port = port;
    return *this;
  }
  CTFOServerParams& SetRESTPort(uint16_t port) {
    rest_port = port;
    return *this;
  }
  CTFOServerParams& SetMidichloriansPort(uint16_t port) {
    midichlorians_port = port;
    return *this;
  }
  CTFOServerParams& SetStorageFile(const std::string& file) {
    storage_file = file;
    return *this;
  }
  CTFOServerParams& SetCardsFile(const std::string& file) {
    cards_file = file;
    return *this;
  }
  CTFOServerParams& SetMidichloriansFile(const std::string& file) {
    midichlorians_file = file;
    return *this;
  }
  CTFOServerParams& SetRESTURLPrefix(const std::string& prefix) {
    rest_url_prefix = prefix;
    return *this;
  }
  CTFOServerParams& SetRESTURLPath(const std::string& path) {
    rest_url_path = path;
    return *this;
  }
  CTFOServerParams& SetRawLogURLPath(const std::string& path) {
    raw_log_url_path = path;
    return *this;
  }
  CTFOServerParams& SetMidichloriansURLPath(const std::string& path) {
    midichlorians_url_path = path;
    return *this;
  }
  CTFOServerParams& SetTickInterval(std::chrono::milliseconds ms) {
    tick_interval_ms = ms;
    return *this;
  }
  CTFOServerParams& SetDebugPrint(bool enable) {
    debug_print_to_stderr = enable;
    return *this;
  }
};

class CTFOServer final {
 public:
  using Storage = CTFOStorage<SherlockStreamPersister>;
  using MidichloriansServer = current::midichlorians::server::MidichloriansHTTPServer<CTFOServer>;
  using RESTStorage = RESTfulStorage<Storage, CTFOHypermedia>;
  using Config = current::SelfModifyingConfig<CTFOServerParams>;

  explicit CTFOServer(const std::string& config_path)
      : config_(config_path),
        midichlorians_server_(config_.Config().midichlorians_port,
                              *this,
                              config_.Config().tick_interval_ms,
                              config_.Config().midichlorians_url_path,
                              "OK\n"),
        storage_(current::sherlock::SherlockNamespaceName("OldCTFOStorage"), config_.Config().storage_file),
        rest_(storage_,
              config_.Config().rest_port,
              config_.Config().rest_url_path,
              config_.Config().rest_url_prefix),
        pusher_(storage_,
                [this]() {
                  std::chrono::microseconds result(0ll);
                  storage_.ReadOnlyTransaction([&result](ImmutableFields<Storage> fields) {
                    const auto placeholder = fields.push_notifications_marker[""];
                    if (Exists(placeholder)) {
                      result = Value(placeholder).last_pushed_notification_timestamp;
                    }
                  }).Go();
                  return result;
                }(),
                config_.Config().onesignal_app_id,
                config_.Config().onesignal_app_port),
        push_notifications_sender_subcriber_scope_(storage_.InternalExposeStream().Subscribe(pusher_)) {
    midichlorians_stream_.open(config_.Config().midichlorians_file, std::ofstream::out | std::ofstream::app);

#ifdef MUST_IMPORT_INITIAL_CTFO_CARDS
    std::ifstream cf(config_.Config().cards_file);
    assert(cf.good());
    storage_.ReadWriteTransaction([&cf](MutableFields<Storage> data) {
      User admin_user;
      admin_user.uid = admin_uid;
      admin_user.level = 80;         // Superuser 80lvl.
      admin_user.score = 999999999;  // With one short to one billion points.
      data.user.Add(admin_user);
      std::string line;
      while (std::getline(cf, line)) {
        const Card card = ParseJSON<Card>(line);
        if (!Exists(data.card[card.cid])) {
          data.card.Add(card);
          data.author_card.Add(AuthorCard{admin_uid, card.cid});
        }
      }
    }).Go();
#endif

    uint16_t port = config_.Config().api_port;
    scoped_http_routes_ += HTTP(port).Register("/healthz", [](Request r) { r("OK\n"); }) +
                           HTTP(port).Register("/ctfo/auth/ios", BindToThis(&CTFOServer::RouteAuthiOS)) +
                           HTTP(port).Register("/ctfo/feed", BindToThis(&CTFOServer::RouteFeed)) +
                           HTTP(port).Register("/ctfo/favs", BindToThis(&CTFOServer::RouteFavorites)) +
                           HTTP(port).Register("/ctfo/my_cards", BindToThis(&CTFOServer::RouteMyCards)) +
                           HTTP(port).Register("/ctfo/card", BindToThis(&CTFOServer::RouteCard)) +
                           HTTP(port).Register("/ctfo/comments", BindToThis(&CTFOServer::RouteComments)) +
                           HTTP(port).Register("/ctfo/comment", BindToThis(&CTFOServer::RouteComments)) +
                           HTTP(port).Register("/initial_cards", BindToThis(&CTFOServer::RouteInitialCards));

    if (Exists(config_.Config().raw_log_url_path)) {
      const auto route = Value(config_.Config().raw_log_url_path);
      DebugPrint("Registering raw log on: `" + route + "`.");
      scoped_http_routes_ += HTTP(port).Register(
          route, URLPathArgs::CountMask::None | URLPathArgs::CountMask::One, storage_.InternalExposeStream());
    }
  }

  void Join() { HTTP(config_.Config().api_port).Join(); }

  template <typename T_EVENT>
  void operator()(const T_EVENT& e) {
    DebugPrint("Not an `iOSGenericEvent`: " + JSON(e));
  }

  void operator()(const current::midichlorians::server::TickLogEntry&) {}

  void operator()(const current::midichlorians::server::EventLogEntry& e) {
    midichlorians_stream_ << JSON(e) << std::endl;
    e.event.Call(*this);
  }

  void operator()(const current::midichlorians::ios::iOSGenericEvent& event) { UpdateStateOnEvent(event); }

 private:
  typedef void (CTFOServer::*CTFOServerMemberFunctionServingRequest)(Request);

  std::function<void(Request)> BindToThis(CTFOServerMemberFunctionServingRequest handler) {
    return std::bind(handler, this, std::placeholders::_1);
  };

  template <typename T_REQUEST>
  T_REQUEST ParseRequest(const std::string& source) {
    return Value<T_REQUEST>(ParseJSON<Variant<T_REQUEST>, JSONFormat::Minimalistic>(source));
  }

  template <typename T_RESPONSE>
  Response MakeResponse(T_RESPONSE&& r) {
    return Response(JSON<JSONFormat::Minimalistic>(Variant<T_RESPONSE>(std::forward<T_RESPONSE>(r))),
                    HTTPResponseCode.OK,
                    current::net::constants::kDefaultJSONContentType);
  }

  void RouteAuthiOS(Request r) {
    if (r.method != "POST") {
      DebugPrint(Printf("[/ctfo/auth/ios] Wrong method '%s'. Requested URL = '%s'",
                        r.method.c_str(),
                        r.url.ComposeURL().c_str()));
      r("METHOD NOT ALLOWED\n", HTTPResponseCode.MethodNotAllowed);
    } else {
      const std::string device_id = r.url.query.get("id", "");
      const std::string app_key = r.url.query.get("key", "");
      const std::string& notifications_since_as_string = r.url.query["notifications_since"];
      const uint64_t notification_since_ms = notifications_since_as_string.empty()
                                                 ? static_cast<uint64_t>(-1)
                                                 : current::FromString<uint64_t>(notifications_since_as_string);
      if (device_id.empty() || app_key.empty()) {
        DebugPrint(Printf("[/ctfo/auth/ios] Wrong query parameters. Requested URL = '%s'",
                          r.url.ComposeURL().c_str()));
        r("NEED VALID ID-KEY PAIR\n", HTTPResponseCode.BadRequest);
      } else {
        const size_t feed_count = current::FromString<size_t>(r.url.query.get("feed_count", "20"));
        // Searching for users with the corresponding authentication key.
        storage_.ReadWriteTransaction(  // clang-format off
            [this, device_id, app_key, feed_count, notification_since_ms](MutableFields<Storage> data) {
              AuthKey auth_key("iOS::" + device_id + "::" + app_key, AUTH_TYPE::IOS);
              UID uid = UID::INVALID_USER;
              User user;
              ResponseUserEntry user_entry;
              std::string token;

              const auto& uid_auth_accessor = data.uid_auth;
              if (uid_auth_accessor.Cols().Has(auth_key)) {
                uid = Value(uid_auth_accessor.GetEntryFromCol(auth_key)).uid;
              }

              auto& auth_token_mutator = data.auth_token;
              if (uid != UID::INVALID_USER) {
                // User exists => invalidate all tokens.
                for (const auto& auth_token : auth_token_mutator.Row(auth_key)) {
                  auth_token_mutator.Add(AuthKeyTokenPair(auth_key, auth_token.token, false));
                }
                user = Value(data.user[uid]);
              }

              // Generate a new token.
              do {
                token = RandomToken();
              } while (auth_token_mutator.Cols().Has(token));
              auth_token_mutator.Add(AuthKeyTokenPair(auth_key, token, true));

              if (uid != UID::INVALID_USER) {  // Existing user.
                user_entry.score = user.score;
                DebugPrint(
                  Printf("[/ctfo/auth/ios] Existing user: UID='%s', DeviceID='%s', AppKey='%s', Token='%s'",
                         UIDToString(uid).c_str(),
                         device_id.c_str(),
                         app_key.c_str(),
                         token.c_str()));
              } else {  // New user.
                uid = RandomUID();
                user.uid = uid;
                data.user.Add(user);
                data.uid_auth.Add(UIDAuthKeyPair(user.uid, auth_key));
              }
              DebugPrint(
                Printf("[/ctfo/auth/ios] New user: UID='%s', DeviceID='%s', AppKey='%s', Token='%s'",
                       UIDToString(uid).c_str(),
                       device_id.c_str(),
                       app_key.c_str(),
                       token.c_str()));

              CopyUserInfoToResponseEntry(user, Exists(data.banned_user[uid]), user_entry);
              user_entry.token = token;

              return MakeResponse(GenerateResponseFeed(data, user_entry, feed_count, notification_since_ms));
            }, std::move(r)).Go();  // clang-format on
      }
    }
  }
  void RouteFeed(Request r) {
    const UID uid = StringToUID(r.url.query["uid"]);
    const std::string& notifications_since_as_string = r.url.query["notifications_since"];
    const uint64_t notification_since_ms = notifications_since_as_string.empty()
                                               ? static_cast<uint64_t>(-1)
                                               : current::FromString<uint64_t>(notifications_since_as_string);
    const std::string token = r.url.query["token"];
    if (r.method != "GET") {
      DebugPrint(Printf("[/ctfo/feed] Wrong method '%s'. Requested URL = '%s'",
                        r.method.c_str(),
                        r.url.ComposeURL().c_str()));
      r("METHOD NOT ALLOWED\n", HTTPResponseCode.MethodNotAllowed);
    } else {
      if (uid == UID::INVALID_USER) {
        DebugPrint(Printf("[/ctfo/feed] Wrong UID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.BadRequest);
      } else {
        const size_t feed_count = current::FromString<size_t>(r.url.query.get("feed_count", "20"));
        const std::string requested_url = r.url.ComposeURL();
        storage_.ReadOnlyTransaction(  // clang-format off
            [this, uid, token, requested_url, feed_count, notification_since_ms](ImmutableFields<Storage> data) {
              bool token_is_valid = false;
              const auto& auth_token_accessor = data.auth_token;
              if (auth_token_accessor.Cols().Has(token)) {
                if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
                  // Double check, if the provided `uid` is correct as well.
                  const auto& uid_auth_accessor = data.uid_auth;
                  token_is_valid = Exists(uid_auth_accessor.Get(
                    uid, Value(auth_token_accessor.GetEntryFromCol(token)).auth_key));
                }
              }
              if (!token_is_valid) {
                DebugPrint(Printf("[/ctfo/feed] Invalid token. Requested URL = '%s'", requested_url.c_str()));
                return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
              } else {
                DebugPrint(Printf("[/ctfo/feed] Token validated. Requested URL = '%s'",
                                  requested_url.c_str()));
                const auto user = Value(data.user[uid]);
                ResponseUserEntry user_entry;
                CopyUserInfoToResponseEntry(user, Exists(data.banned_user[uid]), user_entry);
                user_entry.token = token;
                return MakeResponse(GenerateResponseFeed(data, user_entry, feed_count, notification_since_ms));
              }
            }, std::move(r)).Go();  // clang-format on
      }
    }
  }
  void RouteInitialCards(Request r) {
    storage_.ReadOnlyTransaction([this](ImmutableFields<Storage> data) {
      return MakeResponse(
          GenerateResponseFeed(data, ResponseUserEntry(), 10000u, static_cast<uint64_t>(-1), 10000u, true));
    }, std::move(r)).Go();
  }
  void RouteFavorites(Request r) {
    // TODO(dkorolev): Avoid this shameless copy-pasting.
    const UID uid = StringToUID(r.url.query["uid"]);
    const std::string token = r.url.query["token"];
    if (r.method != "GET") {
      DebugPrint(Printf("[/ctfo/favs] Wrong method '%s'. Requested URL = '%s'",
                        r.method.c_str(),
                        r.url.ComposeURL().c_str()));
      r("METHOD NOT ALLOWED\n", HTTPResponseCode.MethodNotAllowed);
    } else {
      if (uid == UID::INVALID_USER) {
        DebugPrint(Printf("[/ctfo/favs] Wrong UID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.BadRequest);
      } else {
        storage_.ReadOnlyTransaction([this, uid, token](ImmutableFields<Storage> data) {
          bool token_is_valid = false;
          const auto& auth_token_accessor = data.auth_token;
          if (auth_token_accessor.Cols().Has(token)) {
            if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
              // Double check, if the provided `uid` is correct as well.
              const auto& uid_auth_accessor = data.uid_auth;
              token_is_valid = Exists(
                  uid_auth_accessor.Get(uid, Value(auth_token_accessor.GetEntryFromCol(token)).auth_key));
            }
          }
          if (!token_is_valid) {
            DebugPrint("[/ctfo/favs] Invalid token.");
            return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
          } else {
            DebugPrint("[/ctfo/favs] Token validated.");
            const auto user = data.user[uid];
            if (!Exists(user)) {
              return Response("NO SUCH USER\n", HTTPResponseCode.NotFound);
            } else {
              ResponseFavs rfavs;
              CopyUserInfoToResponseEntry(Value(user), Exists(data.banned_user[uid]), rfavs.user);

              const auto& answers = data.answer;

              // Get favs.
              std::vector<std::pair<std::chrono::microseconds, CID>> favs;
              const auto& favorites = data.favorite;
              for (const auto& fav : favorites.Row(uid)) {
                if (fav.favorited) {
                  if (!Exists(data.flagged_card.Get(fav.cid, uid))) {
                    const auto& author = data.author_card.GetEntryFromCol(fav.cid);
                    if (!Exists(author) || !Exists(data.user_blocked_user.Get(uid, Value(author).uid))) {
                      favs.emplace_back(fav.us, fav.cid);
                    }
                  }
                }
              }

              // In reverse chronological order.
              std::sort(favs.rbegin(), favs.rend());

              // And publish them.
              const auto& author_card = data.author_card;
              const auto& comments = data.comment;
              const auto GenerateCardForFavs =
                  [this, uid, &answers, &author_card, &comments](const Card& card) {
                    ResponseCardEntry card_entry;
                    card_entry.cid = CIDToString(card.cid);
                    const auto card_auth = author_card.GetEntryFromCol(card.cid);
                    if (Exists(card_auth)) {
                      const UID author_uid = Value(card_auth).uid;
                      card_entry.author_uid = UIDToString(author_uid);
                      card_entry.is_my_card = (uid == author_uid);
                    }
                    card_entry.number_of_comments = comments.Row(card.cid).Size();
                    const uint64_t now = current::time::Now().count();
                    card_entry.text = card.text;
                    card_entry.ms = std::chrono::duration_cast<std::chrono::milliseconds>(card.us);
                    card_entry.color = card.color;
                    card_entry.relevance =
                        0.9 * std::pow(0.99, (now - card.us.count()) * (1.0 / (1000ll * 1000 * 60 * 60 * 24)));
                    card_entry.ctfo_score = 50u;
                    card_entry.tfu_score = 50u;
                    card_entry.ctfo_count = card.ctfo_count;
                    card_entry.tfu_count = card.tfu_count;
                    card_entry.skip_count = card.skip_count;

                    const auto answer = answers.Get(uid, card.cid);
                    if (Exists(answer)) {
                      const ANSWER vote = Value(answer).answer;
                      if (vote == ANSWER::CTFO) {
                        card_entry.vote = "CTFO";
                      } else if (vote == ANSWER::TFU) {
                        card_entry.vote = "TFU";
                      } else if (vote == ANSWER::SKIP) {
                        card_entry.vote = "SKIP";
                      }
                    }

                    card_entry.favorited = true;

                    return card_entry;
                  };

              for (const auto& c : favs) {
                const auto card = data.card[c.second];
                if (Exists(card)) {
                  rfavs.cards.push_back(GenerateCardForFavs(Value(card)));
                }
              }

              rfavs.ms = std::chrono::duration_cast<std::chrono::milliseconds>(current::time::Now());
              return MakeResponse(std::move(rfavs));
            }
          }
        }, std::move(r)).Go();
      }
    }
  }

  void RouteMyCards(Request r) {
    // TODO(dkorolev): Avoid this shameless copy-pasting.
    const UID uid = StringToUID(r.url.query["uid"]);
    const std::string token = r.url.query["token"];
    if (r.method != "GET") {
      DebugPrint(Printf("[/ctfo/my_cards] Wrong method '%s'. Requested URL = '%s'",
                        r.method.c_str(),
                        r.url.ComposeURL().c_str()));
      r("METHOD NOT ALLOWED\n", HTTPResponseCode.MethodNotAllowed);
    } else {
      if (uid == UID::INVALID_USER) {
        DebugPrint(Printf("[/ctfo/my_cards] Wrong UID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.BadRequest);
      } else {
        storage_.ReadOnlyTransaction([this, uid, token](ImmutableFields<Storage> data) {
          bool token_is_valid = false;
          const auto& auth_token_accessor = data.auth_token;
          if (Exists(auth_token_accessor.GetEntryFromCol(token))) {
            if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
              // Double check, if the provided `uid` is correct as well.
              const auto& uid_auth_accessor = data.uid_auth;
              token_is_valid = Exists(
                  uid_auth_accessor.Get(uid, Value(auth_token_accessor.GetEntryFromCol(token)).auth_key));
            }
          }
          if (!token_is_valid) {
            DebugPrint("[/ctfo/my_cards] Invalid token.");
            return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
          } else {
            DebugPrint("[/ctfo/my_cards] Token validated.");
            const auto user = data.user[uid];
            if (!Exists(user)) {
              return Response("NO SUCH USER\n", HTTPResponseCode.NotFound);
            } else {
              ResponseMyCards r_my_cards;
              CopyUserInfoToResponseEntry(Value(user), Exists(data.banned_user[uid]), r_my_cards.user);

              const auto& answers = data.answer;
              const auto& favorites = data.favorite;

              // Get my cards.
              std::vector<std::pair<std::chrono::microseconds, CID>> my_cards;
              for (const auto& my_card : data.author_card.Row(uid)) {
                my_cards.emplace_back(my_card.us, my_card.cid);
              }

              // In reverse chronological order.
              std::sort(my_cards.rbegin(), my_cards.rend());

              // And publish them.
              const auto& author_cards = data.author_card;
              const auto& comments = data.comment;
              const auto GenerateCardForMyCards =
                  [this, uid, &answers, &favorites, &author_cards, &comments](const Card& card) {
                    ResponseCardEntry card_entry;
                    card_entry.cid = CIDToString(card.cid);
                    const auto card_auth = author_cards.GetEntryFromCol(card.cid);
                    if (Exists(card_auth)) {
                      const UID author_uid = Value(card_auth).uid;
                      card_entry.author_uid = UIDToString(author_uid);
                      card_entry.is_my_card = (uid == author_uid);
                    }
                    card_entry.number_of_comments = comments.Row(card.cid).Size();
                    const uint64_t now = current::time::Now().count();
                    card_entry.text = card.text;
                    card_entry.ms = std::chrono::duration_cast<std::chrono::milliseconds>(card.us);
                    card_entry.color = card.color;
                    card_entry.relevance =
                        0.9 * std::pow(0.99, (now - card.us.count()) * (1.0 / (1e6 * 60 * 60 * 24)));
                    card_entry.ctfo_score = 50u;
                    card_entry.tfu_score = 50u;
                    card_entry.ctfo_count = card.ctfo_count;
                    card_entry.tfu_count = card.tfu_count;
                    card_entry.skip_count = card.skip_count;

                    const auto answer = answers.Get(uid, card.cid);
                    if (Exists(answer)) {
                      const ANSWER vote = Value(answer).answer;
                      if (vote == ANSWER::CTFO) {
                        card_entry.vote = "CTFO";
                      } else if (vote == ANSWER::TFU) {
                        card_entry.vote = "TFU";
                      } else if (vote == ANSWER::SKIP) {
                        card_entry.vote = "SKIP";
                      }
                    }

                    card_entry.favorited = false;
                    const auto fav = favorites.Get(uid, card.cid);
                    if (Exists(fav)) {
                      card_entry.favorited = Value(fav).favorited;
                    }

                    return card_entry;
                  };

              for (const auto& c : my_cards) {
                const auto card = data.card[c.second];
                if (Exists(card)) {
                  r_my_cards.cards.push_back(GenerateCardForMyCards(Value(card)));
                }
              }

              r_my_cards.ms = std::chrono::duration_cast<std::chrono::milliseconds>(current::time::Now());
              return MakeResponse(std::move(r_my_cards));
            }
          }
        }, std::move(r)).Go();
      }
    }
  }

  void RouteCard(Request r) {
    if (r.method == "GET") {
      // For GET /ctfo/card, don't bother checking user and/or token, just return the card.
      const std::string& uid_as_string = r.url.query["uid"];  // OK if not provided.
      const CID cid = StringToCID(r.url.query["cid"]);
      const UID uid = StringToUID(uid_as_string);
      if (cid == CID::INVALID_CARD) {
        DebugPrint(Printf("[/ctfo/card] Wrong CID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED VALID CID\n", HTTPResponseCode.BadRequest);
      } else if (!uid_as_string.empty() && uid == UID::INVALID_USER) {
        DebugPrint(
            Printf("[/ctfo/card] Non-empty yet invalid UID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED EMPTY OR VALID UID\n", HTTPResponseCode.BadRequest);
      } else {
        storage_.ReadOnlyTransaction([this, cid, uid](ImmutableFields<Storage> data) {
          const auto card_wrapper = data.card[cid];
          if (!Exists(card_wrapper)) {
            return Response("NO SUCH CARD\n", HTTPResponseCode.NotFound);
          } else {
            const Card& card = Value(card_wrapper);
            const auto& answers = data.answer;
            const auto& comments = data.comment;
            const auto& favorites = data.favorite;
            const auto& author_cards = data.author_card;
            // TODO(dkorolev): This beautiful copy-paste will eventually go away.
            ResponseCardEntry card_entry;
            card_entry.cid = CIDToString(card.cid);
            const auto card_auth = author_cards.GetEntryFromCol(card.cid);
            if (Exists(card_auth)) {
              const UID author_uid = Value(card_auth).uid;
              card_entry.author_uid = UIDToString(author_uid);
              card_entry.is_my_card = (uid == UID::INVALID_USER ? false : (uid == author_uid));
            }
            card_entry.number_of_comments = comments.Row(card.cid).Size();
            card_entry.text = card.text;
            card_entry.ms = std::chrono::duration_cast<std::chrono::milliseconds>(card.us);
            card_entry.color = card.color;
            card_entry.relevance = 1.0;  // When `GET`-ting a card, make it 1.0.
            card_entry.ctfo_score = 50u;
            card_entry.tfu_score = 50u;
            card_entry.ctfo_count = card.ctfo_count;
            card_entry.tfu_count = card.tfu_count;
            card_entry.skip_count = card.skip_count;

            card_entry.vote = "";
            card_entry.favorited = false;
            if (uid != UID::INVALID_USER) {
              const auto answer = answers.Get(uid, card.cid);
              if (Exists(answer)) {
                const ANSWER vote = Value(answer).answer;
                if (vote == ANSWER::CTFO) {
                  card_entry.vote = "CTFO";
                } else if (vote == ANSWER::TFU) {
                  card_entry.vote = "TFU";
                } else if (vote == ANSWER::SKIP) {
                  card_entry.vote = "SKIP";
                }
              }
              const auto fav = favorites.Get(uid, card.cid);
              if (Exists(fav)) {
                card_entry.favorited = Value(fav).favorited;
              }
            }

            return MakeResponse(std::move(card_entry));
          }
        }, std::move(r)).Go();
      }
    } else if (r.method == "POST") {
      const UID uid = StringToUID(r.url.query["uid"]);
      const std::string token = r.url.query["token"];
      if (uid == UID::INVALID_USER) {
        DebugPrint(Printf("[/ctfo/card] Wrong UID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.BadRequest);
      } else {
        const std::string requested_url = r.url.ComposeURL();
        const CID cid = RandomCID();
        try {
          RequestAddCard request;
          try {
            request = ParseRequest<RequestAddCard>(r.body);
          } catch (const current::TypeSystemParseJSONException&) {
            const auto short_request = ParseRequest<RequestAddCardShort>(r.body);
            request.text = short_request.text;
            request.color = CARD_COLORS[static_cast<uint64_t>(cid) % CARD_COLORS.size()];
          }
          storage_.ReadWriteTransaction(  // clang-format off
              [this, cid, uid, token, request, requested_url](MutableFields<Storage> data) {
                bool token_is_valid = false;
                const auto& auth_token_accessor = data.auth_token;
                if (Exists(auth_token_accessor.GetEntryFromCol(token))) {
                  if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
                    // Double check, if the provided `uid` is correct as well.
                    const auto& uid_auth_accessor = data.uid_auth;
                    token_is_valid = Exists(uid_auth_accessor.Get(
                        uid, Value(auth_token_accessor.GetEntryFromCol(token)).auth_key));
                  }
                }
                if (!token_is_valid) {
                  DebugPrint(Printf("[/ctfo/card] Invalid token. Requested URL = '%s'",
                                    requested_url.c_str()));
                  return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
                } else {
                  DebugPrint(Printf("[/ctfo/card] Token validated. Requested URL = '%s'",
                                    requested_url.c_str()));
                  const auto now = current::time::Now();

                  auto& cards_mutator = data.card;
                  auto& authors_mutator = data.author_card;

                  Card card;
                  card.cid = cid;
                  card.text = request.text;
                  card.color = request.color;
                  cards_mutator.Add(card);

                  AuthorCard author;
                  author.uid = uid;
                  author.cid = cid;
                  authors_mutator.Add(author);

                  ResponseAddCard response;
                  response.ms = std::chrono::duration_cast<std::chrono::milliseconds>(now);
                  response.cid = CIDToString(cid);
                  return MakeResponse(std::move(response));
                }
              }, std::move(r)).Go();  // clang-format on
        } catch (const current::TypeSystemParseJSONException& e) {
          DebugPrint(Printf("[/ctfo/card] Could not parse POST body. Requested URL = '%s', e = '%s'",
                            r.url.ComposeURL().c_str(),
                            e.what()));
          r("NEED VALID BODY\n", HTTPResponseCode.BadRequest);
        }
      }
    } else if (r.method == "DELETE") {
      const UID uid = StringToUID(r.url.query["uid"]);
      const std::string token = r.url.query["token"];
      const CID cid = StringToCID(r.url.query["cid"]);
      if (cid == CID::INVALID_CARD) {
        DebugPrint(Printf("[/ctfo/card] Wrong CID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED VALID CID\n", HTTPResponseCode.BadRequest);
      } else {
        const std::string requested_url = r.url.ComposeURL();
        storage_.ReadWriteTransaction([this, requested_url, uid, cid, token](MutableFields<Storage> data) {
          bool token_is_valid = false;
          const auto& auth_token_accessor = data.auth_token;
          if (Exists(auth_token_accessor.GetEntryFromCol(token))) {
            if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
              // Double check, if the provided `uid` is correct as well.
              const auto& uid_auth_accessor = data.uid_auth;
              token_is_valid = Exists(
                  uid_auth_accessor.Get(uid, Value(auth_token_accessor.GetEntryFromCol(token)).auth_key));
            }
          }
          if (!token_is_valid) {
            DebugPrint(Printf("[/ctfo/card] Invalid token. Requested URL = '%s'", requested_url.c_str()));
            return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
          } else {
            DebugPrint(Printf("[/ctfo/card] Token validated. Requested URL = '%s'", requested_url.c_str()));
            auto& cards_mutator = data.card;
            auto& author_cards_mutator = data.author_card;
            const auto author_card = author_cards_mutator.GetEntryFromCol(cid);
            if (Exists(author_card)) {
              const UID author_uid = Value(author_card).uid;
              if (author_uid != uid) {
                return Response("NOT YOUR CARD BRO\n", HTTPResponseCode.Unauthorized);
              } else {
                cards_mutator.Erase(cid);
                author_cards_mutator.Erase(uid, cid);
                auto& comments_mutator = data.comment;
                std::vector<OID> oids_to_delete;
                const auto comments_per_card = comments_mutator.Rows()[cid];
                for (const auto& c : comments_mutator.Row(cid)) {
                  oids_to_delete.push_back(c.oid);
                }
                for (const OID& o : oids_to_delete) {
                  comments_mutator.Erase(cid, o);
                }
                ResponseDeleteCard response;
                response.ms = std::chrono::duration_cast<std::chrono::milliseconds>(current::time::Now());
                return MakeResponse(std::move(response));
              }
            } else {
              return Response("NO SUCH CARD\n", HTTPResponseCode.NotFound);
            }
          }
        }, std::move(r)).Go();
      }
    } else {
      DebugPrint(Printf("[/ctfo/card] Wrong method '%s'. Requested URL = '%s'",
                        r.method.c_str(),
                        r.url.ComposeURL().c_str()));
      r("METHOD NOT ALLOWED\n", HTTPResponseCode.MethodNotAllowed);
    }
  }

  void RouteComments(Request r) {
    // TODO(dkorolev): Avoid this shameless copy-pasting.
    const UID uid = StringToUID(r.url.query["uid"]);
    const std::string token = r.url.query["token"];
    const CID cid = StringToCID(r.url.query["cid"]);
    if (uid == UID::INVALID_USER) {
      DebugPrint(Printf("[/ctfo/comments] Wrong UID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
      r("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.BadRequest);
    } else if (cid == CID::INVALID_CARD) {
      DebugPrint(Printf("[/ctfo/comments] Wrong CID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
      r("NEED VALID CID\n", HTTPResponseCode.BadRequest);
    } else {
      if (r.method == "GET") {
        const std::string requested_url = r.url.ComposeURL();
        storage_.ReadOnlyTransaction([this, uid, cid, token, requested_url](ImmutableFields<Storage> data) {
          bool token_is_valid = false;
          const auto& auth_token_accessor = data.auth_token;
          if (Exists(auth_token_accessor.GetEntryFromCol(token))) {
            if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
              // Double check, if the provided `uid` is correct as well.
              const auto& uid_auth_accessor = data.uid_auth;
              token_is_valid = Exists(
                  uid_auth_accessor.Get(uid, Value(auth_token_accessor.GetEntryFromCol(token)).auth_key));
            }
          }
          if (!token_is_valid) {
            DebugPrint("[/ctfo/comments] Invalid token.");
            return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
          } else {
            DebugPrint("[/ctfo/comments] Token validated.");
            if (!Exists(data.card[cid])) {
              return Response("NO SUCH CARD\n", HTTPResponseCode.NotFound);
            }
            const auto user = data.user[uid];
            if (!Exists(user)) {
              return Response("NO SUCH USER\n", HTTPResponseCode.NotFound);
            } else {
              const auto& users_accessor = data.user;
              const auto& comments_accessor = data.comment;
              const auto& comment_likes_accessor = data.comment_like;
              const auto& comment_flagged_accessor = data.flagged_comment;
              std::vector<Comment> proto_comments;
              std::set<OID> flagged_comments;
              if (!comments_accessor.Row(cid).Empty()) {
                for (const auto& comment : comments_accessor.Row(cid)) {
                  if (Exists(comment_flagged_accessor.Get(comment.oid, uid))) {
                    flagged_comments.insert(comment.oid);
                  }
                  // Make sure to not return comments left by the users the current user has blocked,
                  // comments left by banned users, or comments that are 2nd level with the 1st level comment
                  // being a comment from the user whom the current user has blocked, or who are banned.
                  bool comment_hidden_due_to_block_or_ban = false;
                  if (Exists(data.user_blocked_user.Get(uid, comment.author_uid))) {
                    comment_hidden_due_to_block_or_ban = true;
                  } else if (Exists(data.banned_user[comment.author_uid])) {
                    comment_hidden_due_to_block_or_ban = true;
                  } else if (comment.parent_oid != OID::INVALID_COMMENT) {
                    const auto top_level_comment = comments_accessor.GetEntryFromCol(comment.parent_oid);
                    if (Exists(top_level_comment)) {
                      const Comment& parent_comment = Value(top_level_comment);
                      if (Exists(data.user_blocked_user.Get(uid, parent_comment.author_uid))) {
                        comment_hidden_due_to_block_or_ban = true;
                      } else if (Exists(data.banned_user[parent_comment.author_uid])) {
                        comment_hidden_due_to_block_or_ban = true;
                      }
                    }
                  }
                  if (!comment_hidden_due_to_block_or_ban) {
                    proto_comments.push_back(Value(comment));
                  }
                }
              }
              const auto sortkey = [&comments_accessor](const Comment& c) -> std::pair<uint64_t, uint64_t> {
                uint64_t comment_timestamp = 0u;
                uint64_t top_level_comment_timestamp = 0u;
                if (c.parent_oid == OID::INVALID_COMMENT) {
                  // This comment is top-level.
                  top_level_comment_timestamp = c.us.count();
                } else {
                  // This is a 2nd-level comment.
                  comment_timestamp = c.us.count();
                  const auto top_level_comment = comments_accessor.GetEntryFromCol(c.parent_oid);
                  if (Exists(top_level_comment)) {
                    top_level_comment_timestamp = Value(top_level_comment).us.count();
                  }
                }
                // Top-level comments reverse chronologically, 2nd level comments chronologically.
                return std::make_pair(~top_level_comment_timestamp, comment_timestamp);
              };
              std::sort(
                  proto_comments.begin(),
                  proto_comments.end(),
                  [&sortkey](const Comment& lhs, const Comment& rhs) { return sortkey(lhs) < sortkey(rhs); });
              ResponseComments response;
              response.ms = std::chrono::duration_cast<std::chrono::milliseconds>(current::time::Now());
              std::vector<ResponseComment>& output_comments = response.comments;
              for (const auto& comment : proto_comments) {
                // TODO(dkorolev): Need a function to convert `Comment` into `ResponseComment`.
                ResponseComment c;
                c.oid = OIDToString(comment.oid);
                if (comment.parent_oid != OID::INVALID_COMMENT) {
                  // `c.parent_oid` should be either parent comment ID, or blank string.
                  c.parent_oid = OIDToString(comment.parent_oid);
                }
                c.author_uid = UIDToString(comment.author_uid);
                c.author_level = 0;
                const auto user_data = users_accessor[comment.author_uid];
                if (Exists(user_data)) {
                  c.author_level = Value(user_data).level;
                }
                c.text = comment.text;
                const auto likers = comment_likes_accessor.Rows()[comment.oid];
                if (Exists(likers)) {
                  const auto v = Value(likers);
                  c.number_of_likes = v.Size();
                  c.liked = v.Has(uid);
                }
                c.flagged_inappropriate = flagged_comments.count(comment.oid);
                c.ms = std::chrono::duration_cast<std::chrono::milliseconds>(comment.us);
                output_comments.push_back(std::move(c));
              }
              return MakeResponse(std::move(response));
            }
          }
        }, std::move(r)).Go();
      } else if (r.method == "POST") {
        const std::string requested_url = r.url.ComposeURL();
        const OID oid = RandomOID();
        try {
          RequestAddComment request;
          try {
            request = ParseRequest<RequestAddComment>(r.body);
          } catch (const current::TypeSystemParseJSONException&) {
            const auto short_request = ParseRequest<RequestAddCommentShort>(r.body);
            request.text = short_request.text;
          }
          storage_.ReadWriteTransaction(  // clang-format off
              [this, cid, uid, oid, token, request, requested_url](MutableFields<Storage> data) {
                bool token_is_valid = false;
                const auto& auth_token_accessor = data.auth_token;
                if (Exists(auth_token_accessor.GetEntryFromCol(token))) {
                  if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
                    // Double check, if the provided `uid` is correct as well.
                    const auto& uid_auth_accessor = data.uid_auth;
                    token_is_valid = Exists(uid_auth_accessor.Get(
                        uid, Value(auth_token_accessor.GetEntryFromCol(token)).auth_key));
                  }
                }
                if (!token_is_valid) {
                  DebugPrint(Printf("[/ctfo/comments] Invalid token. Requested URL = '%s'",
                                    requested_url.c_str()));
                  return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
                } else {
                  DebugPrint(Printf("[/ctfo/comments] Token validated. Requested URL = '%s'",
                                    requested_url.c_str()));
                  const auto now = current::time::Now();

                  UID card_author_uid = UID::INVALID_USER;
                  OID parent_oid = (!request.parent_oid.empty()) ? StringToOID(request.parent_oid)
                                                                  : OID::INVALID_COMMENT;
                  UID parent_comment_author_uid = UID::INVALID_USER;
                  const auto& author_cards = data.author_card;
                  const auto element = author_cards.GetEntryFromCol(cid);
                  if (Exists(element)) {
                    card_author_uid = Value(element).uid;
                  }

                  auto& comments_mutator = data.comment;

                  Comment comment;
                  comment.cid = cid;
                  comment.oid = oid;
                  comment.author_uid = uid;
                  comment.text = request.text;

                  if (parent_oid != OID::INVALID_COMMENT) {
                    comment.parent_oid = parent_oid;
                    const auto v = comments_mutator.GetEntryFromCol(parent_oid);
                    if (Exists(v)) {
                      const Comment& parent_comment = Value(v);
                      if (parent_comment.parent_oid != OID::INVALID_COMMENT) {
                        return Response("ATTEMPTED TO ADD A 3RD LEVEL COMMENT\n",
                                        HTTPResponseCode.BadRequest);
                      }
                      parent_comment_author_uid = parent_comment.author_uid;
                    } else {
                      return Response("NEED EMPTY OR VALID PARENT_OID\n", HTTPResponseCode.BadRequest);
                    }
                  }

                  comments_mutator.Add(comment);

                  // Emit the "new comment on my card" notification.
                  if (card_author_uid != UID::INVALID_USER && card_author_uid != uid) {
                    // Do not send "new comment on my card" if it is the reply to my own comment.
                    if (parent_comment_author_uid == UID::INVALID_USER ||
                        parent_comment_author_uid != card_author_uid) {
                      data.notification.Add(
                          Notification(card_author_uid, now, NotificationMyCardNewComment(comment)));
                    }
                  }

                  // Emit the "new comment on a card you starred" notification.
                  for (const Favorite& fav : data.favorite.Col(cid)) {
                    // Do not send the notification if the favoriter is the author of the card.
                    // (As the code above has already sent the "new comment on my card" notification.)
                    if (fav.uid != uid && fav.uid != card_author_uid) {
                      data.notification.Add(Notification(
                          fav.uid, now, NotificationNewCommentOnCardIStarred(uid, comment)));
                    }
                  }

                  if (parent_comment_author_uid != UID::INVALID_USER &&
                      parent_comment_author_uid != uid) {
                    // Emit the "new reply to your comment" notification.
                    data.notification.Add(Notification(
                        parent_comment_author_uid, now, NotificationNewReplyToMyComment(comment)));
                  }

                  ResponseAddComment response;
                  response.ms = std::chrono::duration_cast<std::chrono::milliseconds>(now);
                  response.oid = OIDToString(oid);
                  return MakeResponse(std::move(response));
                }
              }, std::move(r)).Go();  // clang-format on
        } catch (const current::TypeSystemParseJSONException&) {
          DebugPrint(Printf("[/ctfo/comments] Could not parse POST body. Requested URL = '%s'",
                            r.url.ComposeURL().c_str()));
          r("NEED VALID BODY\n", HTTPResponseCode.BadRequest);
        }
      } else if (r.method == "DELETE") {
        const OID oid = StringToOID(r.url.query["oid"]);
        if (oid == OID::INVALID_COMMENT) {
          DebugPrint(Printf("[/ctfo/comments] Wrong OID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
          r("NEED VALID OID\n", HTTPResponseCode.BadRequest);
        } else {
          const std::string requested_url = r.url.ComposeURL();
          storage_.ReadWriteTransaction(  // clang-format off
              [this, requested_url, uid, cid, token, oid](MutableFields<Storage> data) {
                bool token_is_valid = false;
                const auto& auth_token_accessor = data.auth_token;
                if (Exists(auth_token_accessor.GetEntryFromCol(token))) {
                  if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
                    // Double check, if the provided `uid` is correct as well.
                    const auto& uid_auth_accessor = data.uid_auth;
                    token_is_valid = Exists(uid_auth_accessor.Get(
                        uid, Value(auth_token_accessor.GetEntryFromCol(token)).auth_key));
                  }
                }
                if (!token_is_valid) {
                  DebugPrint(Printf("[/ctfo/comments] Invalid token. Requested URL = '%s'",
                                     requested_url.c_str()));
                  return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
                } else {
                  DebugPrint(Printf("[/ctfo/comments] Token validated. Requested URL = '%s'",
                                     requested_url.c_str()));
                  // TODO(dkorolev): Do something smart about non-existing comments.
                  auto& comments_mutator = data.comment;
                  std::vector<OID> oids_to_delete;
                  oids_to_delete.push_back(oid);
                  const auto comments_per_card = comments_mutator.Rows()[cid];
                  for (const Comment& c : comments_mutator.Row(cid)) {
                    if (c.parent_oid == oid) {
                      oids_to_delete.push_back(c.oid);
                    }
                  }
                  for (const OID& o : oids_to_delete) {
                    comments_mutator.Erase(cid, o);
                  }
                  ResponseDeleteComment response;
                  response.ms =
                      std::chrono::duration_cast<std::chrono::milliseconds>(current::time::Now());
                  return MakeResponse(std::move(response));
                }
              }, std::move(r)).Go();  // clang-format on
        }
      } else {
        DebugPrint(Printf("[/ctfo/comments] Wrong method '%s'. Requested URL = '%s'",
                          r.method.c_str(),
                          r.url.ComposeURL().c_str()));
        r("METHOD NOT ALLOWED\n", HTTPResponseCode.MethodNotAllowed);
      }
    }
  }

 private:
  struct PushNotificationsSender {
    Storage& storage;
    const std::chrono::microseconds starting_from;
    current::integrations::onesignal::IOSPushNotificationsSender transport;

    using transaction_t = typename Storage::transaction_t;

    PushNotificationsSender(Storage& storage,
                            std::chrono::microseconds last_pushed_notification_timestamp,
                            const std::string& onesignal_app_id,
                            uint16_t onesignal_local_port)
        : storage(storage),
          starting_from(last_pushed_notification_timestamp + std::chrono::microseconds(1)),
          transport(onesignal_app_id, onesignal_local_port) {
      std::cerr << "Starting sending push notifications from " << starting_from.count() << " epoch us.\n";
    }

    current::ss::EntryResponse operator()(const transaction_t& entry, idxts_t current, idxts_t) const {
      if (current.us >= starting_from) {
        for (const auto& mutation : entry.mutations) {
          if (Exists<Persisted_NotificationUpdated>(mutation)) {
            const UID uid = Value<Persisted_NotificationUpdated>(mutation).data.uid;
            const std::string player_id =
                Value(storage.ReadOnlyTransaction([uid](ImmutableFields<Storage> fields) -> std::string {
                  const auto rhs = fields.uid_player_id[uid];
                  if (Exists(rhs)) {
                    return Value(rhs).player_id;
                  } else {
                    return "";
                  }
                }).Go());
            if (!player_id.empty()) {
              // Hack: Update `push_starting_from` on success of one push, leave intact on failure.
              if (transport.Push(player_id, 1)) {
                const std::chrono::microseconds last_pushed_notification_timestamp = current.us;
                storage.ReadWriteTransaction(
                            [last_pushed_notification_timestamp](MutableFields<Storage> fields) {
                              PushNotificationsMarker marker;
                              marker.last_pushed_notification_timestamp = last_pushed_notification_timestamp;
                              fields.push_notifications_marker.Add(marker);
                            }).Go();
              }
            }
          }
        }
      }
      return current::ss::EntryResponse::More;
    }

    current::ss::TerminationResponse Terminate() const { return current::ss::TerminationResponse::Terminate; }

    current::ss::EntryResponse EntryResponseIfNoMorePassTypeFilter() const {
      return current::ss::EntryResponse::Done;
    }
  };

 private:
  Config config_;
  std::ofstream midichlorians_stream_;
  MidichloriansServer midichlorians_server_;

  Storage storage_;
  RESTStorage rest_;
  current::ss::StreamSubscriber<PushNotificationsSender, typename Storage::transaction_t> pusher_;
  const current::sherlock::SubscriberScope push_notifications_sender_subcriber_scope_;
  HTTPRoutesScope scoped_http_routes_;

  const std::map<std::string, LOG_EVENT> valid_responses_ = {
      {"CTFO", LOG_EVENT::CTFO},
      {"TFU", LOG_EVENT::TFU},
      {"SKIP", LOG_EVENT::SKIP},
      {"FAV", LOG_EVENT::FAV_CARD},
      {"FAV_CARD", LOG_EVENT::FAV_CARD},
      {"UNFAV", LOG_EVENT::UNFAV_CARD},
      {"UNFAV_CARD", LOG_EVENT::UNFAV_CARD},
      {"LIKE_COMMENT", LOG_EVENT::LIKE_COMMENT},
      {"UNLIKE_COMMENT", LOG_EVENT::UNLIKE_COMMENT},
      {"FLAG_COMMENT", LOG_EVENT::FLAG_COMMENT},
      {"FLAG_CARD", LOG_EVENT::FLAG_CARD},
      {"REPORT_USER", LOG_EVENT::REPORT_USER},
      {"BLOCK_USER", LOG_EVENT::BLOCK_USER},
      {"ONE_SIGNAL_USER_ID", LOG_EVENT::ONE_SIGNAL_USER_ID}};

  void DebugPrint(const std::string& message) {
    if (config_.Config().debug_print_to_stderr) {
      std::cerr << message << std::endl;
    }
  }

  void CopyUserInfoToResponseEntry(const User& user, bool banned, ResponseUserEntry& entry) {
    entry.uid = UIDToString(user.uid);
    entry.score = user.score;
    entry.level = user.level;
    if (user.level < LEVEL_SCORES.size() - 1) {
      entry.next_level_score = LEVEL_SCORES[user.level + 1];  // LEVEL_SCORES = { 0u, ... }
    } else {
      entry.next_level_score = 0u;
    }
    entry.banned = banned;
  }

  ResponseFeed GenerateResponseFeed(ImmutableFields<Storage> data,
                                    ResponseUserEntry user_entry,
                                    size_t feed_size,
                                    uint64_t notifications_since_ms,
                                    size_t FEED_SIZE_LIMIT = 300ul,
                                    bool initial = false) {
    ResponseFeed response;
    const size_t max_count = std::min(feed_size, FEED_SIZE_LIMIT);
    response.user = user_entry;

    const auto& cards = data.card;
    const auto& answers = data.answer;
    const auto& flagged_cards = data.flagged_card;
    const auto& favorites = data.favorite;

    std::set<std::pair<double, CID>> hot_cards;
    std::set<std::pair<double, CID>> recent_cards;
    const UID uid = StringToUID(response.user.uid);

    const uint64_t now = current::time::Now().count();
    for (const auto& card : cards) {
      if (initial && card.ctfo_count + card.tfu_count + card.skip_count < 5) continue;
      if (!Exists(answers.Get(uid, card.cid)) && !Exists(flagged_cards.Get(card.cid, uid))) {
        const UID card_author_uid = [&data](CID cid) {
          const auto& author_cards = data.author_card;
          const auto v = author_cards.GetEntryFromCol(cid);
          return Exists(v) ? Value(v).uid : UID::INVALID_USER;
        }(card.cid);

        if (card_author_uid == UID::INVALID_USER ||
            (!Exists(data.banned_user[card_author_uid]) &&
             !Exists(data.user_blocked_user.Get(uid, card_author_uid)))) {
          // For the recent feed, relevance is the function of the age of the card.
          // Added just now => 1.00. Added 24 hour ago => 0.99. Added 48 hours ago => 0.99^2. Etc.
          double time_order_key =
              0.9 * std::pow(0.99, (now - card.us.count()) * (1.0 / (1000ll * 1000 * 60 * 60 * 24)));
          double hot_order_key = RandomDouble(0.2, 0.4);
          if (initial) {
            hot_order_key = 1.0 / (1.0 + exp(0.01 * (-1.0 * card.ctfo_count + +1.0 * card.tfu_count)));
          }
          if (card.startup_index) {
            time_order_key = 1.0 - 1e-6 * card.startup_index;
            hot_order_key = 1.0 - 1e-6 * card.startup_index;
          }
          hot_cards.emplace(hot_order_key, card.cid);
          recent_cards.emplace(time_order_key, card.cid);
          if (hot_cards.size() > max_count) {
            hot_cards.erase(hot_cards.begin());
          }
          if (recent_cards.size() > max_count) {
            recent_cards.erase(recent_cards.begin());
          }
        }
      }
    }

    const auto& author_cards = data.author_card;
    const auto& comments = data.comment;
    const auto GenerateCardForFeed =
        [this, uid, &answers, &favorites, &comments, &author_cards](const Card& card) {
          ResponseCardEntry card_entry;
          card_entry.cid = CIDToString(card.cid);
          const auto v = author_cards.GetEntryFromCol(card.cid);
          if (Exists(v)) {
            const UID author_uid = Value(v).uid;
            card_entry.author_uid = UIDToString(author_uid);
            card_entry.is_my_card = (uid == author_uid);
          }
          card_entry.number_of_comments = comments.Row(card.cid).Size();
          card_entry.text = card.text;
          card_entry.ms = std::chrono::duration_cast<std::chrono::milliseconds>(card.us);
          card_entry.color = card.color;
          card_entry.relevance = 0.0;  // Will be overridden later.
          card_entry.ctfo_score = 50u;
          card_entry.tfu_score = 50u;
          card_entry.ctfo_count = card.ctfo_count;
          card_entry.tfu_count = card.tfu_count;
          card_entry.skip_count = card.skip_count;

          card_entry.vote = "";
          const auto answer = answers.Get(uid, card.cid);
          if (Exists(answer)) {
            const ANSWER vote = Value(answer).answer;
            if (vote == ANSWER::CTFO) {
              card_entry.vote = "CTFO";
            } else if (vote == ANSWER::TFU) {
              card_entry.vote = "TFU";
            } else if (vote == ANSWER::SKIP) {
              card_entry.vote = "SKIP";
            }
          }

          card_entry.favorited = false;
          const auto fav = favorites.Get(uid, card.cid);
          if (Exists(fav)) {
            card_entry.favorited = Value(fav).favorited;
          }
          return card_entry;
        };

    // Copy the cards back, top scorers first.
    std::vector<std::reference_wrapper<std::set<std::pair<double, CID>>>> prepared_cards;
    prepared_cards.push_back(hot_cards);
    prepared_cards.push_back(recent_cards);

    std::vector<std::reference_wrapper<std::vector<ResponseCardEntry>>> feeds;
    feeds.push_back(response.feed_hot);
    feeds.push_back(response.feed_recent);

    for (size_t i = 0; i < 2; ++i) {
      for (auto cit = prepared_cards[i].get().rbegin(); cit != prepared_cards[i].get().rend(); ++cit) {
        const auto card = data.card[cit->second];
        if (Exists(card)) {
          feeds[i].get().push_back(GenerateCardForFeed(Value(card)));
          feeds[i].get().back().relevance = cit->first;
        }
      }
    }

    if (notifications_since_ms != static_cast<uint64_t>(-1)) {
      // Shamelessly copy-pasted from GenerateCardForFeed. -- D.K.
      const auto GenerateCardForNotification =
          [this, uid, &answers, &favorites, &comments, &author_cards](const Card& card) {
            ResponseCardEntry card_entry;
            card_entry.cid = CIDToString(card.cid);
            const auto v = author_cards.GetEntryFromCol(card.cid);
            if (Exists(v)) {
              const UID author_uid = Value(v).uid;
              card_entry.author_uid = UIDToString(author_uid);
              card_entry.is_my_card = (uid == author_uid);
            }
            card_entry.number_of_comments = comments.Row(card.cid).Size();
            card_entry.text = card.text;
            card_entry.ms = std::chrono::duration_cast<std::chrono::milliseconds>(card.us);
            card_entry.color = card.color;
            card_entry.relevance = 0.0;  // Will be overridden later.
            card_entry.ctfo_score = 50u;
            card_entry.tfu_score = 50u;
            card_entry.ctfo_count = card.ctfo_count;
            card_entry.tfu_count = card.tfu_count;
            card_entry.skip_count = card.skip_count;

            card_entry.vote = "";
            const auto answer = answers.Get(uid, card.cid);
            if (Exists(answer)) {
              const ANSWER vote = Value(answer).answer;
              if (vote == ANSWER::CTFO) {
                card_entry.vote = "CTFO";
              } else if (vote == ANSWER::TFU) {
                card_entry.vote = "TFU";
              } else if (vote == ANSWER::SKIP) {
                card_entry.vote = "SKIP";
              }
            }

            card_entry.favorited = false;
            const auto fav = favorites.Get(uid, card.cid);
            if (Exists(fav)) {
              card_entry.favorited = Value(fav).favorited;
            }
            return card_entry;
          };
      // TODO(dkorolev): `upper_bound`, not go through all notifications for this user.
      std::map<CID, ResponseCardEntry> cards_used;
      // The app expects only notifications "after", not "since".
      const uint64_t notifications_since_us = (notifications_since_ms + 1) * 1000;
      for (const Notification& notification : data.notification.Row(uid)) {
        if (static_cast<uint64_t>(notification.timestamp.count()) > notifications_since_us) {
          const CID cid = notification.GetCID();
          if (cid != CID::INVALID_CARD && !cards_used.count(cid)) {
            const auto card = data.card[cid];
            if (Exists(card)) {
              cards_used[cid] = GenerateCardForNotification(Value(card));
            }
          }
          // Explicitly hold off on sending notifications for already non-existent cards.
          if (cid == CID::INVALID_CARD || cards_used.count(cid)) {
            response.notifications.push_back(notification.BuildResponseNotification());
            if (cid != CID::INVALID_CARD) {
              response.notifications.back().card = cards_used[cid];
            }
            // TODO(dkorolev): Command line flag or parameter instead of bare `50`.
            if (response.notifications.size() >= 50u) {
              break;
            }
          }
        }
      }
      std::reverse(response.notifications.begin(), response.notifications.end());
    }

    response.ms = std::chrono::duration_cast<std::chrono::milliseconds>(current::time::Now());
    DebugPrint(Printf("[RespondWithFeed] Generated response for UID '%s' with %u 'hot' and %u 'recent' cards",
                      response.user.uid.c_str(),
                      response.feed_hot.size(),
                      response.feed_recent.size()));
    return response;
  }

  void UpdateStateOnEvent(const current::midichlorians::ios::iOSGenericEvent& ge) {
    try {
      if (!valid_responses_.count(ge.event)) {
        DebugPrint(Printf("[UpdateStateOnEvent] Unhandled event: '%s'", ge.event.c_str()));
        return;
      }
      const LOG_EVENT response = valid_responses_.at(ge.event);
      const std::string uid_str = ge.fields.at("uid");
      const std::string token = ge.fields.at("token");
      const std::string whom_str = ge.fields.count("whom") ? ge.fields.at("whom") : "";
      const std::string cid_str = ge.fields.count("cid") ? ge.fields.at("cid") : "";
      const std::string oid_str = ge.fields.count("oid") ? ge.fields.at("oid") : "";
      const std::string user_id_str = ge.fields.count("user_id") ? ge.fields.at("user_id") : "";
      const UID uid = StringToUID(uid_str);
      const UID whom = StringToUID(whom_str);
      const CID cid = StringToCID(cid_str);
      const OID oid = StringToOID(oid_str);
      DebugPrint(
          Printf("[UpdateStateOnEvent] Event='%s', uid='%s', cid='%s', oid='%s', token='%s', user_id='%s'",
                 ge.event.c_str(),
                 uid_str.c_str(),
                 cid_str.c_str(),
                 oid_str.c_str(),
                 token.c_str(),
                 user_id_str.c_str()));
      if (uid != UID::INVALID_USER) {  // clang-format off
        storage_.ReadWriteTransaction(
            [this, uid, whom, cid, oid, uid_str, whom_str, cid_str, oid_str, token, user_id_str, response](
                MutableFields<Storage> data) {
              const auto& auth_token_accessor = data.auth_token;
              bool token_is_valid = false;
              if (Exists(auth_token_accessor.GetEntryFromCol(token))) {
                if (Value(auth_token_accessor.GetEntryFromCol(token)).valid) {
                  token_is_valid = true;
                }
              }
              if (token_is_valid) {
                if (!Exists(data.user[uid])) {
                  DebugPrint(Printf("[UpdateStateOnEvent] Nonexistent UID '%s'.", uid_str.c_str()));
                  return;
                }
                if (response == LOG_EVENT::SKIP || response == LOG_EVENT::CTFO ||
                    response == LOG_EVENT::TFU) {
                  if (cid == CID::INVALID_CARD) {
                    DebugPrint("[UpdateStateOnEvent] No CID.");
                    return;
                  }
                  if (!Exists(data.card[cid])) {
                    DebugPrint(Printf("[UpdateStateOnEvent] Nonexistent CID '%s' in SKIP/CTFO/TFU.",
                                      cid_str.c_str()));
                    return;
                  }
                  // Do not overwrite existing answers, except CTFO/TFU can and should overwrite SKIP.
                  const bool has_answer = Exists(data.answer.Get(uid, cid));
                  const bool skip_to_overwrite = response != LOG_EVENT::SKIP && has_answer &&
                                                  Value(data.answer.Get(uid, cid)).answer == ANSWER::SKIP;
                  if (!has_answer || skip_to_overwrite) {
                    data.answer.Add(Answer(uid, cid, static_cast<ANSWER>(response)));
                    DebugPrint(Printf("[UpdateStateOnEvent] Added new answer: [%s, %s, %d]",
                                      UIDToString(uid).c_str(),
                                      CIDToString(cid).c_str(),
                                      static_cast<int>(response)));
                    const auto optional_card = data.card[cid];
                    const auto optional_user = data.user[uid];
                    if (Exists(optional_card) && Exists(optional_user)) {
                      Card card = Value(optional_card);
                      User user = Value(optional_user);
                      if (response == LOG_EVENT::SKIP) {
                        ++card.skip_count;
                      } else {
                        if (skip_to_overwrite) {
                          if (card.skip_count) {
                            // Extra safety just in case it's zero.
                            --card.skip_count;
                          }
                        }
                        if (response == LOG_EVENT::CTFO) {
                          ++card.ctfo_count;
                          DebugPrint(Printf("[UpdateStateOnEvent] Card '%s' new ctfo_count = %u",
                                            CIDToString(cid).c_str(),
                                            card.ctfo_count));
                          user.score += 50u;
                          DebugPrint(
                              Printf("[UpdateStateOnEvent] User '%s' got %u points for 'CTFO' answer",
                                     UIDToString(uid).c_str(),
                                     50u));
                        } else if (response == LOG_EVENT::TFU) {
                          ++card.tfu_count;
                          DebugPrint(Printf("[UpdateStateOnEvent] Card '%s' new tfu_count = %u",
                                            CIDToString(cid).c_str(),
                                            card.tfu_count));
                          user.score += 50u;
                          DebugPrint(
                              Printf("[UpdateStateOnEvent] User '%s' got %u points for 'TFU' answer",
                                     UIDToString(uid).c_str(),
                                     50u));
                        }
                      }

                      if (response != LOG_EVENT::SKIP && user.level < LEVEL_SCORES.size() - 1 &&
                          user.score > LEVEL_SCORES[user.level + 1]) {
                        user.score -= LEVEL_SCORES[user.level + 1];
                        ++user.level;
                        DebugPrint(
                            Printf("[UpdateStateOnEvent] User '%s' got promoted to a new level = %u",
                                  UIDToString(uid).c_str(),
                                  user.level));
                      }
                      data.card.Add(card);
                      data.user.Add(user);

                      if (response != LOG_EVENT::SKIP) {
                        // Emit the "new votes on my card" notification.
                        const auto author = data.author_card.GetEntryFromCol(cid);
                        if (Exists(author)) {
                          const UID author_uid = Value(author).uid;
                          if (author_uid != UID::INVALID_USER && author_uid != uid) {
                            data.notification.Add(Notification(author_uid,
                                                               current::time::Now(),
                                                               NotificationNewVotesOnMyCard(uid, cid)));
                          }
                        }
                      }
                    }
                  } else {
                    DebugPrint(Printf("[UpdateStateOnEvent] Answer already exists: [%s, %s, %d]",
                                      UIDToString(uid).c_str(),
                                      CIDToString(cid).c_str(),
                                      static_cast<int>(Value(data.answer.Get(uid, cid)).answer)));
                  }
                } else if (response == LOG_EVENT::FAV_CARD || response == LOG_EVENT::UNFAV_CARD) {
                  if (cid == CID::INVALID_CARD) {
                    DebugPrint("[UpdateStateOnEvent] No CID.");
                    return;
                  }
                  if (!Exists(data.card[cid])) {
                    DebugPrint(
                        Printf("[UpdateStateOnEvent] Nonexistent CID '%s' FAV/UNFAV.", cid_str.c_str()));
                    return;
                  }
                  auto& favorites_mutator = data.favorite;
                  favorites_mutator.Add(Favorite(uid, cid, (response == LOG_EVENT::FAV_CARD)));
                  DebugPrint(Printf("[UpdateStateOnEvent] Added favorite: [%s, %s, %s]",
                                    UIDToString(uid).c_str(),
                                    CIDToString(cid).c_str(),
                                    (response == LOG_EVENT::FAV_CARD) ? "Favorite" : "Unfavorite"));

                  if (response == LOG_EVENT::FAV_CARD) {
                    // Emit the "my card starred" notification.
                    const auto star_notification_key = UIDAndCID(uid, cid);
                    if (!Exists(data.starred_notification_already_sent[star_notification_key])) {
                      // Only send the notification about the behavior of this { user, card } once.
                      data.starred_notification_already_sent.Add(
                          StarNotificationAlreadySent{star_notification_key});
                      const auto author = data.author_card.GetEntryFromCol(cid);
                      if (Exists(author)) {
                        const UID author_uid = Value(author).uid;
                        if (author_uid != UID::INVALID_USER && author_uid != uid) {
                          data.notification.Add(Notification(
                              author_uid, current::time::Now(), NotificationMyCardStarred(uid, cid)));
                        }
                      }
                    }
                  }
                } else if (response == LOG_EVENT::LIKE_COMMENT ||
                           response == LOG_EVENT::UNLIKE_COMMENT ||
                           response == LOG_EVENT::FLAG_COMMENT) {
                  if (oid == OID::INVALID_COMMENT) {
                    DebugPrint("[UpdateStateOnEvent] No OID.");
                    return;
                  }
                  if (!data.comment.Cols().Has(oid)) {
                    DebugPrint(Printf("[UpdateStateOnEvent] Nonexistent OID '%s' LIKE/UNLIKE/FLAG.",
                                      oid_str.c_str()));
                    return;
                  }
                  if (response == LOG_EVENT::LIKE_COMMENT) {
                    DebugPrint(Printf("[UpdateStateOnEvent] Like comment '%s' '%s'.",
                                      uid_str.c_str(),
                                      oid_str.c_str()));
                    CommentLike like;
                    like.oid = oid;
                    like.uid = uid;
                    data.comment_like.Add(like);

                    // Emit the "my comment liked" notification.
                    const auto v = data.comment.GetEntryFromCol(oid);
                    if (Exists(v)) {
                      const Comment& comment = Value(v);
                      UID comment_author_uid = comment.author_uid;
                      if (comment_author_uid != UID::INVALID_USER && comment_author_uid != like.uid) {
                        data.notification.Add(
                            Notification(comment_author_uid,
                                         current::time::Now(),
                                         NotificationMyCommentLiked(like.uid, comment)));
                      }
                    }

                  } else if (response == LOG_EVENT::UNLIKE_COMMENT) {
                    DebugPrint(Printf("[UpdateStateOnEvent] Unlike comment '%s' '%s'.",
                                      uid_str.c_str(),
                                      oid_str.c_str()));
                    data.comment_like.Erase(oid, uid);
                  } else if (response == LOG_EVENT::FLAG_COMMENT) {
                    DebugPrint(Printf("[UpdateStateOnEvent] Flag comment '%s' '%s'.",
                                      uid_str.c_str(),
                                      oid_str.c_str()));
                    CommentFlagAsInappropriate flag;
                    flag.oid = oid;
                    flag.uid = uid;
                    data.flagged_comment.Add(flag);
                  } else {
                    DebugPrint(Printf(
                        "[UpdateStateOnEvent] Ignoring: Response=<%d>, uid='%s', cid='%s',token='%s'",
                        static_cast<int>(response),
                        uid_str.c_str(),
                        cid_str.c_str(),
                        token.c_str()));
                  }
                } else if (response == LOG_EVENT::FLAG_CARD) {
                  if (cid == CID::INVALID_CARD) {
                    DebugPrint("[UpdateStateOnEvent] No CID.");
                    return;
                  }
                  if (!Exists(data.card[cid])) {
                    DebugPrint(Printf("[UpdateStateOnEvent] Nonexistent CID '%s' in FLAG_CARD.",
                                      cid_str.c_str()));
                    return;
                  }
                  DebugPrint(Printf(
                      "[UpdateStateOnEvent] Flag card '%s' '%s'.", uid_str.c_str(), cid_str.c_str()));
                  CardFlagAsInappropriate flag;
                  flag.cid = cid;
                  flag.uid = uid;
                  data.flagged_card.Add(flag);
                } else if (response == LOG_EVENT::REPORT_USER || response == LOG_EVENT::BLOCK_USER) {
                  if (whom == UID::INVALID_USER) {
                    DebugPrint("[UpdateStateOnEvent] No WHOM.");
                    return;
                  }
                  if (whom == uid) {
                    DebugPrint("[UpdateStateOnEvent] WHOM == UID.");
                    return;
                  }
                  if (whom == admin_uid) {
                    DebugPrint("[UpdateStateOnEvent] WHOM == ADMIN_UID.");
                    return;
                  }
                  DebugPrint(Printf("[UpdateStateOnEvent] '%s' %s '%s'.",
                                    uid_str.c_str(),
                                    (response == LOG_EVENT::REPORT_USER) ? "reported" : "blocked",
                                    whom_str.c_str()));
                  if (response == LOG_EVENT::REPORT_USER) {
                    data.user_reported_user.Add(UserReportedUser(uid, whom));
                  } else {
                    data.user_blocked_user.Add(UserBlockedUser(uid, whom));
                  }
                  {
                    const auto blocks = data.user_blocked_user.Cols()[whom];
                    const auto reports = data.user_reported_user.Cols()[whom];
                    if ((Exists(blocks) && Value(blocks).Size() >= FLAGS_blocks_to_ban) ||
                        (Exists(reports) && Value(reports).Size() >= FLAGS_reports_to_ban)) {
                      BanUser(data, whom);
                    }
                  }
                } else if (response == LOG_EVENT::ONE_SIGNAL_USER_ID) {
                  if (!user_id_str.empty()) {
                    UserNotificationPlayerID player_id_record;
                    player_id_record.uid = uid;
                    player_id_record.player_id = user_id_str;
                    data.uid_player_id.Add(player_id_record);
                  } else {
                    DebugPrint("[UpdateStateOnEvent] Ignoring `ONE_SIGNAL_USER_ID`, no `used_id` provided.\n");
                  }
                } else {
                  DebugPrint(Printf(
                      "[UpdateStateOnEvent] Ignoring: Response=<%d>, uid='%s', cid='%s',token='%s'",
                      static_cast<int>(response),
                      uid_str.c_str(),
                      cid_str.c_str(),
                      token.c_str()));
                }
              } else {
                DebugPrint(
                    Printf("[UpdateStateOnEvent] Not valid token '%s' found in event.", token.c_str()));
              }
            }).Go();  // clang-format on
      } else {
        DebugPrint("[UpdateStateOnEvent] Invalid UID.");
      }
    } catch (const std::out_of_range& e) {
      DebugPrint(Printf("[UpdateStateOnEvent] std::out_of_range: '%s'", e.what()));
    }
  }

  void BanUser(MutableFields<Storage> data, const UID uid) {
    if (!Exists(data.banned_user[uid])) {
      data.banned_user.Add(BannedUser(uid));
    }
  }
};

}  // namespace CTFO

#endif  // CTFO_SERVER_H
