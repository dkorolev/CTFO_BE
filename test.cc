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

#define CURRENT_MOCK_TIME  // `SetNow()`.

#include "server.h"

#include "../Current/Bricks/file/file.h"

#include "../Current/Bricks/dflags/dflags.h"
#include "../Current/3rdparty/gtest/gtest-main-with-dflags.h"

DEFINE_string(cards_file, "cards.json", "Cards data file in JSON format.");
DEFINE_int32(api_port, 8383, "Port to spawn CTFO RESTful server on.");
DEFINE_int32(event_log_port, 8384, "Port to spawn event collector on.");

using namespace CTFO;
using namespace current::midichlorians::ios;
using events_variant_t = Variant<ios_events_t>;

// Uncomment the following line to have the unit test dump debug information to console.
// #define CTFO_DEBUG

std::unique_ptr<CTFOServer> SpawnTestServer(const std::string& suffix) {
#ifdef CTFO_DEBUG
  const std::string db_file = "unittest-db-" + suffix + ".log";
  const std::string log_file = "unittest-log-" + suffix + ".log";
  current::FileSystem::RmFile(db_file, current::FileSystem::RmFileParameters::Silent);
  current::FileSystem::RmFile(log_file, current::FileSystem::RmFileParameters::Silent);
#else
  static_cast<void>(suffix);

  const std::string db_file = current::FileSystem::GenTmpFileName();
  current::FileSystem::ScopedRmFile scoped_rm_db_file(db_file);

  const std::string log_file = current::FileSystem::GenTmpFileName();
  current::FileSystem::ScopedRmFile scoped_rm_log_file(log_file);
#endif

  current::time::SetNow(std::chrono::microseconds(1000));
  current::random::SetRandomSeed(42);

  return std::make_unique<CTFOServer>(FLAGS_cards_file,
                                      FLAGS_api_port,
                                      db_file,
                                      FLAGS_event_log_port,
                                      log_file,
                                      std::chrono::milliseconds(100)
#ifdef CTFO_DEBUG
                                      // clang-format off
                                 ,
                                 true  // Debug print.
#endif
                                      // clang-format on
                                      );
}

TEST(CTFO, IDsRangeTest) {
  EXPECT_EQ("c00000000000000000042", CIDToString(static_cast<CID>(42)));
  EXPECT_TRUE(static_cast<int>(42) == static_cast<int>(StringToCID(CIDToString(static_cast<CID>(42)))));

  EXPECT_EQ("u00000000000000000042", UIDToString(static_cast<UID>(42)));
  EXPECT_TRUE(static_cast<int>(42) == static_cast<int>(StringToUID(UIDToString(static_cast<UID>(42)))));

  EXPECT_EQ("o00000000000000000042", OIDToString(static_cast<OID>(42)));
  EXPECT_TRUE(static_cast<int>(42) == static_cast<int>(StringToOID(OIDToString(static_cast<OID>(42)))));

  EXPECT_NE(CIDToString(static_cast<CID>(42)), UIDToString(static_cast<UID>(42)));
  EXPECT_NE(CIDToString(static_cast<CID>(42)), OIDToString(static_cast<OID>(42)));
  EXPECT_NE(UIDToString(static_cast<UID>(42)), OIDToString(static_cast<OID>(42)));
}

