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

#include "../Current/Blocks/HTTP/api.h"

#include "../Current/Bricks/file/file.h"
#include "../Current/Bricks/strings/strings.h"
#include "../Current/Bricks/util/random.h"

#include "../Current/Blocks/HTTP/api.h"

#include "../Current/EventCollector/event_collector.h"

#include "schema.h"
#include "util.h"

// Structured iOS events.
#include "../Current/Midichlorians/Dev/Beta/MidichloriansDataDictionary.h"

using namespace yoda;
using namespace bricks::random;
using namespace bricks::strings;

class CTFOServer final {
 public:
  explicit CTFOServer(const std::string& cards_file,
                      int port,
                      const std::string& storage_file,
                      int event_log_port,
                      const std::string& event_log_file,
                      const bricks::time::MILLISECONDS_INTERVAL tick_interval_ms,
                      const bool debug_print_to_stderr = false)
      : port_(port),
        event_log_file_(event_log_file),
        event_collector_(event_log_port ? event_log_port : port,
                         event_log_stream_,
                         tick_interval_ms,
                         "/ctfo/log",
                         "OK\n",
                         std::bind(&CTFOServer::OnMidichloriansEvent, this, std::placeholders::_1)),
        debug_print_(debug_print_to_stderr),
        storage_("CTFO storage", storage_file) {
    event_log_stream_.open(event_log_file_, std::ofstream::out | std::ofstream::app);

    bricks::cerealize::CerealFileParser<Card, bricks::cerealize::CerealFormat::JSON> cf(cards_file);
    storage_.Transaction([&cf](StorageAPI::T_DATA data) {
                           while (cf.Next([&data](const Card& card) { data.Add(card); })) {
                             ;
                           }
                         }).Go();

    HTTP(port_).Register("/ctfo/auth/ios", BindToThis(&CTFOServer::RouteAuth));
    HTTP(port_).Register("/ctfo/feed", BindToThis(&CTFOServer::RouteFeed));
    HTTP(port_).Register("/ctfo/favs", BindToThis(&CTFOServer::RouteFavorites));
    HTTP(port_).Register("/ctfo/my_cards", BindToThis(&CTFOServer::RouteMyCards));
    HTTP(port_).Register("/ctfo/card", BindToThis(&CTFOServer::RouteCard));
    HTTP(port_).Register("/ctfo/comments", BindToThis(&CTFOServer::RouteComments));
    HTTP(port_).Register("/ctfo/comment", BindToThis(&CTFOServer::RouteComments));
  }

  ~CTFOServer() {
    // TODO(dkorolev): Scoped registerers FTW.
    HTTP(port_).UnRegister("/ctfo/auth/ios");
    HTTP(port_).UnRegister("/ctfo/feed");
    HTTP(port_).UnRegister("/ctfo/favs");
    HTTP(port_).UnRegister("/ctfo/my_cards");
    HTTP(port_).UnRegister("/ctfo/card");
    HTTP(port_).UnRegister("/ctfo/comments");
    HTTP(port_).UnRegister("/ctfo/comment");
  }

  void Join() { HTTP(port_).Join(); }

 private:
  typedef void (CTFOServer::*CTFOServerMemberFunctionServingRequest)(Request);

  std::function<void(Request)> BindToThis(CTFOServerMemberFunctionServingRequest handler) {
    return std::bind(handler, this, std::placeholders::_1);
  };

  void RouteAuth(Request r) {
    if (r.method != "POST") {
      DebugPrint(Printf("[/ctfo/auth/ios] Wrong method '%s'. Requested URL = '%s'",
                        r.method.c_str(),
                        r.url.ComposeURL().c_str()));
      r("METHOD NOT ALLOWED\n", HTTPResponseCode.MethodNotAllowed);
    } else {
      const std::string device_id = r.url.query.get("id", "");
      const std::string app_key = r.url.query.get("key", "");
      if (device_id.empty() || app_key.empty()) {
        DebugPrint(Printf("[/ctfo/auth/ios] Wrong query parameters. Requested URL = '%s'",
                          r.url.ComposeURL().c_str()));
        r("NEED VALID ID-KEY PAIR\n", HTTPResponseCode.BadRequest);
      } else {
        const size_t feed_count = FromString<size_t>(r.url.query.get("feed_count", "20"));
        // Searching for users with the corresponding authentication key.
        storage_.Transaction(
            [this, device_id, app_key, feed_count](StorageAPI::T_DATA data) {
              AuthKey auth_key("iOS::" + device_id + "::" + app_key, AUTH_TYPE::IOS);
              UID uid = UID::INVALID_USER;
              User user;
              ResponseUserEntry user_entry;
              std::string token;

              const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
              if (auth_uid_accessor.Rows().Has(auth_key)) {
                // Something went terribly wrong
                // if we have more than one UID for authentication key.
                assert(auth_uid_accessor[auth_key].size() == 1);
                uid = auth_uid_accessor[auth_key].begin()->uid;
              }

              auto auth_token_mutator = Matrix<AuthKeyTokenPair>::Mutator(data);
              if (uid != UID::INVALID_USER) {
                // User exists => invalidate all tokens.
                for (const auto& auth_token : auth_token_mutator[auth_key]) {
                  auth_token_mutator.Add(AuthKeyTokenPair(auth_key, auth_token.token, false));
                }
                user = data.Get(uid);
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
                data.Add(user);
                data.Add(AuthKeyUIDPair(auth_key, user.uid));
              }
              DebugPrint(Printf("[/ctfo/auth/ios] New user: UID='%s', DeviceID='%s', AppKey='%s', Token='%s'",
                                UIDToString(uid).c_str(),
                                device_id.c_str(),
                                app_key.c_str(),
                                token.c_str()));

              CopyUserInfoToResponseEntry(user, user_entry);
              user_entry.token = token;

              ResponseFeed rfeed = GenerateResponseFeed(data, user_entry, feed_count);
              return Response(rfeed, "feed");
            },
            std::move(r));
      }
    }
  }
  void RouteFeed(Request r) {
    const UID uid = StringToUID(r.url.query["uid"]);
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
        const size_t feed_count = FromString<size_t>(r.url.query.get("feed_count", "20"));
        const std::string requested_url = r.url.ComposeURL();
        storage_.Transaction(
            [this, uid, token, requested_url, feed_count](StorageAPI::T_DATA data) {
              bool token_is_valid = false;
              const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
              if (auth_token_accessor.Cols().Has(token)) {
                // Something went terribly wrong
                // if we have more than one authentication key for token.
                assert(auth_token_accessor[token].size() == 1);
                if (auth_token_accessor[token].begin()->valid) {
                  // Double check, if the provided `uid` is correct as well.
                  const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
                  token_is_valid = auth_uid_accessor.Has(auth_token_accessor[token].begin().key(), uid);
                }
              }
              if (!token_is_valid) {
                DebugPrint(Printf("[/ctfo/feed] Invalid token. Requested URL = '%s'", requested_url.c_str()));
                return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
              } else {
                DebugPrint(Printf("[/ctfo/feed] Token validated. Requested URL = '%s'", requested_url.c_str()));
                const auto user = data.Get(uid);
                ResponseUserEntry user_entry;
                CopyUserInfoToResponseEntry(user, user_entry);
                user_entry.token = token;
                ResponseFeed rfeed = GenerateResponseFeed(data, user_entry, feed_count);
                return Response(rfeed, "feed");
              }
            },
            std::move(r));
      }
    }
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
        storage_.Transaction([this, uid, token](StorageAPI::T_DATA data) {
                               bool token_is_valid = false;
                               const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
                               if (auth_token_accessor.Cols().Has(token)) {
                                 // Something went terribly wrong
                                 // if we have more than one authentication key for token.
                                 assert(auth_token_accessor[token].size() == 1);
                                 if (auth_token_accessor[token].begin()->valid) {
                                   // Double check, if the provided `uid` is correct as well.
                                   const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
                                   token_is_valid =
                                       auth_uid_accessor.Has(auth_token_accessor[token].begin().key(), uid);
                                 }
                               }
                               if (!token_is_valid) {
                                 DebugPrint("[/ctfo/favs] Invalid token.");
                                 return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
                               } else {
                                 DebugPrint("[/ctfo/favs] Token validated.");
                                 const auto user = data.Get(uid);
                                 if (!user) {
                                   return Response("NEED VALID USER\n", HTTPResponseCode.Unauthorized);
                                 } else {
                                   ResponseFavs rfavs;
                                   CopyUserInfoToResponseEntry(user, rfavs.user);

                                   const auto answers = Matrix<Answer>::Accessor(data);

                                   // Get favs.
                                   std::vector<std::pair<uint64_t, CID>> favs;
                                   const auto favorites = Matrix<Favorite>::Accessor(data);
                                   try {
                                     for (const auto& fav : favorites[uid]) {
                                       if (fav.favorited) {
                                         favs.emplace_back(fav.ms, fav.cid);
                                       }
                                     }
                                   } catch (const yoda::SubscriptException<Favorite>&) {
                                     // No favorites for this user.
                                   }

                                   // In reverse chronological order.
                                   std::sort(favs.rbegin(), favs.rend());

                                   // And publish them.
                                   const auto card_authors = Matrix<CardAuthor>::Accessor(data);
                                   const auto comments = Matrix<Comment>::Accessor(data);
                                   const auto GenerateCardForFavs =
                                       [this, uid, &answers, &card_authors, &comments](const Card& card) {
                                     ResponseCardEntry card_entry;
                                     card_entry.cid = CIDToString(card.cid);
                                     try {
                                       const auto& iterable = card_authors[card.cid];
                                       if (iterable.size() == 1u) {
                                         const UID author_uid = (*iterable.begin()).uid;
                                         card_entry.author_uid = UIDToString(author_uid);
                                         card_entry.is_my_card = (uid == author_uid);
                                       }
                                     } catch (const yoda::SubscriptException<CardAuthor>&) {
                                     }
                                     try {
                                       card_entry.number_of_comments = comments[card.cid].size();
                                     } catch (const yoda::SubscriptException<Comment>&) {
                                       // TODO(dkorolev): MatrixSubscriptException<C, X> into Yoda?
                                     }
                                     card_entry.text = card.text;
                                     card_entry.ms = card.ms;
                                     card_entry.color = card.color;
                                     card_entry.relevance = RandomDouble(0, 1);
                                     card_entry.ctfo_score = 50u;
                                     card_entry.tfu_score = 50u;
                                     card_entry.ctfo_count = card.ctfo_count;
                                     card_entry.tfu_count = card.tfu_count;
                                     card_entry.skip_count = card.skip_count;

                                     const EntryWrapper<Answer> answer = answers.Get(uid, card.cid);
                                     if (answer) {
                                       const ANSWER vote = static_cast<Answer>(answer).answer;
                                       if (vote == ANSWER::CTFO) {
                                         card_entry.vote = "CTFO";
                                       } else if (vote == ANSWER::TFU) {
                                         card_entry.vote = "TFU";
                                       }
                                     }

                                     card_entry.favorited = true;

                                     return card_entry;
                                   };

                                   for (const auto& c : favs) {
                                     rfavs.cards.push_back(GenerateCardForFavs(data.Get(c.second)));
                                   }

                                   rfavs.ms = static_cast<uint64_t>(bricks::time::Now());
                                   return Response(rfavs, "favs");
                                 }
                               }
                             },
                             std::move(r));
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
        storage_.Transaction(
            [this, uid, token](StorageAPI::T_DATA data) {
              bool token_is_valid = false;
              const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
              if (auth_token_accessor.Cols().Has(token)) {
                // Something went terribly wrong
                // if we have more than one authentication key for token.
                assert(auth_token_accessor[token].size() == 1);
                if (auth_token_accessor[token].begin()->valid) {
                  // Double check, if the provided `uid` is correct as well.
                  const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
                  token_is_valid = auth_uid_accessor.Has(auth_token_accessor[token].begin().key(), uid);
                }
              }
              if (!token_is_valid) {
                DebugPrint("[/ctfo/my_cards] Invalid token.");
                return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
              } else {
                DebugPrint("[/ctfo/my_cards] Token validated.");
                const auto user = data.Get(uid);
                if (!user) {
                  return Response("NEED VALID USER\n", HTTPResponseCode.Unauthorized);
                } else {
                  ResponseMyCards r_my_cards;
                  CopyUserInfoToResponseEntry(user, r_my_cards.user);

                  const auto answers = Matrix<Answer>::Accessor(data);
                  const auto favorites = Matrix<Favorite>::Accessor(data);

                  // Get my cards.
                  std::vector<std::pair<uint64_t, CID>> my_cards;
                  const auto cards_by_author = Matrix<CardAuthor>::Accessor(data);
                  try {
                    for (const auto& my_card : cards_by_author[uid]) {
                      my_cards.emplace_back(my_card.ms, my_card.cid);
                    }
                  } catch (const yoda::SubscriptException<CardAuthor>&) {
                    // No cards for this user.
                  }

                  // In reverse chronological order.
                  std::sort(my_cards.rbegin(), my_cards.rend());

                  // And publish them.
                  const auto card_authors = Matrix<CardAuthor>::Accessor(data);
                  const auto comments = Matrix<Comment>::Accessor(data);
                  const auto GenerateCardForMyCards =
                      [this, uid, &answers, &favorites, &card_authors, &comments](const Card& card) {
                    ResponseCardEntry card_entry;
                    card_entry.cid = CIDToString(card.cid);
                    try {
                      const auto& iterable = card_authors[card.cid];
                      if (iterable.size() == 1u) {
                        const UID author_uid = (*iterable.begin()).uid;
                        card_entry.author_uid = UIDToString(author_uid);
                        card_entry.is_my_card = (uid == author_uid);
                      }
                    } catch (const yoda::SubscriptException<CardAuthor>&) {
                    }
                    try {
                      card_entry.number_of_comments = comments[card.cid].size();
                    } catch (const yoda::SubscriptException<Comment>&) {
                      // TODO(dkorolev): MatrixSubscriptException<C, X> into Yoda?
                    }
                    card_entry.text = card.text;
                    card_entry.ms = card.ms;
                    card_entry.color = card.color;
                    card_entry.relevance = RandomDouble(0, 1);
                    card_entry.ctfo_score = 50u;
                    card_entry.tfu_score = 50u;
                    card_entry.ctfo_count = card.ctfo_count;
                    card_entry.tfu_count = card.tfu_count;
                    card_entry.skip_count = card.skip_count;

                    const EntryWrapper<Answer> answer = answers.Get(uid, card.cid);
                    if (answer) {
                      const ANSWER vote = static_cast<Answer>(answer).answer;
                      if (vote == ANSWER::CTFO) {
                        card_entry.vote = "CTFO";
                      } else if (vote == ANSWER::TFU) {
                        card_entry.vote = "TFU";
                      }
                    }

                    card_entry.favorited = false;
                    const EntryWrapper<Favorite> fav = favorites.Get(uid, card.cid);
                    if (fav) {
                      card_entry.favorited = static_cast<Favorite>(fav).favorited;
                    }

                    return card_entry;
                  };

                  for (const auto& c : my_cards) {
                    r_my_cards.cards.push_back(GenerateCardForMyCards(data.Get(c.second)));
                  }

                  r_my_cards.ms = static_cast<uint64_t>(bricks::time::Now());
                  return Response(r_my_cards, "my_cards");
                }
              }
            },
            std::move(r));
      }
    }
  }

  void RouteCard(Request r) {
    // POST to add a card.
    const UID uid = StringToUID(r.url.query["uid"]);
    const std::string token = r.url.query["token"];
    if (r.method == "POST") {
      if (uid == UID::INVALID_USER) {
        DebugPrint(Printf("[/ctfo/card] Wrong UID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.BadRequest);
      } else {
        const std::string requested_url = r.url.ComposeURL();
        const CID cid = RandomCID();
        try {
          AddCardRequest request;
          try {
            ParseJSON(r.body, request);
          } catch (const bricks::ParseJSONException&) {
            const auto short_request = ParseJSON<AddCardShortRequest>(r.body);
            request.text = short_request.text;
            request.color = CARD_COLORS[static_cast<uint64_t>(cid) % CARD_COLORS.size()];
          }
          storage_.Transaction(
              [this, cid, uid, token, request, requested_url](StorageAPI::T_DATA data) {
                bool token_is_valid = false;
                const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
                if (auth_token_accessor.Cols().Has(token)) {
                  // Something went terribly wrong
                  // if we have more than one authentication key for token.
                  assert(auth_token_accessor[token].size() == 1);
                  if (auth_token_accessor[token].begin()->valid) {
                    // Double check, if the provided `uid` is correct as well.
                    const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
                    token_is_valid = auth_uid_accessor.Has(auth_token_accessor[token].begin().key(), uid);
                  }
                }
                if (!token_is_valid) {
                  DebugPrint(Printf("[/ctfo/card] Invalid token. Requested URL = '%s'", requested_url.c_str()));
                  return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
                } else {
                  DebugPrint(
                      Printf("[/ctfo/card] Token validated. Requested URL = '%s'", requested_url.c_str()));
                  const auto now = static_cast<uint64_t>(bricks::time::Now());

                  auto cards_mutator = Dictionary<Card>::Mutator(data);
                  auto authors_mutator = Matrix<CardAuthor>::Mutator(data);
                  auto favorites_mutator = Matrix<Favorite>::Mutator(data);

                  Card card;
                  card.cid = cid;
                  card.text = request.text;
                  card.color = request.color;
                  cards_mutator.Add(card);

                  CardAuthor author;
                  author.uid = uid;
                  author.cid = cid;
                  authors_mutator.Add(author);

                  if (false) {
                    // Master Gene taught us own cards should not be favorited by default.
                    Favorite favorite;
                    favorite.uid = uid;
                    favorite.cid = cid;
                    favorite.favorited = true;
                    favorites_mutator.Add(favorite);
                  }

                  AddCardResponse response;
                  response.ms = now;
                  response.cid = CIDToString(cid);
                  return Response(response, "created");
                }
              },
              std::move(r));
        } catch (const bricks::ParseJSONException&) {
          DebugPrint(Printf("[/ctfo/card] Could not parse POST body. Requested URL = '%s'",
                            r.url.ComposeURL().c_str()));
          r("NEED VALID BODY\n", HTTPResponseCode.BadRequest);
        }
      }
    } else if (r.method == "DELETE") {
      const CID cid = StringToCID(r.url.query["cid"]);
      if (cid == CID::INVALID_CARD) {
        DebugPrint(Printf("[/ctfo/card] Wrong CID. Requested URL = '%s'", r.url.ComposeURL().c_str()));
        r("NEED VALID OID\n", HTTPResponseCode.BadRequest);
      } else {
        const std::string requested_url = r.url.ComposeURL();
        storage_.Transaction(
            [this, requested_url, uid, cid, token](StorageAPI::T_DATA data) {
              bool token_is_valid = false;
              const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
              if (auth_token_accessor.Cols().Has(token)) {
                // Something went terribly wrong
                // if we have more than one authentication key for token.
                assert(auth_token_accessor[token].size() == 1);
                if (auth_token_accessor[token].begin()->valid) {
                  // Double check, if the provided `uid` is correct as well.
                  const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
                  token_is_valid = auth_uid_accessor.Has(auth_token_accessor[token].begin().key(), uid);
                }
              }
              if (!token_is_valid) {
                DebugPrint(Printf("[/ctfo/card] Invalid token. Requested URL = '%s'", requested_url.c_str()));
                return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
              } else {
                DebugPrint(Printf("[/ctfo/card] Token validated. Requested URL = '%s'", requested_url.c_str()));
                // TODO(dkorolev): Do something smart about non-existing cards.
                auto cards_mutator = Dictionary<Card>::Mutator(data);
                cards_mutator.Delete(cid);
                auto card_authors_mutator = Matrix<CardAuthor>::Mutator(data);
                card_authors_mutator.Delete(cid, uid);
                try {
                  auto comments_mutator = Matrix<Comment>::Mutator(data);
                  std::vector<OID> oids_to_delete;
                  for (const Comment& c : comments_mutator[cid]) {
                    oids_to_delete.push_back(c.oid);
                  }
                  for (const OID& o : oids_to_delete) {
                    comments_mutator.Delete(cid, o);
                  }
                } catch (const yoda::SubscriptException<Comment>&) {
                  DebugPrint(Printf("[/ctfo/card] yoda::SubscriptException<Comment>, Requested URL = '%s'",
                                    requested_url.c_str()));
                }
                DeleteCardResponse response;
                response.ms = static_cast<uint64_t>(bricks::time::Now());
                return Response(response, "deleted");
              }
            },
            std::move(r));
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
        storage_.Transaction(
            [this, uid, cid, token, requested_url](StorageAPI::T_DATA data) {
              bool token_is_valid = false;
              const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
              if (auth_token_accessor.Cols().Has(token)) {
                // Something went terribly wrong
                // if we have more than one authentication key for token.
                assert(auth_token_accessor[token].size() == 1);
                if (auth_token_accessor[token].begin()->valid) {
                  // Double check, if the provided `uid` is correct as well.
                  const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
                  token_is_valid = auth_uid_accessor.Has(auth_token_accessor[token].begin().key(), uid);
                }
              }
              if (!token_is_valid) {
                DebugPrint("[/ctfo/comments] Invalid token.");
                return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
              } else {
                DebugPrint("[/ctfo/comments] Token validated.");
                const auto user = data.Get(uid);
                if (!user) {
                  return Response("NEED VALID USER\n", HTTPResponseCode.Unauthorized);
                } else {
                  const auto users_accessor = Dictionary<User>::Accessor(data);
                  const auto comments_accessor = Matrix<Comment>::Accessor(data);
                  const auto comment_likes_accessor = Matrix<CommentLike>::Accessor(data);
                  const auto comment_flagged_accessor = Matrix<CommentFlagAsInappropriate>::Accessor(data);
                  std::vector<Comment> proto_comments;
                  try {
                    const auto comments = Matrix<Comment>::Accessor(data);
                    for (const auto& comment : comments[cid]) {
                      if (!comment_flagged_accessor.Has(comment.oid, uid)) {
                        proto_comments.push_back(comment);
                      }
                    }
                  } catch (const yoda::SubscriptException<Comment>&) {
                    DebugPrint(Printf("[/ctfo/comments] yoda:SubscriptException<Comment>, Requested URL = '%s'",
                                      requested_url.c_str()));
                  }
                  const auto sortkey = [&comments_accessor](const Comment& c) -> std::pair<uint64_t, uint64_t> {
                    uint64_t comment_timestamp = 0u;
                    uint64_t top_level_comment_timestamp = 0u;
                    if (c.parent_oid == OID::INVALID_COMMENT) {
                      // This comment is top-level.
                      top_level_comment_timestamp = c.ms;
                    } else {
                      // This is a 2nd-level comment.
                      comment_timestamp = c.ms;
                      try {
                        const auto& iterable = comments_accessor.Cols()[c.parent_oid];
                        if (iterable.size() == 1u) {
                          top_level_comment_timestamp = (*iterable.begin()).ms;
                        }
                      } catch (const yoda::SubscriptException<Comment>&) {
                      }
                    }
                    // Top-level comments reverse chronologically, 2nd level comments chronologically.
                    return std::make_pair(~top_level_comment_timestamp, comment_timestamp);
                  };
                  std::sort(proto_comments.begin(),
                            proto_comments.end(),
                            [&sortkey](const Comment& lhs,
                                       const Comment& rhs) { return sortkey(lhs) < sortkey(rhs); });
                  ResponseComments response;
                  response.ms = static_cast<uint64_t>(bricks::time::Now());
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
                    c.author_level = users_accessor[uid].level;
                    c.text = comment.text;
                    try {
                      const auto& likers = comment_likes_accessor[comment.oid];
                      c.number_of_likes = likers.size();
                      c.liked = likers.Has(uid);
                    } catch (const yoda::SubscriptException<CommentLike>&) {
                    }
                    c.ms = comment.ms;
                    output_comments.push_back(std::move(c));
                  }
                  return Response(response, "comments");
                }
              }
            },
            std::move(r));
      } else if (r.method == "POST") {
        const std::string requested_url = r.url.ComposeURL();
        const OID oid = RandomOID();
        try {
          AddCommentRequest request;
          try {
            ParseJSON(r.body, request);
          } catch (const bricks::ParseJSONException&) {
            const auto short_request = ParseJSON<AddCommentShortRequest>(r.body);
            request.text = short_request.text;
          }
          storage_.Transaction(
              [this, cid, uid, oid, token, request, requested_url](StorageAPI::T_DATA data) {
                bool token_is_valid = false;
                const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
                if (auth_token_accessor.Cols().Has(token)) {
                  // Something went terribly wrong
                  // if we have more than one authentication key for token.
                  assert(auth_token_accessor[token].size() == 1);
                  if (auth_token_accessor[token].begin()->valid) {
                    // Double check, if the provided `uid` is correct as well.
                    const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
                    token_is_valid = auth_uid_accessor.Has(auth_token_accessor[token].begin().key(), uid);
                  }
                }
                if (!token_is_valid) {
                  DebugPrint(
                      Printf("[/ctfo/comments] Invalid token. Requested URL = '%s'", requested_url.c_str()));
                  return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
                } else {
                  DebugPrint(
                      Printf("[/ctfo/comments] Token validated. Requested URL = '%s'", requested_url.c_str()));
                  const auto now = static_cast<uint64_t>(bricks::time::Now());

                  auto comments_mutator = Matrix<Comment>::Mutator(data);

                  Comment comment;
                  comment.cid = cid;
                  comment.oid = oid;
                  comment.author_uid = uid;
                  comment.text = request.text;

                  if (!request.parent_oid.empty()) {
                    comment.parent_oid = StringToOID(request.parent_oid);
                    try {
                      const auto& iterable = comments_mutator.Cols()[comment.parent_oid];
                      if (iterable.size() != 1u) {
                        // TODO(dkorolev): This error is oh so wrong. Fix it.
                        return Response("NEED EMPTY OR VALID PARENT_OID\n", HTTPResponseCode.BadRequest);
                      } else if ((*iterable.begin()).parent_oid != OID::INVALID_COMMENT) {
                        return Response("ATTEMPTED TO ADD A 3RD LEVEL COMMENT\n", HTTPResponseCode.BadRequest);
                      }
                    } catch (const yoda::SubscriptException<Comment>&) {
                      return Response("NEED EMPTY OR VALID PARENT_OID\n", HTTPResponseCode.BadRequest);
                    }
                  }

                  comments_mutator.Add(comment);

                  AddCommentResponse response;
                  response.ms = now;
                  response.oid = OIDToString(oid);
                  return Response(response, "created");
                }
              },
              std::move(r));
        } catch (const bricks::ParseJSONException&) {
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
          storage_.Transaction(
              [this, requested_url, uid, cid, token, oid](StorageAPI::T_DATA data) {
                bool token_is_valid = false;
                const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
                if (auth_token_accessor.Cols().Has(token)) {
                  // Something went terribly wrong
                  // if we have more than one authentication key for token.
                  assert(auth_token_accessor[token].size() == 1);
                  if (auth_token_accessor[token].begin()->valid) {
                    // Double check, if the provided `uid` is correct as well.
                    const auto auth_uid_accessor = Matrix<AuthKeyUIDPair>::Accessor(data);
                    token_is_valid = auth_uid_accessor.Has(auth_token_accessor[token].begin().key(), uid);
                  }
                }
                if (!token_is_valid) {
                  DebugPrint(
                      Printf("[/ctfo/comments] Invalid token. Requested URL = '%s'", requested_url.c_str()));
                  return Response("NEED VALID UID-TOKEN PAIR\n", HTTPResponseCode.Unauthorized);
                } else {
                  DebugPrint(
                      Printf("[/ctfo/comments] Token validated. Requested URL = '%s'", requested_url.c_str()));
                  // TODO(dkorolev): Do something smart about non-existing comments.
                  try {
                    auto comments_mutator = Matrix<Comment>::Mutator(data);
                    std::vector<OID> oids_to_delete;
                    oids_to_delete.push_back(oid);
                    for (const Comment& c : comments_mutator[cid]) {
                      if (c.parent_oid == oid) {
                        oids_to_delete.push_back(c.oid);
                      }
                    }
                    for (const OID& o : oids_to_delete) {
                      comments_mutator.Delete(cid, o);
                    }
                  } catch (const yoda::SubscriptException<Comment>&) {
                  }
                  DeleteCommentResponse response;
                  response.ms = static_cast<uint64_t>(bricks::time::Now());
                  return Response(response, "deleted");
                }
              },
              std::move(r));
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
  const int port_;
  const std::string event_log_file_;
  std::ofstream event_log_stream_;
  EventCollectorHTTPServer event_collector_;
  const bool debug_print_;

  typedef SingleFileAPI<Dictionary<User>,
                        Matrix<AuthKeyTokenPair>,
                        Matrix<AuthKeyUIDPair>,
                        Dictionary<Card>,
                        Matrix<CardAuthor>,
                        Matrix<Answer>,
                        Matrix<Favorite>,
                        Matrix<Comment>,
                        Matrix<CommentLike>,
                        Matrix<CardFlagAsInappropriate>,
                        Matrix<CommentFlagAsInappropriate>> StorageAPI;
  StorageAPI storage_;

  const std::map<std::string, RESPONSE> valid_responses_ = {{"CTFO", RESPONSE::CTFO},
                                                            {"TFU", RESPONSE::TFU},
                                                            {"SKIP", RESPONSE::SKIP},
                                                            {"FAV", RESPONSE::FAV_CARD},
                                                            {"FAV_CARD", RESPONSE::FAV_CARD},
                                                            {"UNFAV", RESPONSE::UNFAV_CARD},
                                                            {"UNFAV_CARD", RESPONSE::UNFAV_CARD},
                                                            {"LIKE_COMMENT", RESPONSE::LIKE_COMMENT},
                                                            {"UNLIKE_COMMENT", RESPONSE::UNLIKE_COMMENT},
                                                            {"FLAG_COMMENT", RESPONSE::FLAG_COMMENT},
                                                            {"FLAG_CARD", RESPONSE::FLAG_CARD}};

  void DebugPrint(const std::string& message) {
    if (debug_print_) {
      std::cerr << message << std::endl;
    }
  }

  void CopyUserInfoToResponseEntry(const User& user, ResponseUserEntry& entry) {
    entry.uid = UIDToString(user.uid);
    entry.score = user.score;
    entry.level = user.level;
    if (user.level < LEVEL_SCORES.size() - 1) {
      entry.next_level_score = LEVEL_SCORES[user.level + 1];  // LEVEL_SCORES = { 0u, ... }
    } else {
      entry.next_level_score = 0u;
    }
  }

  ResponseFeed GenerateResponseFeed(StorageAPI::T_DATA& data, ResponseUserEntry user_entry, size_t feed_size) {
    ResponseFeed response;
    constexpr size_t FEED_SIZE_LIMIT = 300ul;
    const size_t max_count = std::min(feed_size, FEED_SIZE_LIMIT);
    response.user = user_entry;

    const auto cards = Dictionary<Card>::Accessor(data);
    const auto answers = Matrix<Answer>::Accessor(data);
    const auto flagged_cards = Matrix<CardFlagAsInappropriate>::Accessor(data);
    const auto favorites = Matrix<Favorite>::Accessor(data);

    std::set<std::pair<double, CID>> hot_cards;
    std::set<std::pair<double, CID>> recent_cards;
    const UID uid = StringToUID(response.user.uid);

    const uint64_t now = static_cast<uint64_t>(bricks::time::Now());
    for (const auto& card : cards) {
      if (!answers.Has(uid, card.cid) && !flagged_cards.Has(card.cid, uid)) {
        // For the recent feed, relevance is the function of the age of he card.
        // Added just now => 1.00. Added 24 hour ago => 0.99. Added 48 hours ago => 0.99^2. Etc.
        const double time_key = std::pow(0.99, (now - card.ms) * (1.0 / (1000 * 60 * 60 * 24)));
        hot_cards.emplace(RandomDouble(0.2, 0.4), card.cid);
        recent_cards.emplace(time_key, card.cid);
        if (hot_cards.size() > max_count) {
          hot_cards.erase(hot_cards.begin());
        }
        if (recent_cards.size() > max_count) {
          recent_cards.erase(recent_cards.begin());
        }
      }
    }

    const auto card_authors = Matrix<CardAuthor>::Accessor(data);
    const auto comments = Matrix<Comment>::Accessor(data);
    const auto GenerateCardForFeed =
        [this, uid, &answers, &favorites, &comments, &card_authors](const Card& card) {
      ResponseCardEntry card_entry;
      card_entry.cid = CIDToString(card.cid);
      try {
        const auto& iterable = card_authors[card.cid];
        if (iterable.size() == 1u) {
          const UID author_uid = (*iterable.begin()).uid;
          card_entry.author_uid = UIDToString(author_uid);
          card_entry.is_my_card = (uid == author_uid);
        }
      } catch (const yoda::SubscriptException<CardAuthor>&) {
      }
      try {
        card_entry.number_of_comments = comments[card.cid].size();
      } catch (const yoda::SubscriptException<Comment>&) {
        // TODO(dkorolev): MatrixSubscriptException<C, X> into Yoda?
      }
      card_entry.text = card.text;
      card_entry.ms = card.ms;
      card_entry.color = card.color;
      card_entry.relevance = RandomDouble(0, 1);
      card_entry.ctfo_score = 50u;
      card_entry.tfu_score = 50u;
      card_entry.ctfo_count = card.ctfo_count;
      card_entry.tfu_count = card.tfu_count;
      card_entry.skip_count = card.skip_count;

      card_entry.vote = "";
      const EntryWrapper<Answer> answer = answers.Get(uid, card.cid);
      if (answer) {
        const ANSWER vote = static_cast<Answer>(answer).answer;
        if (vote == ANSWER::CTFO) {
          card_entry.vote = "CTFO";
        } else if (vote == ANSWER::TFU) {
          card_entry.vote = "TFU";
        }
      }

      card_entry.favorited = false;
      const EntryWrapper<Favorite> fav = favorites.Get(uid, card.cid);
      if (fav) {
        card_entry.favorited = static_cast<Favorite>(fav).favorited;
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
        feeds[i].get().push_back(GenerateCardForFeed(data.Get(cit->second)));
        feeds[i].get().back().relevance = cit->first;
      }
    }

    response.ms = static_cast<uint64_t>(bricks::time::Now());
    DebugPrint(Printf("[RespondWithFeed] Generated response for UID '%s' with %u 'hot' and %u 'recent' cards",
                      response.user.uid.c_str(),
                      response.feed_hot.size(),
                      response.feed_recent.size()));
    return response;
  }

  void OnMidichloriansEvent(const LogEntryWithHeaders& entry) {
    std::unique_ptr<MidichloriansEvent> event;
    if (entry.m == "POST") {
      try {
        ParseJSON(entry.b, event);
        UpdateStateOnEvent(event);
      } catch (const bricks::ParseJSONException&) {
        DebugPrint(Printf("[OnMidichloriansEvent] ParseJSON failed. entry.b = '%s')", entry.b.c_str()));
      }
    } else {
      if (entry.m != "TICK") {
        DebugPrint(Printf(
            "[OnMidichloriansEvent] Suspicious event with method '%s' (t = %llu)", entry.m.c_str(), entry.t));
      }
    }
  }

  void UpdateStateOnEvent(const std::unique_ptr<MidichloriansEvent>& event) {
    DebugPrint("[UpdateStateOnEvent] Parsing event.");
    try {
      const iOSGenericEvent& ge = dynamic_cast<const iOSGenericEvent&>(*event.get());
      try {
        const RESPONSE response = valid_responses_.at(ge.event);
        const std::string& uid_str = ge.fields.at("uid");
        const std::string token = ge.fields.at("token");
        const std::string cid_str = ge.fields.count("cid") ? ge.fields.at("cid") : "";
        const std::string oid_str = ge.fields.count("oid") ? ge.fields.at("oid") : "";
        const UID uid = StringToUID(uid_str);
        const CID cid = StringToCID(cid_str);
        const OID oid = StringToOID(oid_str);
        DebugPrint(Printf("[UpdateStateOnEvent] Event='%s', uid='%s', cid='%s', oid='%s', token='%s'",
                          ge.event.c_str(),
                          uid_str.c_str(),
                          cid_str.c_str(),
                          oid_str.c_str(),
                          token.c_str()));
        if (uid != UID::INVALID_USER) {
          storage_.Transaction([this, uid, cid, oid, uid_str, cid_str, oid_str, token, response](
              StorageAPI::T_DATA data) {
            const auto auth_token_accessor = Matrix<AuthKeyTokenPair>::Accessor(data);
            bool token_is_valid = false;
            if (auth_token_accessor.Cols().Has(token)) {
              // Something went terribly wrong
              // if we have more than one authentication key for token.
              assert(auth_token_accessor[token].size() == 1);
              if (auth_token_accessor[token].begin()->valid) {
                token_is_valid = true;
              }
            }
            if (token_is_valid) {
              if (!data.Has(uid)) {
                DebugPrint(Printf("[UpdateStateOnEvent] Nonexistent UID '%s'.", uid_str.c_str()));
                return;
              }
              if (response == RESPONSE::SKIP || response == RESPONSE::CTFO || response == RESPONSE::TFU) {
                if (cid == CID::INVALID_CARD) {
                  DebugPrint("[UpdateStateOnEvent] No CID.");
                  return;
                }
                if (!data.Has(cid)) {
                  DebugPrint(
                      Printf("[UpdateStateOnEvent] Nonexistent CID '%s' in SKIP/CTFO/TFU.", cid_str.c_str()));
                  return;
                }
                auto answers_mutator = Matrix<Answer>::Mutator(data);
                if (!answers_mutator.Has(uid, cid)) {  // Do not overwrite existing answers.
                  data.Add(Answer(uid, cid, static_cast<ANSWER>(response)));
                  DebugPrint(Printf("[UpdateStateOnEvent] Added new answer: [%s, %s, %d]",
                                    UIDToString(uid).c_str(),
                                    CIDToString(cid).c_str(),
                                    static_cast<int>(response)));
                  Card card = data.Get(cid);
                  auto user_mutator = Dictionary<User>::Mutator(data);
                  User user = user_mutator.Get(uid);
                  if (response != RESPONSE::SKIP) {
                    if (response == RESPONSE::CTFO) {
                      ++card.ctfo_count;
                      DebugPrint(Printf("[UpdateStateOnEvent] Card '%s' new ctfo_count = %u",
                                        CIDToString(cid).c_str(),
                                        card.ctfo_count));
                      user.score += 50u;
                      DebugPrint(Printf("[UpdateStateOnEvent] User '%s' got %u points for 'CTFO' answer",
                                        UIDToString(uid).c_str(),
                                        50u));
                    }
                    if (response == RESPONSE::TFU) {
                      ++card.tfu_count;
                      DebugPrint(Printf("[UpdateStateOnEvent] Card '%s' new tfu_count = %u",
                                        CIDToString(cid).c_str(),
                                        card.tfu_count));
                      user.score += 50u;
                      DebugPrint(Printf("[UpdateStateOnEvent] User '%s' got %u points for 'TFU' answer",
                                        UIDToString(uid).c_str(),
                                        50u));
                    }
                    if (user.level < LEVEL_SCORES.size() - 1 && user.score > LEVEL_SCORES[user.level + 1]) {
                      user.score -= LEVEL_SCORES[user.level + 1];
                      ++user.level;
                      DebugPrint(Printf("[UpdateStateOnEvent] User '%s' got promoted to a new level = %u",
                                        UIDToString(uid).c_str(),
                                        user.level));
                    }
                    data.Add(card);
                    data.Add(user);
                  }
                } else {
                  DebugPrint(
                      Printf("[UpdateStateOnEvent] Answer already exists: [%s, %s, %d]",
                             UIDToString(uid).c_str(),
                             CIDToString(cid).c_str(),
                             static_cast<int>(static_cast<Answer>(answers_mutator.Get(uid, cid)).answer)));
                }
              } else if (response == RESPONSE::FAV_CARD || response == RESPONSE::UNFAV_CARD) {
                if (cid == CID::INVALID_CARD) {
                  DebugPrint("[UpdateStateOnEvent] No CID.");
                  return;
                }
                if (!data.Has(cid)) {
                  DebugPrint(Printf("[UpdateStateOnEvent] Nonexistent CID '%s' FAV/UNFAV.", cid_str.c_str()));
                  return;
                }
                auto favorites_mutator = Matrix<Favorite>::Mutator(data);
                favorites_mutator.Add(Favorite(uid, cid, (response == RESPONSE::FAV_CARD)));
                DebugPrint(Printf("[UpdateStateOnEvent] Added favorite: [%s, %s, %s]",
                                  UIDToString(uid).c_str(),
                                  CIDToString(cid).c_str(),
                                  (response == RESPONSE::FAV_CARD) ? "Favorite" : "Unfavorite"));
              } else if (response == RESPONSE::LIKE_COMMENT || response == RESPONSE::UNLIKE_COMMENT ||
                         response == RESPONSE::FLAG_COMMENT) {
                if (oid == OID::INVALID_COMMENT) {
                  DebugPrint("[UpdateStateOnEvent] No OID.");
                  return;
                }
                if (!Matrix<Comment>::Accessor(data).Cols().Has(oid)) {
                  DebugPrint(
                      Printf("[UpdateStateOnEvent] Nonexistent OID '%s' LIKE/UNLIKE/FLAG.", oid_str.c_str()));
                  return;
                }
                if (response == RESPONSE::LIKE_COMMENT) {
                  DebugPrint(
                      Printf("[UpdateStateOnEvent] Like comment '%s' '%s'.", uid_str.c_str(), oid_str.c_str()));
                  CommentLike like;
                  like.oid = oid;
                  like.uid = uid;
                  data.Add(like);
                } else if (response == RESPONSE::UNLIKE_COMMENT) {
                  DebugPrint(Printf(
                      "[UpdateStateOnEvent] Unlike comment '%s' '%s'.", uid_str.c_str(), oid_str.c_str()));
                  Matrix<CommentLike>::Mutator(data).Delete(oid, uid);
                } else if (response == RESPONSE::FLAG_COMMENT) {
                  DebugPrint(
                      Printf("[UpdateStateOnEvent] Flag comment '%s' '%s'.", uid_str.c_str(), oid_str.c_str()));
                  CommentFlagAsInappropriate flag;
                  flag.oid = oid;
                  flag.uid = uid;
                  data.Add(flag);
                } else {
                  DebugPrint(
                      Printf("[UpdateStateOnEvent] Ignoring: Response=<%d>, uid='%s', cid='%s',token='%s'",
                             static_cast<int>(response),
                             uid_str.c_str(),
                             cid_str.c_str(),
                             token.c_str()));
                }
              } else if (response == RESPONSE::FLAG_CARD) {
                if (cid == CID::INVALID_CARD) {
                  DebugPrint("[UpdateStateOnEvent] No CID.");
                  return;
                }
                if (!Dictionary<Card>::Accessor(data).Has(cid)) {
                  DebugPrint(
                      Printf("[UpdateStateOnEvent] Nonexistent CID '%s' in FLAG_CARD.", cid_str.c_str()));
                  return;
                }
                DebugPrint(
                    Printf("[UpdateStateOnEvent] Flag card '%s' '%s'.", uid_str.c_str(), cid_str.c_str()));
                CardFlagAsInappropriate flag;
                flag.cid = cid;
                flag.uid = uid;
                data.Add(flag);
              } else {
                DebugPrint(Printf("[UpdateStateOnEvent] Ignoring: Response=<%d>, uid='%s', cid='%s',token='%s'",
                                  static_cast<int>(response),
                                  uid_str.c_str(),
                                  cid_str.c_str(),
                                  token.c_str()));
              }
            } else {
              DebugPrint(Printf("[UpdateStateOnEvent] Not valid token '%s' found in event.", token.c_str()));
            }
          });
        } else {
          DebugPrint("[UpdateStateOnEvent] Invalid UID.");
        }
      } catch (const std::out_of_range& e) {
        DebugPrint(Printf("[UpdateStateOnEvent] std::out_of_range: '%s'", e.what()));
      }
    } catch (const std::bad_cast&) {
      // `event` is not an `iOSGenericEvent`.
      DebugPrint("[UpdateStateOnEvent] Not an `iOSGenericEvent`.");
    }
  }
};

#endif  // CTFO_SERVER_H