// TODO(dkorolev): Strictly speaking, this test is flaky, since Midichlorians logs processing is async.
// TODO(dkorolev): Fix it.
TEST(CTFO, SmokeTest) {
  const auto server_scope = SpawnTestServer("smoke");

  // Get authentication `uid` and `token`.
  current::time::SetNow(std::chrono::microseconds(101 * 1000));
  const char* const auth_id = "A_BUNCH_OF_DIGITS";
  const char* const auth_key = "1234567890abcdef";
  const auto auth_http_response = HTTP(
      POST(Printf("http://localhost:%d/ctfo/auth/ios?id=%s&key=%s", FLAGS_api_port, auth_id, auth_key), ""));
  EXPECT_EQ(200, static_cast<int>(auth_http_response.code));
  const auto auth_response = ParseResponse<ResponseFeed>(auth_http_response.body);
  EXPECT_EQ(101u, auth_response.ms.count());
  const std::string actual_uid = auth_response.user.uid;
  const std::string actual_token = auth_response.user.token;

  // Get authentication `uid` and `token` for another user.
  current::time::SetNow(std::chrono::microseconds(201 * 1000));
  const char* const another_auth_id = "ANOTHER_BUNCH_OF_DIGITS";
  const char* const another_auth_key = "abcdef1234567890";
  const auto another_auth_http_response = HTTP(POST(
      Printf(
          "http://localhost:%d/ctfo/auth/ios?id=%s&key=%s", FLAGS_api_port, another_auth_id, another_auth_key),
      ""));
  EXPECT_EQ(200, static_cast<int>(another_auth_http_response.code));
  const auto another_auth_response = ParseResponse<ResponseFeed>(another_auth_http_response.body);
  EXPECT_EQ(201u, another_auth_response.ms.count());
  const std::string another_actual_uid = another_auth_response.user.uid;
  const std::string another_actual_token = another_auth_response.user.token;

  EXPECT_NE(actual_uid, another_actual_uid);
  EXPECT_NE(actual_token, another_actual_token);

  current::time::SetNow(std::chrono::microseconds(1001 * 1000));

  // Get the feed.
  std::unordered_set<std::string> hot_cids;
  std::unordered_set<std::string> hot_texts;
  std::unordered_set<std::string> recent_cids;
  std::unordered_set<std::string> recent_texts;
  {
    const auto feed_http_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&feed_count=40",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(feed_http_response.code));
    const auto feed_response = ParseResponse<ResponseFeed>(feed_http_response.body);
    EXPECT_EQ(1001u, feed_response.ms.count());
    EXPECT_EQ(actual_uid, feed_response.user.uid);
    EXPECT_EQ(actual_token, feed_response.user.token);
    EXPECT_EQ(0u, feed_response.user.level);
    EXPECT_EQ(0u, feed_response.user.score);
    EXPECT_EQ(15000u, feed_response.user.next_level_score);
    EXPECT_EQ(40u, feed_response.feed_hot.size());
    EXPECT_EQ(40u, feed_response.feed_recent.size());

    for (const ResponseCardEntry& card : feed_response.feed_hot) {
      hot_cids.insert(card.cid);
      hot_texts.insert(card.text);
      EXPECT_EQ(0u, card.ctfo_count);
      EXPECT_EQ(0u, card.tfu_count);
      EXPECT_EQ(0u, card.skip_count);
      EXPECT_FALSE(card.is_my_card);
      EXPECT_EQ(0u, card.number_of_comments);
    }
    for (const ResponseCardEntry& card : feed_response.feed_recent) {
      recent_cids.insert(card.cid);
      recent_texts.insert(card.text);
      EXPECT_EQ(0u, card.ctfo_count);
      EXPECT_EQ(0u, card.tfu_count);
      EXPECT_EQ(0u, card.skip_count);
      EXPECT_FALSE(card.is_my_card);
      EXPECT_EQ(0u, card.number_of_comments);
    }
    EXPECT_EQ(40u, hot_cids.size());
    EXPECT_EQ(40u, hot_texts.size());
    EXPECT_EQ(40u, recent_cids.size());
    EXPECT_EQ(40u, recent_texts.size());
  }

  // Extract card CID-s
  std::string cid1;
  std::string cid2;
  {
    cid1 = *hot_cids.begin();
    cid2 = *(++hot_cids.begin());
  }

  // Confirm that GET /ctfo/card does the job.
  {
    {
      const auto get_card_response =
          HTTP(GET(Printf("http://localhost:%d/ctfo/card?cid=%s", FLAGS_api_port, cid1.c_str())));
      EXPECT_EQ(200, static_cast<int>(get_card_response.code));
      const auto card_entry = ParseResponse<ResponseCardEntry>(get_card_response.body);
      EXPECT_EQ(card_entry.cid, cid1);
      EXPECT_FALSE(card_entry.favorited);
      EXPECT_FALSE(card_entry.is_my_card);
    }
    {
      const auto get_card_response = HTTP(GET(Printf(
          "http://localhost:%d/ctfo/card?uid=%s&cid=%s", FLAGS_api_port, actual_uid.c_str(), cid1.c_str())));
      EXPECT_EQ(200, static_cast<int>(get_card_response.code));
      const auto card_entry = ParseResponse<ResponseCardEntry>(get_card_response.body);
      EXPECT_EQ(card_entry.cid, cid1);
      EXPECT_FALSE(card_entry.favorited);
      EXPECT_FALSE(card_entry.is_my_card);
    }
    {
      const auto get_card_response = HTTP(GET(Printf("http://localhost:%d/ctfo/card?cid=%s",
                                                     FLAGS_api_port,
                                                     CIDToString(static_cast<CID>(987654321)).c_str())));
      EXPECT_EQ(404, static_cast<int>(get_card_response.code));
      EXPECT_EQ("NO SUCH CARD\n", get_card_response.body);
    }
  }

  // Add two cards to favorites.
  iOSGenericEvent favorite_event;

  {
    current::time::SetNow(std::chrono::microseconds(10001 * 1000));
    favorite_event.event = "FAV";
    favorite_event.fields["uid"] = actual_uid;
    favorite_event.fields["cid"] = cid1;
    favorite_event.fields["token"] = actual_token;
    const auto post_favorite_response_1 =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), favorite_event));
    EXPECT_EQ(200, static_cast<int>(post_favorite_response_1.code));
    EXPECT_EQ("OK\n", post_favorite_response_1.body);

    current::time::SetNow(std::chrono::microseconds(10002 * 1000));
    favorite_event.fields["cid"] = cid2;
    const auto post_favorite_response_2 =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), favorite_event));
    EXPECT_EQ(200, static_cast<int>(post_favorite_response_2.code));
    EXPECT_EQ("OK\n", post_favorite_response_2.body);
  }

  // Confirm that GET /ctfo/card returns card favorite status given the `uid`.
  {
    current::time::SetNow(std::chrono::microseconds(10003 * 1000));
    {
      const auto get_card_response = HTTP(GET(Printf(
          "http://localhost:%d/ctfo/card?uid=%s&cid=%s", FLAGS_api_port, actual_uid.c_str(), cid1.c_str())));
      EXPECT_EQ(200, static_cast<int>(get_card_response.code));
      const auto card_entry = ParseResponse<ResponseCardEntry>(get_card_response.body);
      EXPECT_EQ(card_entry.cid, cid1);
      EXPECT_TRUE(card_entry.favorited);
    }
    {
      const auto get_card_response = HTTP(GET(Printf("http://localhost:%d/ctfo/card?uid=%s&cid=%s",
                                                     FLAGS_api_port,
                                                     another_actual_uid.c_str(),
                                                     cid1.c_str())));
      EXPECT_EQ(200, static_cast<int>(get_card_response.code));
      const auto card_entry = ParseResponse<ResponseCardEntry>(get_card_response.body);
      EXPECT_EQ(card_entry.cid, cid1);
      EXPECT_FALSE(card_entry.favorited);  // Since the `uid` of another user was passed in.
    }
    {
      const auto get_card_response =
          HTTP(GET(Printf("http://localhost:%d/ctfo/card?cid=%s", FLAGS_api_port, cid1.c_str())));
      EXPECT_EQ(200, static_cast<int>(get_card_response.code));
      const auto card_entry = ParseResponse<ResponseCardEntry>(get_card_response.body);
      EXPECT_EQ(card_entry.cid, cid1);
      EXPECT_FALSE(card_entry.favorited);  // Since no `uid` was passed in.
    }
  }

  // Attempt to add a non-existing card to favorites.
  {
    current::time::SetNow(std::chrono::microseconds(10004 * 1000));
    favorite_event.fields["cid"] = CIDToString(static_cast<CID>(987654321));  // A non-existent ID.
    const auto post_favorite_response_3 =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), favorite_event));
    EXPECT_EQ(200, static_cast<int>(post_favorite_response_3.code));
    EXPECT_EQ("OK\n", post_favorite_response_3.body);
  }

  // Confirm both are returned as favorites for this user.
  {
    current::time::SetNow(std::chrono::microseconds(11001 * 1000));
    const auto feed_with_2_favs_response = HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                                                           FLAGS_api_port,
                                                           actual_uid.c_str(),
                                                           actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(feed_with_2_favs_response.code));
    const auto two_favs_response = ParseResponse<ResponseFavs>(feed_with_2_favs_response.body);
    EXPECT_EQ(11001u, two_favs_response.ms.count());
    EXPECT_EQ(actual_uid, two_favs_response.user.uid);
    ASSERT_EQ(2u, two_favs_response.cards.size());
    EXPECT_EQ(cid2, two_favs_response.cards[0].cid);
    EXPECT_EQ(cid1, two_favs_response.cards[1].cid);
  }

  // Unfavorite one card.
  {
    current::time::SetNow(std::chrono::microseconds(12001 * 1000));
    favorite_event.event = "UNFAV";
    favorite_event.fields["cid"] = cid1;
    const auto post_unfavorite_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), favorite_event));
    EXPECT_EQ(200, static_cast<int>(post_unfavorite_response.code));
    EXPECT_EQ("OK\n", post_unfavorite_response.body);
  }

  // Confirm the remaining one is returned as a favorite for this user.
  {
    current::time::SetNow(std::chrono::microseconds(13001 * 1000));
    const auto feed_with_1_fav_response = HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                                                          FLAGS_api_port,
                                                          actual_uid.c_str(),
                                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(feed_with_1_fav_response.code));
    const auto one_fav_response = ParseResponse<ResponseFavs>(feed_with_1_fav_response.body);
    EXPECT_EQ(13001u, one_fav_response.ms.count());
    EXPECT_EQ(actual_uid, one_fav_response.user.uid);
    ASSERT_EQ(1u, one_fav_response.cards.size());
    EXPECT_EQ(cid2, one_fav_response.cards[0].cid);
    EXPECT_EQ("", one_fav_response.cards[0].vote);
    EXPECT_EQ(0u, one_fav_response.cards[0].ctfo_count);
    EXPECT_EQ(0u, one_fav_response.cards[0].tfu_count);
    EXPECT_EQ(0u, one_fav_response.cards[0].skip_count);
  }

  // First, skip this card. This test casting the "CTFO" vote is possible after the card was skipped.
  {
    current::time::SetNow(std::chrono::microseconds(14001 * 1000));
    iOSGenericEvent ctfo_event;
    ctfo_event.event = "SKIP";
    ctfo_event.fields["uid"] = actual_uid;
    ctfo_event.fields["cid"] = cid2;
    ctfo_event.fields["token"] = actual_token;
    const auto post_ctfo_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), ctfo_event));
    EXPECT_EQ(200, static_cast<int>(post_ctfo_response.code));
    EXPECT_EQ("OK\n", post_ctfo_response.body);
  }

  // Confirm the card has one "SKIP" counter.
  {
    current::time::SetNow(std::chrono::microseconds(14002 * 1000));
    const auto feed_with_skip_made_on_fav = HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                                                            FLAGS_api_port,
                                                            actual_uid.c_str(),
                                                            actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(feed_with_skip_made_on_fav.code));
    const auto skip_made_response = ParseResponse<ResponseFavs>(feed_with_skip_made_on_fav.body);
    EXPECT_EQ(14002u, skip_made_response.ms.count());
    EXPECT_EQ(actual_uid, skip_made_response.user.uid);
    ASSERT_EQ(1u, skip_made_response.cards.size());
    EXPECT_EQ(cid2, skip_made_response.cards[0].cid);
    EXPECT_EQ("", skip_made_response.cards[0].vote);
    EXPECT_EQ(0u, skip_made_response.cards[0].ctfo_count);
    EXPECT_EQ(0u, skip_made_response.cards[0].tfu_count);
    EXPECT_EQ(1u, skip_made_response.cards[0].skip_count);
  }

  // Cast a vote on this card.
  {
    current::time::SetNow(std::chrono::microseconds(14003 * 1000));
    iOSGenericEvent ctfo_event;
    ctfo_event.event = "CTFO";
    ctfo_event.fields["uid"] = actual_uid;
    ctfo_event.fields["cid"] = cid2;
    ctfo_event.fields["token"] = actual_token;
    const auto post_ctfo_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), ctfo_event));
    EXPECT_EQ(200, static_cast<int>(post_ctfo_response.code));
    EXPECT_EQ("OK\n", post_ctfo_response.body);
  }

  // Confirm the card has one "CTFO" vote.
  {
    current::time::SetNow(std::chrono::microseconds(15001 * 1000));
    const auto feed_with_ctfo_cast_on_fav = HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                                                            FLAGS_api_port,
                                                            actual_uid.c_str(),
                                                            actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(feed_with_ctfo_cast_on_fav.code));
    const auto ctfo_vote_cast_response = ParseResponse<ResponseFavs>(feed_with_ctfo_cast_on_fav.body);
    EXPECT_EQ(15001u, ctfo_vote_cast_response.ms.count());
    EXPECT_EQ(actual_uid, ctfo_vote_cast_response.user.uid);
    ASSERT_EQ(1u, ctfo_vote_cast_response.cards.size());
    EXPECT_EQ(cid2, ctfo_vote_cast_response.cards[0].cid);
    EXPECT_EQ("CTFO", ctfo_vote_cast_response.cards[0].vote);
    EXPECT_EQ(1u, ctfo_vote_cast_response.cards[0].ctfo_count);
    EXPECT_EQ(0u, ctfo_vote_cast_response.cards[0].tfu_count);
    EXPECT_EQ(0u, ctfo_vote_cast_response.cards[0].skip_count);
  }

  // Add a card.
  std::string added_card_cid;
  {
    current::time::SetNow(std::chrono::microseconds(16001 * 1000));
    AddCardRequest add_card_request;
    add_card_request.text = "Foo.";
    add_card_request.color.red = 1;
    add_card_request.color.green = 2;
    add_card_request.color.blue = 3;
    const auto post_card_response = HTTP(POST(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s",
                                                     FLAGS_api_port,
                                                     actual_uid.c_str(),
                                                     actual_token.c_str()),
                                              add_card_request));
    EXPECT_EQ(200, static_cast<int>(post_card_response.code));
    const auto add_card_response = ParseResponse<AddCardResponse>(post_card_response.body);
    EXPECT_EQ(16001u, add_card_response.ms.count());

    added_card_cid = add_card_response.cid;
  }

  // Confirm card ownership is reported by GET /card when the `uid` is provided.
  {
    {
      const auto get_card_response = HTTP(GET(Printf("http://localhost:%d/ctfo/card?uid=%s&cid=%s",
                                                     FLAGS_api_port,
                                                     actual_uid.c_str(),
                                                     added_card_cid.c_str())));
      EXPECT_EQ(200, static_cast<int>(get_card_response.code));
      const auto card_entry = ParseResponse<ResponseCardEntry>(get_card_response.body);
      EXPECT_EQ(card_entry.cid, added_card_cid);
      EXPECT_TRUE(card_entry.is_my_card);
    }
    {
      const auto get_card_response = HTTP(GET(Printf("http://localhost:%d/ctfo/card?uid=%s&cid=%s",
                                                     FLAGS_api_port,
                                                     another_actual_uid.c_str(),
                                                     added_card_cid.c_str())));
      EXPECT_EQ(200, static_cast<int>(get_card_response.code));
      const auto card_entry = ParseResponse<ResponseCardEntry>(get_card_response.body);
      EXPECT_EQ(card_entry.cid, added_card_cid);
      EXPECT_FALSE(card_entry.is_my_card);  // Since `another_actual_uid` != `actual_uid`.
    }
    {
      const auto get_card_response =
          HTTP(GET(Printf("http://localhost:%d/ctfo/card?cid=%s", FLAGS_api_port, added_card_cid.c_str())));
      EXPECT_EQ(200, static_cast<int>(get_card_response.code));
      const auto card_entry = ParseResponse<ResponseCardEntry>(get_card_response.body);
      EXPECT_EQ(card_entry.cid, added_card_cid);
      EXPECT_FALSE(card_entry.is_my_card);  // Since `uid` is not passed in.
    }
  }

  // Confirm the freshly added card tops the "Recent" feed. And that its age matters.
  {
    {
      const auto feed_recent = ParseResponse<ResponseFeed>(
                                   HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&feed_count=1",
                                                   FLAGS_api_port,
                                                   actual_uid.c_str(),
                                                   actual_token.c_str()))).body).feed_recent;
      ASSERT_EQ(1u, feed_recent.size());
      EXPECT_EQ(added_card_cid, feed_recent[0].cid);
      EXPECT_EQ("Foo.", feed_recent[0].text);
      EXPECT_TRUE(feed_recent[0].is_my_card);
      EXPECT_EQ(0u, feed_recent[0].number_of_comments);
      EXPECT_DOUBLE_EQ(0.9, feed_recent[0].relevance);
    }

    {
      // Request recent cards 24 hours later.
      current::time::SetNow(std::chrono::microseconds((16001 + 1000 * 60 * 60 * 24) * 1000));

      const auto feed_recent = ParseResponse<ResponseFeed>(
                                   HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&feed_count=1",
                                                   FLAGS_api_port,
                                                   actual_uid.c_str(),
                                                   actual_token.c_str()))).body).feed_recent;
      EXPECT_EQ(1u, feed_recent.size());
      EXPECT_EQ(added_card_cid, feed_recent[0].cid);
      EXPECT_EQ("Foo.", feed_recent[0].text);
      EXPECT_TRUE(feed_recent[0].is_my_card);
      EXPECT_EQ(0u, feed_recent[0].number_of_comments);
      EXPECT_DOUBLE_EQ(0.9 * 0.99, feed_recent[0].relevance);
    }

    {
      // Request recent cards 48 hours later.
      current::time::SetNow(std::chrono::microseconds((16001 + 1000 * 60 * 60 * 48) * 1000));

      const auto feed_recent = ParseResponse<ResponseFeed>(
                                   HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&feed_count=1",
                                                   FLAGS_api_port,
                                                   actual_uid.c_str(),
                                                   actual_token.c_str()))).body).feed_recent;
      EXPECT_EQ(1u, feed_recent.size());
      EXPECT_EQ(added_card_cid, feed_recent[0].cid);
      EXPECT_EQ("Foo.", feed_recent[0].text);
      EXPECT_TRUE(feed_recent[0].is_my_card);
      EXPECT_EQ(0u, feed_recent[0].number_of_comments);
      EXPECT_DOUBLE_EQ(0.9 * 0.99 * 0.99, feed_recent[0].relevance);
    }

    // Restore the time back.
    current::time::SetNow(std::chrono::microseconds(16001 * 1000));
  }

  // Confirm this new card is not favorited by default.
  {
    current::time::SetNow(std::chrono::microseconds(17001 * 1000));
    const auto favs_including_my_card_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(favs_including_my_card_response.code));
    const auto my_card_fav_response = ParseResponse<ResponseFavs>(favs_including_my_card_response.body);
    EXPECT_EQ(17001u, my_card_fav_response.ms.count());
    EXPECT_EQ(actual_uid, my_card_fav_response.user.uid);
    ASSERT_EQ(1u, my_card_fav_response.cards.size());
    EXPECT_EQ(cid2, my_card_fav_response.cards[0].cid);
    EXPECT_EQ("CTFO", my_card_fav_response.cards[0].vote);
    EXPECT_EQ(1u, my_card_fav_response.cards[0].ctfo_count);
    EXPECT_EQ(0u, my_card_fav_response.cards[0].tfu_count);
    EXPECT_EQ(0u, my_card_fav_response.cards[0].skip_count);
  }

  // Get a list of my cards, should be only one.
  {
    current::time::SetNow(std::chrono::microseconds(18001 * 1000));
    const auto my_cards = HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                          FLAGS_api_port,
                                          actual_uid.c_str(),
                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(my_cards.code));
    const auto my_cards_response = ParseResponse<ResponseMyCards>(my_cards.body);

    EXPECT_EQ(18001u, my_cards_response.ms.count());
    EXPECT_EQ(actual_uid, my_cards_response.user.uid);
    ASSERT_EQ(1u, my_cards_response.cards.size());
    EXPECT_EQ("Foo.", my_cards_response.cards[0].text);
    EXPECT_EQ(16001u, my_cards_response.cards[0].ms.count());
  }

  // Add a second card, with full JSON body, specifying the color explicitly.
  std::string added_card2_cid;
  {
    current::time::SetNow(std::chrono::microseconds(19001 * 1000));
    const auto post_card_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s",
                         FLAGS_api_port,
                         actual_uid.c_str(),
                         actual_token.c_str()),
                  "{\"card\":{\"text\":\"Bar.\",\"color\":{\"red\":100,\"green\":101,\"blue\":102}}}"));
    EXPECT_EQ(200, static_cast<int>(post_card_response.code));
    const auto add_card_response = ParseResponse<AddCardResponse>(post_card_response.body);
    EXPECT_EQ(19001u, add_card_response.ms.count());

    added_card2_cid = add_card_response.cid;
  }

  // Get a list of my cards, should be two.
  {
    current::time::SetNow(std::chrono::microseconds(20001 * 1000));
    const auto my_cards = HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                          FLAGS_api_port,
                                          actual_uid.c_str(),
                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(my_cards.code));
    const auto my_cards_response = ParseResponse<ResponseMyCards>(my_cards.body);

    EXPECT_EQ(20001u, my_cards_response.ms.count());
    EXPECT_EQ(actual_uid, my_cards_response.user.uid);
    ASSERT_EQ(2u, my_cards_response.cards.size());
    EXPECT_EQ("Bar.", my_cards_response.cards[0].text);
    EXPECT_EQ(19001u, my_cards_response.cards[0].ms.count());
    EXPECT_EQ("Foo.", my_cards_response.cards[1].text);
    EXPECT_EQ(16001u, my_cards_response.cards[1].ms.count());
  }

  // Add a third card, not specifying color.
  std::string added_card3_cid;
  {
    current::time::SetNow(std::chrono::microseconds(21001 * 1000));
    const auto post_card_response = HTTP(POST(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s",
                                                     FLAGS_api_port,
                                                     actual_uid.c_str(),
                                                     actual_token.c_str()),
                                              "{\"card\":{\"text\":\"Meh.\"}}"));
    EXPECT_EQ(200, static_cast<int>(post_card_response.code));
    const auto add_card_response = ParseResponse<AddCardResponse>(post_card_response.body);
    EXPECT_EQ(21001u, add_card_response.ms.count());

    added_card3_cid = add_card_response.cid;
  }

// Attempt to add a card but send it a malformed BODY.
#if 0
  {
    // TODO(dkorolev): Revisit as we migrate to the new type system.
    current::time::SetNow(std::chrono::microseconds(22001*1000));
    const auto post_card_response = HTTP(POST(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s",
                                                     FLAGS_api_port,
                                                     actual_uid.c_str(),
                                                     actual_token.c_str()),
                                              "This card shall not pass."));
    EXPECT_EQ(400, static_cast<int>(post_card_response.code));
    EXPECT_EQ("NEED VALID BODY\n", post_card_response.body);
  }
#endif

  // Get a list of my cards, should be three.
  {
    current::time::SetNow(std::chrono::microseconds(23001 * 1000));
    const auto my_cards = HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                          FLAGS_api_port,
                                          actual_uid.c_str(),
                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(my_cards.code));
    const auto my_cards_response = ParseResponse<ResponseMyCards>(my_cards.body);

    EXPECT_EQ(23001u, my_cards_response.ms.count());
    EXPECT_EQ(actual_uid, my_cards_response.user.uid);
    ASSERT_EQ(3u, my_cards_response.cards.size());
    EXPECT_EQ("Meh.", my_cards_response.cards[0].text);
    EXPECT_EQ(21001u, my_cards_response.cards[0].ms.count());
    EXPECT_EQ("Bar.", my_cards_response.cards[1].text);
    EXPECT_EQ(19001u, my_cards_response.cards[1].ms.count());
    EXPECT_EQ("Foo.", my_cards_response.cards[2].text);
    EXPECT_EQ(16001u, my_cards_response.cards[2].ms.count());
  }

  // Confirm that three recently added cards are on the top of the recent feed.
  {
    const auto feed_recent = ParseResponse<ResponseFeed>(
                                 HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&feed_count=3",
                                                 FLAGS_api_port,
                                                 actual_uid.c_str(),
                                                 actual_token.c_str()))).body).feed_recent;
    ASSERT_EQ(3u, feed_recent.size());
    EXPECT_EQ("Meh.", feed_recent[0].text);
    EXPECT_TRUE(feed_recent[0].is_my_card);
    EXPECT_EQ(0u, feed_recent[0].number_of_comments);
    EXPECT_EQ("Bar.", feed_recent[1].text);
    EXPECT_TRUE(feed_recent[1].is_my_card);
    EXPECT_EQ(0u, feed_recent[1].number_of_comments);
    EXPECT_EQ("Foo.", feed_recent[2].text);
    EXPECT_TRUE(feed_recent[2].is_my_card);
    EXPECT_EQ(0u, feed_recent[2].number_of_comments);
  }

  // Get comments for a non-exising card, expecting an error.
  {
    current::time::SetNow(std::chrono::microseconds(100001 * 1000));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        CIDToString(RandomCID()).c_str())));
    EXPECT_EQ(404, static_cast<int>(get_comments_response.code));
    EXPECT_EQ("NO SUCH CARD\n", get_comments_response.body);
  }

  // Get comments for the actual card, expecting valid response with no comments.
  {
    current::time::SetNow(std::chrono::microseconds(101001 * 1000));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        added_card2_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(get_comments_response.code));
    const auto response = ParseResponse<ResponseComments>(get_comments_response.body);
    EXPECT_EQ(101001u, response.ms.count());
    EXPECT_EQ(0u, response.comments.size());
  }

  // Add a top-level comment.
  std::string added_comment_oid;
  {
    current::time::SetNow(std::chrono::microseconds(102001 * 1000));
    AddCommentRequest add_comment_request;
    add_comment_request.text = "Meh.";
    const auto post_comment_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s",
                         FLAGS_api_port,
                         actual_uid.c_str(),
                         actual_token.c_str(),
                         added_card_cid.c_str()),
                  add_comment_request));
    EXPECT_EQ(200, static_cast<int>(post_comment_response.code));
    const auto add_comment_response = ParseResponse<AddCommentResponse>(post_comment_response.body);
    EXPECT_EQ(102001u, add_comment_response.ms.count());

    added_comment_oid = add_comment_response.oid;
  }

  // Confirm the card payload lists the number of comments as "1" now.
  {
    current::time::SetNow(std::chrono::microseconds(102501 * 1000));
    const auto my_cards = HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                          FLAGS_api_port,
                                          actual_uid.c_str(),
                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(my_cards.code));
    const auto my_cards_response = ParseResponse<ResponseMyCards>(my_cards.body);

    EXPECT_EQ(102501u, my_cards_response.ms.count());
    EXPECT_EQ(actual_uid, my_cards_response.user.uid);
    ASSERT_EQ(3u, my_cards_response.cards.size());
    EXPECT_EQ("Meh.", my_cards_response.cards[0].text);
    EXPECT_EQ(21001u, my_cards_response.cards[0].ms.count());
    EXPECT_EQ(0u, my_cards_response.cards[0].number_of_comments);
    EXPECT_EQ("Bar.", my_cards_response.cards[1].text);
    EXPECT_EQ(19001u, my_cards_response.cards[1].ms.count());
    EXPECT_EQ(0u, my_cards_response.cards[1].number_of_comments);
    EXPECT_EQ("Foo.", my_cards_response.cards[2].text);
    EXPECT_EQ(16001u, my_cards_response.cards[2].ms.count());
    EXPECT_EQ(1u, my_cards_response.cards[2].number_of_comments);
  }

  // Confirm that the recent feed also mentions that this card has one comment.
  {
    const auto feed_recent = ParseResponse<ResponseFeed>(
                                 HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&feed_count=3",
                                                 FLAGS_api_port,
                                                 actual_uid.c_str(),
                                                 actual_token.c_str()))).body).feed_recent;
    ASSERT_EQ(3u, feed_recent.size());
    EXPECT_EQ("Meh.", feed_recent[0].text);
    EXPECT_TRUE(feed_recent[0].is_my_card);
    EXPECT_EQ(0u, feed_recent[0].number_of_comments);
    EXPECT_EQ("Bar.", feed_recent[1].text);
    EXPECT_TRUE(feed_recent[1].is_my_card);
    EXPECT_EQ(0u, feed_recent[1].number_of_comments);
    EXPECT_EQ("Foo.", feed_recent[2].text);
    EXPECT_TRUE(feed_recent[2].is_my_card);
    EXPECT_EQ(1u, feed_recent[2].number_of_comments);
  }

  // Get comments for the card where the comment was added, expecting one.
  {
    current::time::SetNow(std::chrono::microseconds(103001 * 1000));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        added_card_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(get_comments_response.code));
    const auto response = ParseResponse<ResponseComments>(get_comments_response.body);
    EXPECT_EQ(103001u, response.ms.count());
    ASSERT_EQ(1u, response.comments.size());
    EXPECT_EQ(added_comment_oid, response.comments[0].oid);
    EXPECT_EQ("", response.comments[0].parent_oid);
    EXPECT_EQ(actual_uid, response.comments[0].author_uid);
    EXPECT_EQ("Meh.", response.comments[0].text);
    EXPECT_EQ(102001u, response.comments[0].ms.count());
  }

  // Get comments for the card where the comment was not added, expecting none.
  {
    current::time::SetNow(std::chrono::microseconds(104001 * 1000));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        added_card2_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(get_comments_response.code));
    const auto response = ParseResponse<ResponseComments>(get_comments_response.body);
    EXPECT_EQ(104001u, response.ms.count());
    EXPECT_EQ(0u, response.comments.size());
  }

  // Add another top-level comment.
  std::string added_second_comment_oid;
  {
    current::time::SetNow(std::chrono::microseconds(105001 * 1000));
    AddCommentShortRequest add_comment_request;
    add_comment_request.text = "Bla.";
    const auto post_comment_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s",
                         FLAGS_api_port,
                         actual_uid.c_str(),
                         actual_token.c_str(),
                         added_card_cid.c_str()),
                  add_comment_request));
    EXPECT_EQ(200, static_cast<int>(post_comment_response.code));
    const auto add_comment_response = ParseResponse<AddCommentResponse>(post_comment_response.body);
    EXPECT_EQ(105001u, add_comment_response.ms.count());

    added_second_comment_oid = add_comment_response.oid;
  }

  // Get comments, expecting two top-level comments.
  {
    current::time::SetNow(std::chrono::microseconds(106001 * 1000));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        added_card_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(get_comments_response.code));
    const auto response = ParseResponse<ResponseComments>(get_comments_response.body);
    EXPECT_EQ(106001u, response.ms.count());
    ASSERT_EQ(2u, response.comments.size());
    EXPECT_EQ(added_second_comment_oid, response.comments[0].oid);
    EXPECT_EQ("", response.comments[0].parent_oid);
    EXPECT_EQ(actual_uid, response.comments[0].author_uid);
    EXPECT_EQ("Bla.", response.comments[0].text);
    EXPECT_EQ(105001u, response.comments[0].ms.count());
    EXPECT_EQ(added_comment_oid, response.comments[1].oid);
    EXPECT_EQ("", response.comments[1].parent_oid);
    EXPECT_EQ(actual_uid, response.comments[1].author_uid);
    EXPECT_EQ("Meh.", response.comments[1].text);
    EXPECT_EQ(102001u, response.comments[1].ms.count());
  }

  // Add 2nd level comment 1/2.
  std::string added_nested_comment_1_oid;
  {
    current::time::SetNow(std::chrono::microseconds(107001 * 1000));
    AddCommentRequest add_comment_request;
    add_comment_request.text = "for";
    add_comment_request.parent_oid = added_second_comment_oid;
    const auto post_comment_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s",
                         FLAGS_api_port,
                         actual_uid.c_str(),
                         actual_token.c_str(),
                         added_card_cid.c_str()),
                  add_comment_request));
    EXPECT_EQ(200, static_cast<int>(post_comment_response.code));
    const auto add_comment_response = ParseResponse<AddCommentResponse>(post_comment_response.body);
    EXPECT_EQ(107001u, add_comment_response.ms.count());

    added_nested_comment_1_oid = add_comment_response.oid;
  }

  // Add 2nd level comment 2/2, to confirm the final sort order of the GET is right.
  std::string added_nested_comment_2_oid;
  {
    current::time::SetNow(std::chrono::microseconds(108001 * 1000));
    AddCommentRequest add_comment_request;
    add_comment_request.text = "real?";
    add_comment_request.parent_oid = added_second_comment_oid;
    const auto post_comment_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s",
                         FLAGS_api_port,
                         actual_uid.c_str(),
                         actual_token.c_str(),
                         added_card_cid.c_str()),
                  add_comment_request));
    EXPECT_EQ(200, static_cast<int>(post_comment_response.code));
    const auto add_comment_response = ParseResponse<AddCommentResponse>(post_comment_response.body);
    EXPECT_EQ(108001u, add_comment_response.ms.count());

    added_nested_comment_2_oid = add_comment_response.oid;
  }

  // Get comments, expecting two top-level ones, and two 2nd level ones, in the right order.
  {
    current::time::SetNow(std::chrono::microseconds(109001 * 1000));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        added_card_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(get_comments_response.code));
    const auto response = ParseResponse<ResponseComments>(get_comments_response.body);
    EXPECT_EQ(109001u, response.ms.count());

    ASSERT_EQ(4u, response.comments.size());

    EXPECT_EQ(added_second_comment_oid, response.comments[0].oid);
    EXPECT_EQ("", response.comments[0].parent_oid);
    EXPECT_EQ(actual_uid, response.comments[0].author_uid);
    EXPECT_EQ("Bla.", response.comments[0].text);
    EXPECT_EQ(105001u, response.comments[0].ms.count());
    EXPECT_EQ(actual_uid, response.comments[0].author_uid);
    EXPECT_EQ(0u, response.comments[0].author_level);
    EXPECT_EQ(0u, response.comments[0].number_of_likes);
    EXPECT_FALSE(response.comments[0].liked);

    EXPECT_EQ(added_nested_comment_1_oid, response.comments[1].oid);
    EXPECT_EQ(added_second_comment_oid, response.comments[1].parent_oid);
    EXPECT_EQ(actual_uid, response.comments[1].author_uid);
    EXPECT_EQ("for", response.comments[1].text);
    EXPECT_EQ(107001u, response.comments[1].ms.count());
    EXPECT_EQ(actual_uid, response.comments[1].author_uid);
    EXPECT_EQ(0u, response.comments[1].author_level);
    EXPECT_EQ(0u, response.comments[1].number_of_likes);
    EXPECT_FALSE(response.comments[1].liked);

    EXPECT_EQ(added_nested_comment_2_oid, response.comments[2].oid);
    EXPECT_EQ(added_second_comment_oid, response.comments[2].parent_oid);
    EXPECT_EQ(actual_uid, response.comments[2].author_uid);
    EXPECT_EQ("real?", response.comments[2].text);
    EXPECT_EQ(108001u, response.comments[2].ms.count());
    EXPECT_EQ(actual_uid, response.comments[2].author_uid);
    EXPECT_EQ(0u, response.comments[2].author_level);
    EXPECT_EQ(0u, response.comments[2].number_of_likes);
    EXPECT_FALSE(response.comments[2].liked);

    EXPECT_EQ(added_comment_oid, response.comments[3].oid);
    EXPECT_EQ("", response.comments[3].parent_oid);
    EXPECT_EQ(actual_uid, response.comments[3].author_uid);
    EXPECT_EQ("Meh.", response.comments[3].text);
    EXPECT_EQ(102001u, response.comments[3].ms.count());
    EXPECT_EQ(actual_uid, response.comments[3].author_uid);
    EXPECT_EQ(0u, response.comments[3].author_level);
    EXPECT_EQ(0u, response.comments[3].number_of_likes);
    EXPECT_FALSE(response.comments[3].liked);
  }

  // Confirm the card payload lists the number of comments as "4" now.
  {
    current::time::SetNow(std::chrono::microseconds(109501 * 1000));
    const auto my_cards = HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                          FLAGS_api_port,
                                          actual_uid.c_str(),
                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(my_cards.code));
    const auto my_cards_response = ParseResponse<ResponseMyCards>(my_cards.body);

    EXPECT_EQ(109501u, my_cards_response.ms.count());
    EXPECT_EQ(actual_uid, my_cards_response.user.uid);
    ASSERT_EQ(3u, my_cards_response.cards.size());
    EXPECT_EQ("Meh.", my_cards_response.cards[0].text);
    EXPECT_EQ(21001u, my_cards_response.cards[0].ms.count());
    EXPECT_EQ(0u, my_cards_response.cards[0].number_of_comments);
    EXPECT_EQ("Bar.", my_cards_response.cards[1].text);
    EXPECT_EQ(19001u, my_cards_response.cards[1].ms.count());
    EXPECT_EQ(0u, my_cards_response.cards[1].number_of_comments);
    EXPECT_EQ("Foo.", my_cards_response.cards[2].text);
    EXPECT_EQ(16001u, my_cards_response.cards[2].ms.count());
    EXPECT_EQ(4u, my_cards_response.cards[2].number_of_comments);
  }

  // Attempt to add a 3rd level comment, expecting an error.
  {
    current::time::SetNow(std::chrono::microseconds(110001 * 1000));
    AddCommentRequest add_comment_request;
    add_comment_request.text = "Nah.";
    add_comment_request.parent_oid = added_nested_comment_2_oid;
    const auto post_comment_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s",
                         FLAGS_api_port,
                         actual_uid.c_str(),
                         actual_token.c_str(),
                         added_card_cid.c_str()),
                  add_comment_request));
    EXPECT_EQ(400, static_cast<int>(post_comment_response.code));
    EXPECT_EQ("ATTEMPTED TO ADD A 3RD LEVEL COMMENT\n", post_comment_response.body);
  }

  // Attempt to add a 2nd level comment to a non-existing 1st level comment.
  {
    current::time::SetNow(std::chrono::microseconds(110001 * 1000));
    AddCommentRequest add_comment_request;
    add_comment_request.text = "Still nah.";
    add_comment_request.parent_oid = OIDToString(RandomOID());
    const auto post_comment_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s",
                         FLAGS_api_port,
                         actual_uid.c_str(),
                         actual_token.c_str(),
                         added_card_cid.c_str()),
                  add_comment_request));
    EXPECT_EQ(400, static_cast<int>(post_comment_response.code));
    EXPECT_EQ("NEED EMPTY OR VALID PARENT_OID\n", post_comment_response.body);
  }

  // Like the comment.
  {
    iOSGenericEvent like_comment_event;
    like_comment_event.event = "LIKE_COMMENT";
    like_comment_event.fields["uid"] = actual_uid;
    like_comment_event.fields["token"] = actual_token;
    like_comment_event.fields["oid"] = added_nested_comment_1_oid;
    const auto reponse =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), like_comment_event));
    EXPECT_EQ(200, static_cast<int>(reponse.code));
    EXPECT_EQ("OK\n", reponse.body);
  }

  // Confirm the comment got liked.
  {
    const auto comments = ParseResponse<ResponseComments>(
                              HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                                              FLAGS_api_port,
                                              actual_uid.c_str(),
                                              actual_token.c_str(),
                                              added_card_cid.c_str()))).body).comments;
    ASSERT_EQ(4u, comments.size());

    EXPECT_EQ(added_second_comment_oid, comments[0].oid);
    EXPECT_EQ("Bla.", comments[0].text);
    EXPECT_EQ(0u, comments[0].number_of_likes);
    EXPECT_FALSE(comments[0].liked);
    EXPECT_FALSE(comments[0].flagged_inappropriate);

    EXPECT_EQ(added_nested_comment_1_oid, comments[1].oid);
    EXPECT_EQ("for", comments[1].text);
    EXPECT_EQ(1u, comments[1].number_of_likes);
    EXPECT_TRUE(comments[1].liked);
    EXPECT_FALSE(comments[1].flagged_inappropriate);

    EXPECT_EQ(added_nested_comment_2_oid, comments[2].oid);
    EXPECT_EQ("real?", comments[2].text);
    EXPECT_EQ(0u, comments[2].number_of_likes);
    EXPECT_FALSE(comments[2].liked);
    EXPECT_FALSE(comments[2].flagged_inappropriate);

    EXPECT_EQ(added_comment_oid, comments[3].oid);
    EXPECT_EQ("Meh.", comments[3].text);
    EXPECT_EQ(0u, comments[3].number_of_likes);
    EXPECT_FALSE(comments[3].liked);
    EXPECT_FALSE(comments[3].flagged_inappropriate);
  }

  // Unlike the comment.
  {
    iOSGenericEvent unlike_comment_event;
    unlike_comment_event.event = "UNLIKE_COMMENT";
    unlike_comment_event.fields["uid"] = actual_uid;
    unlike_comment_event.fields["token"] = actual_token;
    unlike_comment_event.fields["oid"] = added_nested_comment_1_oid;
    const auto reponse =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), unlike_comment_event));
    EXPECT_EQ(200, static_cast<int>(reponse.code));
    EXPECT_EQ("OK\n", reponse.body);
  }

  // Confirm the comment got unliked.
  {
    const auto comments = ParseResponse<ResponseComments>(
                              HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                                              FLAGS_api_port,
                                              actual_uid.c_str(),
                                              actual_token.c_str(),
                                              added_card_cid.c_str()))).body).comments;
    ASSERT_EQ(4u, comments.size());

    EXPECT_EQ(added_second_comment_oid, comments[0].oid);
    EXPECT_EQ("Bla.", comments[0].text);
    EXPECT_EQ(0u, comments[0].number_of_likes);
    EXPECT_FALSE(comments[0].liked);
    EXPECT_FALSE(comments[0].flagged_inappropriate);

    EXPECT_EQ(added_nested_comment_1_oid, comments[1].oid);
    EXPECT_EQ("for", comments[1].text);
    EXPECT_EQ(0u, comments[1].number_of_likes);
    EXPECT_FALSE(comments[1].liked);
    EXPECT_FALSE(comments[1].flagged_inappropriate);

    EXPECT_EQ(added_nested_comment_2_oid, comments[2].oid);
    EXPECT_EQ("real?", comments[2].text);
    EXPECT_EQ(0u, comments[2].number_of_likes);
    EXPECT_FALSE(comments[2].liked);
    EXPECT_FALSE(comments[2].flagged_inappropriate);

    EXPECT_EQ(added_comment_oid, comments[3].oid);
    EXPECT_EQ("Meh.", comments[3].text);
    EXPECT_EQ(0u, comments[3].number_of_likes);
    EXPECT_FALSE(comments[3].liked);
    EXPECT_FALSE(comments[3].flagged_inappropriate);
  }

  // Flag the comment.
  {
    iOSGenericEvent flag_comment_event;
    flag_comment_event.event = "FLAG_COMMENT";
    flag_comment_event.fields["uid"] = actual_uid;
    flag_comment_event.fields["token"] = actual_token;
    flag_comment_event.fields["oid"] = added_nested_comment_1_oid;
    const auto reponse =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), flag_comment_event));
    EXPECT_EQ(200, static_cast<int>(reponse.code));
    EXPECT_EQ("OK\n", reponse.body);
  }

  // Confirm the flagged comment has the corresponding flag in the response.
  {
    const auto comments = ParseResponse<ResponseComments>(
                              HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                                              FLAGS_api_port,
                                              actual_uid.c_str(),
                                              actual_token.c_str(),
                                              added_card_cid.c_str()))).body).comments;
    ASSERT_EQ(4u, comments.size());

    EXPECT_EQ(added_second_comment_oid, comments[0].oid);
    EXPECT_EQ("Bla.", comments[0].text);
    EXPECT_EQ(0u, comments[0].number_of_likes);
    EXPECT_FALSE(comments[0].liked);

    EXPECT_EQ(added_nested_comment_1_oid, comments[1].oid);
    EXPECT_EQ("for", comments[1].text);
    EXPECT_EQ(0u, comments[1].number_of_likes);
    EXPECT_FALSE(comments[1].liked);
    EXPECT_TRUE(comments[1].flagged_inappropriate);

    EXPECT_EQ(added_nested_comment_2_oid, comments[2].oid);
    EXPECT_EQ("real?", comments[2].text);
    EXPECT_EQ(0u, comments[2].number_of_likes);
    EXPECT_FALSE(comments[2].liked);

    EXPECT_EQ(added_comment_oid, comments[3].oid);
    EXPECT_EQ("Meh.", comments[3].text);
    EXPECT_EQ(0u, comments[3].number_of_likes);
    EXPECT_FALSE(comments[3].liked);
  }

  // Delete the top-level comment with no second level comments,
  // and confirm that the number of comments goes down from 4 to 3.
  {
    current::time::SetNow(std::chrono::microseconds(600001 * 1000));
    const auto delete_comment_response =
        HTTP(DELETE(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s&oid=%s",
                           FLAGS_api_port,
                           actual_uid.c_str(),
                           actual_token.c_str(),
                           added_card_cid.c_str(),
                           added_comment_oid.c_str())));
    EXPECT_EQ(200, static_cast<int>(delete_comment_response.code));
    const auto payload = ParseResponse<DeleteCommentResponse>(delete_comment_response.body);
    EXPECT_EQ(600001u, payload.ms.count());

    EXPECT_EQ(
        3u,
        ParseResponse<ResponseMyCards>(HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                                       FLAGS_api_port,
                                                       actual_uid.c_str(),
                                                       actual_token.c_str()))).body)
            .cards[2]
            .number_of_comments);
  }

  // Try to delete a comment w/o specifying card ID.
  {
    const auto delete_comment_response =
        HTTP(DELETE(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&oid=%s",
                           FLAGS_api_port,
                           actual_uid.c_str(),
                           actual_token.c_str(),
                           added_comment_oid.c_str())));
    EXPECT_EQ(400, static_cast<int>(delete_comment_response.code));
    EXPECT_EQ("NEED VALID CID\n", delete_comment_response.body);
  }

  // Try to delete a comment using a wrong token.
  {
    const auto delete_comment_response =
        HTTP(DELETE(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=WRONG_TOKEN&cid=%s&oid=%s",
                           FLAGS_api_port,
                           actual_uid.c_str(),
                           added_card_cid.c_str(),
                           added_comment_oid.c_str())));
    EXPECT_EQ(401, static_cast<int>(delete_comment_response.code));
    EXPECT_EQ("NEED VALID UID-TOKEN PAIR\n", delete_comment_response.body);
  }

  // Delete one of two second-level comments, and confirm that
  // the number of comments goes down from 3 to 2.
  {
    current::time::SetNow(std::chrono::microseconds(112001 * 1000));
    const auto delete_comment_response =
        HTTP(DELETE(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s&oid=%s",
                           FLAGS_api_port,
                           actual_uid.c_str(),
                           actual_token.c_str(),
                           added_card_cid.c_str(),
                           added_nested_comment_2_oid.c_str())));
    EXPECT_EQ(200, static_cast<int>(delete_comment_response.code));
    const auto payload = ParseResponse<DeleteCommentResponse>(delete_comment_response.body);
    EXPECT_EQ(112001u, payload.ms.count());

    EXPECT_EQ(
        2u,
        ParseResponse<ResponseMyCards>(HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                                       FLAGS_api_port,
                                                       actual_uid.c_str(),
                                                       actual_token.c_str()))).body)
            .cards[2]
            .number_of_comments);
  }

  // Confirm that another user adding a comment results in the user who added a card being notified.
  {
    // Should be no notifications.
    {
      const auto feed_http_response =
          HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&notifications_since=0",
                          FLAGS_api_port,
                          actual_uid.c_str(),
                          actual_token.c_str())));
      EXPECT_EQ(200, static_cast<int>(feed_http_response.code));
      const auto feed_response = ParseResponse<ResponseFeed>(feed_http_response.body);
      EXPECT_EQ(0u, feed_response.notifications.size());
    }
    // Add a comment to generate a notification.
    std::string comment_to_be_notified_about_oid;
    {
      current::time::SetNow(std::chrono::microseconds(112002 * 1000));
      AddCommentRequest add_comment_request;
      add_comment_request.text = "Ding!";
      const auto post_comment_response =
          HTTP(POST(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s",
                           FLAGS_api_port,
                           another_actual_uid.c_str(),
                           another_actual_token.c_str(),
                           added_card_cid.c_str()),
                    add_comment_request));
      EXPECT_EQ(200, static_cast<int>(post_comment_response.code));
      const auto add_comment_response = ParseResponse<AddCommentResponse>(post_comment_response.body);

      comment_to_be_notified_about_oid = add_comment_response.oid;
    }
    // Should be one notification.
    {
      current::time::SetNow(std::chrono::microseconds(112003 * 1000));
      const auto feed_http_response =
          HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&notifications_since=0",
                          FLAGS_api_port,
                          actual_uid.c_str(),
                          actual_token.c_str())));
      EXPECT_EQ(200, static_cast<int>(feed_http_response.code));
      const auto feed_response = ParseResponse<ResponseFeed>(feed_http_response.body);
      ASSERT_EQ(1u, feed_response.notifications.size());
      EXPECT_EQ("MyCardNewComment", feed_response.notifications[0].type);
      EXPECT_EQ(another_actual_uid, feed_response.notifications[0].uid);
      EXPECT_EQ(added_card_cid, feed_response.notifications[0].cid);
      EXPECT_EQ("Foo.", feed_response.notifications[0].card.text);
      EXPECT_EQ(comment_to_be_notified_about_oid, feed_response.notifications[0].oid);
      EXPECT_EQ("Ding!", feed_response.notifications[0].text);
      EXPECT_EQ(1u, feed_response.notifications[0].n);
      EXPECT_EQ("n05000000000112002000", feed_response.notifications[0].nid);
      EXPECT_EQ(112002u, feed_response.notifications[0].ms.count());
    }
    // Delete that comment.
    {
      const auto delete_comment_response =
          HTTP(DELETE(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s&oid=%s",
                             FLAGS_api_port,
                             another_actual_uid.c_str(),
                             another_actual_token.c_str(),
                             added_card_cid.c_str(),
                             comment_to_be_notified_about_oid.c_str())));
      EXPECT_EQ(200, static_cast<int>(delete_comment_response.code));
      ParseResponse<DeleteCommentResponse>(delete_comment_response.body);
    }
  }

  // Delete the remaining top-level comment, and confirm that the number of comments
  // goes down from 2 to 0, since deleting the top-level comments deletes its children.
  {
    current::time::SetNow(std::chrono::microseconds(601001 * 1000));
    const auto delete_comment_response =
        HTTP(DELETE(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s&oid=%s",
                           FLAGS_api_port,
                           actual_uid.c_str(),
                           actual_token.c_str(),
                           added_card_cid.c_str(),
                           added_second_comment_oid.c_str())));
    EXPECT_EQ(200, static_cast<int>(delete_comment_response.code));
    const auto payload = ParseResponse<DeleteCommentResponse>(delete_comment_response.body);
    EXPECT_EQ(601001u, payload.ms.count());

    EXPECT_EQ(
        0u,
        ParseResponse<ResponseMyCards>(HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                                       FLAGS_api_port,
                                                       actual_uid.c_str(),
                                                       actual_token.c_str()))).body)
            .cards[2]
            .number_of_comments);
  }

  // TODO(dkorolev): Test that I can only delete my own comments.

  // Test that deleting one card of mine reduces the number of my cards from three to two.
  {
    EXPECT_EQ(
        3u,
        ParseResponse<ResponseMyCards>(HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                                       FLAGS_api_port,
                                                       actual_uid.c_str(),
                                                       actual_token.c_str()))).body).cards.size());

    current::time::SetNow(std::chrono::microseconds(601501 * 1000));
    const auto unauthorized_delete_card_response =
        HTTP(DELETE(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s&cid=%s",
                           FLAGS_api_port,
                           another_actual_uid.c_str(),
                           another_actual_token.c_str(),
                           added_card_cid.c_str())));
    EXPECT_EQ(401, static_cast<int>(unauthorized_delete_card_response.code));
    EXPECT_EQ("NOT YOUR CARD BRO\n", unauthorized_delete_card_response.body);

    current::time::SetNow(std::chrono::microseconds(602001 * 1000));
    const auto delete_card_response = HTTP(DELETE(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s&cid=%s",
                                                         FLAGS_api_port,
                                                         actual_uid.c_str(),
                                                         actual_token.c_str(),
                                                         added_card_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(delete_card_response.code));
    const auto payload = ParseResponse<DeleteCardResponse>(delete_card_response.body);
    EXPECT_EQ(602001u, payload.ms.count());

    EXPECT_EQ(
        2u,
        ParseResponse<ResponseMyCards>(HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                                       FLAGS_api_port,
                                                       actual_uid.c_str(),
                                                       actual_token.c_str()))).body).cards.size());

    current::time::SetNow(std::chrono::microseconds(602501 * 1000));
    const auto nonexistent_card_delete_response =
        HTTP(DELETE(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s&cid=%s",
                           FLAGS_api_port,
                           actual_uid.c_str(),
                           actual_token.c_str(),
                           added_card_cid.c_str())));
    EXPECT_EQ(404, static_cast<int>(nonexistent_card_delete_response.code));
    EXPECT_EQ("NO SUCH CARD\n", nonexistent_card_delete_response.body);
  }

  // TODO(dkorolev): Test that I can only delete my own cards.

  // Confirm that the recent feed still contains two of my cards as the most recently added ones.
  {
    const auto feed_recent = ParseResponse<ResponseFeed>(
                                 HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&feed_count=2",
                                                 FLAGS_api_port,
                                                 actual_uid.c_str(),
                                                 actual_token.c_str()))).body).feed_recent;
    ASSERT_EQ(2u, feed_recent.size());
    EXPECT_EQ("Meh.", feed_recent[0].text);
    EXPECT_EQ("Bar.", feed_recent[1].text);
  }

  // Flag one of the cards.
  {
    iOSGenericEvent flag_card_event;
    flag_card_event.event = "FLAG_CARD";
    flag_card_event.fields["uid"] = actual_uid;
    flag_card_event.fields["token"] = actual_token;
    flag_card_event.fields["cid"] = added_card3_cid;
    const auto reponse =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port), flag_card_event));
    EXPECT_EQ(200, static_cast<int>(reponse.code));
    EXPECT_EQ("OK\n", reponse.body);
  }

  // Confirm that the flagged card is not returned as part of the feed.
  {
    const auto feed_recent = ParseResponse<ResponseFeed>(
                                 HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s&feed_count=1",
                                                 FLAGS_api_port,
                                                 actual_uid.c_str(),
                                                 actual_token.c_str()))).body).feed_recent;
    ASSERT_EQ(1u, feed_recent.size());
    EXPECT_EQ("Bar.", feed_recent[0].text);
  }
}

TEST(CTFO, StrictAuth) {
  const auto server_scope = SpawnTestServer("auth");

  const char* const some_uid = "u01000000000000000001";
  const char* const some_token = "t03000000000000000001";

  const char* const auth_id = "A_BUNCH_OF_DIGITS";
  const char* const auth_key = "1234567890abcdef";

  // Can't do a request without auth.
  const auto no_auth_feed_response =
      HTTP(GET(Printf("http://localhost:%d/ctfo/feed?uid=%s&token=%s", FLAGS_api_port, some_uid, some_token)));
  EXPECT_EQ(401, static_cast<int>(no_auth_feed_response.code));
  EXPECT_EQ("NEED VALID UID-TOKEN PAIR\n", no_auth_feed_response.body);

  // No device ID for `ios` auth.
  const auto no_device_id_auth_response =
      HTTP(POST(Printf("http://localhost:%d/ctfo/auth/ios", FLAGS_api_port), ""));
  EXPECT_EQ(400, static_cast<int>(no_device_id_auth_response.code));
  EXPECT_EQ("NEED VALID ID-KEY PAIR\n", no_device_id_auth_response.body);

  // Successful `ios` auth.
  const auto auth_http_response = HTTP(
      POST(Printf("http://localhost:%d/ctfo/auth/ios?id=%s&key=%s", FLAGS_api_port, auth_id, auth_key), ""));
  EXPECT_EQ(200, static_cast<int>(auth_http_response.code));
  const auto auth_response = ParseResponse<ResponseFeed>(auth_http_response.body);
  EXPECT_EQ(1u, auth_response.ms.count());
}

TEST(CTFO, UseRightHTTPVerbs) {
  const auto server_scope = SpawnTestServer("verbs");

  const auto post_feed = HTTP(POST(Printf("http://localhost:%d/ctfo/feed", FLAGS_api_port), ""));
  EXPECT_EQ(405, static_cast<int>(post_feed.code));
  EXPECT_EQ("METHOD NOT ALLOWED\n", post_feed.body);
}

TEST(CTFO, NotificationsSerializeWell) {
  const UID me = static_cast<UID>(42);
  const UID uid = static_cast<UID>(1);
  const CID cid = static_cast<CID>(2);
  const OID oid = static_cast<OID>(3);
  const Notification notification(
      me, std::chrono::microseconds(12345000ull), NotificationMyCardNewComment(uid, cid, oid, "foo"));
  const std::string user_facing_json = JSON(notification.BuildResponseNotification());
  EXPECT_EQ(
      "{\"data\":{\"nid\":\"n05000000000012345000\",\"type\":\"MyCardNewComment\",\"ms\":12345,\"uid\":"
      "\"u00000000000000000001\",\"cid\":\"c00000000000000000002\",\"oid\":\"o00000000000000000003\",\"text\":"
      "\"foo\",\"card\":{\"cid\":\"cINVALID\",\"author_uid\":\"uINVALID\",\"text\":\"\",\"ms\":0,"
      "\"color\":{\"red\":0,\"green\":0,\"blue\":0},\"relevance\":0,\"ctfo_score\":0,\"tfu_score\":0,\"ctfo_"
      "count\":0,\"tfu_count\":0,\"skip_count\":0,\"vote\":\"\",\"favorited\":false,\"is_my_card\":false,"
      "\"number_of_comments\":0},\"n\":1}}",
      user_facing_json);
  const std::string stream_stored_json = JSON(notification);
  EXPECT_EQ(
      "{\"data\":{\"uid\":42,\"timestamp\":{\"ms\":12345},\"notification\":{\"polymorphic_id\":2147483649,"
      "\"polymorphic_name\":\"NotificationMyCardNewComment\",\"ptr_wrapper\":{\"id\":2147483649,\"data\":{"
      "\"uid\":1,\"cid\":2,\"oid\":3,\"text\":\"foo\"}}}}}",
      stream_stored_json);
  EXPECT_EQ(stream_stored_json, JSON(ParseJSON<Notification>(stream_stored_json)));
}

// TODO(dkorolev): Test the remaining notification types.
