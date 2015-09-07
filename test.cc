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

#define BRICKS_MOCK_TIME

#include "server.h"

#include "../Current/Bricks/file/file.h"

#include "../Current/Bricks/dflags/dflags.h"
#include "../Current/3rdparty/gtest/gtest-main-with-dflags.h"

CEREAL_REGISTER_TYPE(User);
CEREAL_REGISTER_TYPE(AuthKeyTokenPair);
CEREAL_REGISTER_TYPE(AuthKeyUIDPair);
CEREAL_REGISTER_TYPE(Card);
CEREAL_REGISTER_TYPE(CardAuthor);
CEREAL_REGISTER_TYPE(Answer);
CEREAL_REGISTER_TYPE(Favorite);
CEREAL_REGISTER_TYPE(Comment);

DEFINE_string(cards_file, "cards.json", "Cards data file in JSON format.");
DEFINE_int32(api_port, 8383, "Port to spawn CTFO RESTful server on.");
DEFINE_int32(event_log_port, 8384, "Port to spawn event collector on.");

// Uncomment the following line to have the unit test dump debug information to console.
// #define CTFO_DEBUG

std::unique_ptr<CTFOServer> SpawnTestServer(const std::string& suffix) {
#ifdef CTFO_DEBUG
  const std::string db_file = "unittest-db-" + suffix + ".log";
  const std::string log_file = "unittest-log-" + suffix + ".log";
  bricks::FileSystem::RmFile(db_file, bricks::FileSystem::RmFileParameters::Silent);
  bricks::FileSystem::RmFile(log_file, bricks::FileSystem::RmFileParameters::Silent);
#else
  static_cast<void>(suffix);

  const std::string db_file = bricks::FileSystem::GenTmpFileName();
  bricks::FileSystem::ScopedRmFile scoped_rm_db_file(db_file);

  const std::string log_file = bricks::FileSystem::GenTmpFileName();
  bricks::FileSystem::ScopedRmFile scoped_rm_log_file(log_file);
#endif

  bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(1));
  bricks::random::SetSeed(42);

  return make_unique<CTFOServer>(FLAGS_cards_file,
                                 FLAGS_api_port,
                                 db_file,
                                 FLAGS_event_log_port,
                                 log_file,
                                 static_cast<bricks::time::MILLISECONDS_INTERVAL>(100)
#ifdef CTFO_DEBUG
                                 ,
                                 true  // Debug print.
#endif
                                 );
}

// TODO(dkorolev): Strictly speaking, this test is flaky, since Midichlorians logs processing is async.
// TODO(dkorolev): Fix it.
TEST(CTFO, SmokeTest) {
  const auto server_scope = SpawnTestServer("smoke");

  // Get authentication `uid` and `token`.
  const char* const auth_id = "A_BUNCH_OF_DIGITS";
  const char* const auth_key = "1234567890abcdef";
  const auto auth_http_response = HTTP(
      POST(Printf("http://localhost:%d/ctfo/auth/ios?id=%s&key=%s", FLAGS_api_port, auth_id, auth_key), ""));
  EXPECT_EQ(200, static_cast<int>(auth_http_response.code));
  const auto auth_response = ParseJSON<ResponseFeed>(auth_http_response.body);
  EXPECT_EQ(1u, auth_response.ms);
  const std::string actual_uid = auth_response.user.uid;
  const std::string actual_token = auth_response.user.token;

  bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(1001));

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
    const auto feed_response = ParseJSON<ResponseFeed>(feed_http_response.body);
    EXPECT_EQ(1001u, feed_response.ms);
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
    }
    for (const ResponseCardEntry& card : feed_response.feed_recent) {
      recent_cids.insert(card.cid);
      recent_texts.insert(card.text);
      EXPECT_EQ(0u, card.ctfo_count);
      EXPECT_EQ(0u, card.tfu_count);
      EXPECT_EQ(0u, card.skip_count);
    }
    EXPECT_EQ(40u, hot_cids.size());
    EXPECT_EQ(40u, hot_texts.size());
    EXPECT_EQ(40u, recent_cids.size());
    EXPECT_EQ(40u, recent_texts.size());

    std::vector<std::string> cids_intersection;
    std::set_intersection(hot_cids.begin(),
                          hot_cids.end(),
                          recent_cids.begin(),
                          recent_cids.end(),
                          std::back_inserter(cids_intersection));
    EXPECT_EQ(0u, cids_intersection.size());
    std::vector<std::string> texts_intersection;
    std::set_intersection(hot_texts.begin(),
                          hot_texts.end(),
                          recent_texts.begin(),
                          recent_texts.end(),
                          std::back_inserter(texts_intersection));
    EXPECT_EQ(0u, texts_intersection.size());
  }

  // Add two cards to favorites.
  iOSGenericEvent favorite_event;
  std::string cid1;
  std::string cid2;

  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(10001));
    cid1 = *hot_cids.begin();
    cid2 = *(++hot_cids.begin());
    favorite_event.event = "FAV";
    favorite_event.fields["uid"] = actual_uid;
    favorite_event.fields["cid"] = cid1;
    favorite_event.fields["token"] = actual_token;
    const auto post_favorite_response_1 =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port),
                  WithBaseType<MidichloriansEvent>(favorite_event)));
    EXPECT_EQ(200, static_cast<int>(post_favorite_response_1.code));
    EXPECT_EQ("OK\n", post_favorite_response_1.body);

    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(10002));
    favorite_event.fields["cid"] = cid2;
    const auto post_favorite_response_2 =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port),
                  WithBaseType<MidichloriansEvent>(favorite_event)));
    EXPECT_EQ(200, static_cast<int>(post_favorite_response_2.code));
    EXPECT_EQ("OK\n", post_favorite_response_2.body);
  }

  // Attempt to add a non-existing card to favorites.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(10003));
    favorite_event.fields["cid"] = CIDToString(static_cast<CID>(987654321));  // A non-existent ID.
    const auto post_favorite_response_3 =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port),
                  WithBaseType<MidichloriansEvent>(favorite_event)));
    EXPECT_EQ(200, static_cast<int>(post_favorite_response_3.code));
    EXPECT_EQ("OK\n", post_favorite_response_3.body);
  }

  // Confirm both are returned as favorites for this user.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(11001));
    const auto feed_with_2_favs_response = HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                                                           FLAGS_api_port,
                                                           actual_uid.c_str(),
                                                           actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(feed_with_2_favs_response.code));
    const auto two_favs_response = ParseJSON<ResponseFavs>(feed_with_2_favs_response.body);
    EXPECT_EQ(11001u, two_favs_response.ms);
    EXPECT_EQ(actual_uid, two_favs_response.user.uid);
    ASSERT_EQ(2u, two_favs_response.cards.size());
    EXPECT_EQ(cid2, two_favs_response.cards[0].cid);
    EXPECT_EQ(cid1, two_favs_response.cards[1].cid);
  }

  // Unfavorite one card.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(12001));
    favorite_event.event = "UNFAV";
    favorite_event.fields["cid"] = cid1;
    const auto post_unfavorite_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port),
                  WithBaseType<MidichloriansEvent>(favorite_event)));
    EXPECT_EQ(200, static_cast<int>(post_unfavorite_response.code));
    EXPECT_EQ("OK\n", post_unfavorite_response.body);
  }

  // Confirm the remaining one is returned as a favorite for this user.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(13001));
    const auto feed_with_1_fav_response = HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                                                          FLAGS_api_port,
                                                          actual_uid.c_str(),
                                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(feed_with_1_fav_response.code));
    const auto one_fav_response = ParseJSON<ResponseFavs>(feed_with_1_fav_response.body);
    EXPECT_EQ(13001u, one_fav_response.ms);
    EXPECT_EQ(actual_uid, one_fav_response.user.uid);
    ASSERT_EQ(1u, one_fav_response.cards.size());
    EXPECT_EQ(cid2, one_fav_response.cards[0].cid);
    EXPECT_EQ("", one_fav_response.cards[0].vote);
    EXPECT_EQ(0u, one_fav_response.cards[0].ctfo_count);
    EXPECT_EQ(0u, one_fav_response.cards[0].tfu_count);
    EXPECT_EQ(0u, one_fav_response.cards[0].skip_count);
  }

  // Cast a vote on this card.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(14001));
    iOSGenericEvent ctfo_event;
    ctfo_event.event = "CTFO";
    ctfo_event.fields["uid"] = actual_uid;
    ctfo_event.fields["cid"] = cid2;
    ctfo_event.fields["token"] = actual_token;
    const auto post_ctfo_response = HTTP(POST(Printf("http://localhost:%d/ctfo/log", FLAGS_event_log_port),
                                              WithBaseType<MidichloriansEvent>(ctfo_event)));
    EXPECT_EQ(200, static_cast<int>(post_ctfo_response.code));
    EXPECT_EQ("OK\n", post_ctfo_response.body);
  }

  // Confirm the card has one "CTFO" vote.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(15001));
    const auto feed_with_ctfo_cast_on_fav = HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                                                            FLAGS_api_port,
                                                            actual_uid.c_str(),
                                                            actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(feed_with_ctfo_cast_on_fav.code));
    const auto ctfo_vote_cast_response = ParseJSON<ResponseFavs>(feed_with_ctfo_cast_on_fav.body);
    EXPECT_EQ(15001u, ctfo_vote_cast_response.ms);
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
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(16001));
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
    const auto add_card_response = ParseJSON<AddCardResponse>(post_card_response.body);
    EXPECT_EQ(16001u, add_card_response.ms);

    added_card_cid = add_card_response.cid;
  }

  // Confirm this new card is not favorited by default.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(17001));
    const auto favs_including_my_card_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/favs?uid=%s&token=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(favs_including_my_card_response.code));
    const auto my_card_fav_response = ParseJSON<ResponseFavs>(favs_including_my_card_response.body);
    EXPECT_EQ(17001u, my_card_fav_response.ms);
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
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(18001));
    const auto my_cards = HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                          FLAGS_api_port,
                                          actual_uid.c_str(),
                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(my_cards.code));
    const auto my_cards_response = ParseJSON<ResponseMyCards>(my_cards.body);

    EXPECT_EQ(18001u, my_cards_response.ms);
    EXPECT_EQ(actual_uid, my_cards_response.user.uid);
    ASSERT_EQ(1u, my_cards_response.cards.size());
    EXPECT_EQ("Foo.", my_cards_response.cards[0].text);
    EXPECT_EQ(16001u, my_cards_response.cards[0].ms);
  }

  // Add a second card, with full JSON body, specifying the color explicitly.
  std::string added_card2_cid;
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(19001));
    const auto post_card_response =
        HTTP(POST(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s",
                         FLAGS_api_port,
                         actual_uid.c_str(),
                         actual_token.c_str()),
                  "{\"card\":{\"text\":\"Bar.\",\"color\":{\"red\":100,\"green\":101,\"blue\":102}}}"));
    EXPECT_EQ(200, static_cast<int>(post_card_response.code));
    const auto add_card_response = ParseJSON<AddCardResponse>(post_card_response.body);
    EXPECT_EQ(19001u, add_card_response.ms);

    added_card2_cid = add_card_response.cid;
  }

  // Get a list of my cards, should be two.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(20001));
    const auto my_cards = HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                          FLAGS_api_port,
                                          actual_uid.c_str(),
                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(my_cards.code));
    const auto my_cards_response = ParseJSON<ResponseMyCards>(my_cards.body);

    EXPECT_EQ(20001u, my_cards_response.ms);
    EXPECT_EQ(actual_uid, my_cards_response.user.uid);
    ASSERT_EQ(2u, my_cards_response.cards.size());
    EXPECT_EQ("Bar.", my_cards_response.cards[0].text);
    EXPECT_EQ(19001u, my_cards_response.cards[0].ms);
    EXPECT_EQ("Foo.", my_cards_response.cards[1].text);
    EXPECT_EQ(16001u, my_cards_response.cards[1].ms);
  }

  // Add a third card, not specifying color.
  std::string added_card3_cid;
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(21001));
    const auto post_card_response = HTTP(POST(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s",
                                                     FLAGS_api_port,
                                                     actual_uid.c_str(),
                                                     actual_token.c_str()),
                                              "{\"card\":{\"text\":\"Meh.\"}}"));
    EXPECT_EQ(200, static_cast<int>(post_card_response.code));
    const auto add_card_response = ParseJSON<AddCardResponse>(post_card_response.body);
    EXPECT_EQ(21001u, add_card_response.ms);

    added_card3_cid = add_card_response.cid;
  }

  // Attempt to add a card but send it a malformed BODY.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(22001));
    const auto post_card_response = HTTP(POST(Printf("http://localhost:%d/ctfo/card?uid=%s&token=%s",
                                                     FLAGS_api_port,
                                                     actual_uid.c_str(),
                                                     actual_token.c_str()),
                                              "This card shall not pass."));
    EXPECT_EQ(400, static_cast<int>(post_card_response.code));
    EXPECT_EQ("NEED VALID BODY\n", post_card_response.body);
  }

  // Get a list of my cards, should be three.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(23001));
    const auto my_cards = HTTP(GET(Printf("http://localhost:%d/ctfo/my_cards?uid=%s&token=%s",
                                          FLAGS_api_port,
                                          actual_uid.c_str(),
                                          actual_token.c_str())));
    EXPECT_EQ(200, static_cast<int>(my_cards.code));
    const auto my_cards_response = ParseJSON<ResponseMyCards>(my_cards.body);

    EXPECT_EQ(23001u, my_cards_response.ms);
    EXPECT_EQ(actual_uid, my_cards_response.user.uid);
    ASSERT_EQ(3u, my_cards_response.cards.size());
    EXPECT_EQ("Meh.", my_cards_response.cards[0].text);
    EXPECT_EQ(21001u, my_cards_response.cards[0].ms);
    EXPECT_EQ("Bar.", my_cards_response.cards[1].text);
    EXPECT_EQ(19001u, my_cards_response.cards[1].ms);
    EXPECT_EQ("Foo.", my_cards_response.cards[2].text);
    EXPECT_EQ(16001u, my_cards_response.cards[2].ms);
  }

  // Get comments for a non-exising card, expecting an error.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(100001));
    const auto get_comments_response = HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=",
                                                       FLAGS_api_port,
                                                       actual_uid.c_str(),
                                                       actual_token.c_str(),
                                                       CIDToString(RandomCID()).c_str())));
    EXPECT_EQ(400, static_cast<int>(get_comments_response.code));
    EXPECT_EQ("NEED VALID CID\n", get_comments_response.body);
  }

  // Get comments for the actual card, expecting valid response with no comments.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(101001));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        added_card2_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(get_comments_response.code));
    const auto response = ParseJSON<ResponseComments>(get_comments_response.body);
    EXPECT_EQ(101001u, response.ms);
    EXPECT_EQ(0u, response.comments.size());
  }

  // Add a top-level comment.
  std::string added_comment_oid;
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(102001));
    AddCommentRequest add_comment_request;
    add_comment_request.text = "Meh.";
    const auto post_comment_response = HTTP(POST(Printf("http://localhost:%d/ctfo/comment?uid=%s&token=%s&cid=%s",
                                                     FLAGS_api_port,
                                                     actual_uid.c_str(),
                                                     actual_token.c_str(),
                                                     added_card_cid.c_str()),
                                              add_comment_request));
    EXPECT_EQ(200, static_cast<int>(post_comment_response.code));
    const auto add_comment_response = ParseJSON<AddCommentResponse>(post_comment_response.body);
    EXPECT_EQ(102001u, add_comment_response.ms);

    added_comment_oid = add_comment_response.oid;
  }

  // Get comments for the card where the comment was added, expecting one.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(103001));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        added_card_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(get_comments_response.code));
    const auto response = ParseJSON<ResponseComments>(get_comments_response.body);
    EXPECT_EQ(103001u, response.ms);
    ASSERT_EQ(1u, response.comments.size());
    EXPECT_EQ(added_comment_oid, response.comments[0].oid);
    EXPECT_EQ("", response.comments[0].parent_oid);
    EXPECT_EQ(actual_uid, response.comments[0].author_uid);
    EXPECT_EQ("Meh.", response.comments[0].text);
    EXPECT_EQ(102001u, response.comments[0].ms);
  }

  // Get comments for the card where the comment was not added, expecting none.
  {
    bricks::time::SetNow(static_cast<bricks::time::EPOCH_MILLISECONDS>(104001));
    const auto get_comments_response =
        HTTP(GET(Printf("http://localhost:%d/ctfo/comments?uid=%s&token=%s&cid=%s",
                        FLAGS_api_port,
                        actual_uid.c_str(),
                        actual_token.c_str(),
                        added_card2_cid.c_str())));
    EXPECT_EQ(200, static_cast<int>(get_comments_response.code));
    const auto response = ParseJSON<ResponseComments>(get_comments_response.body);
    EXPECT_EQ(104001u, response.ms);
    EXPECT_EQ(0u, response.comments.size());
  }

  // Add another top-level comment.

  // Get comments, expecting two top-level comments.

  // Add a 2nd level comment.

  // Get comments, expecting two top-level ones, and one 2nd level one.

  // Attempt to add a 3rd level comment, expecting an error.
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
  const auto auth_response = ParseJSON<ResponseFeed>(auth_http_response.body);
  EXPECT_EQ(1u, auth_response.ms);
}

TEST(CTFO, UseRightHTTPVerbs) {
  const auto server_scope = SpawnTestServer("verbs");

  const auto post_feed = HTTP(POST(Printf("http://localhost:%d/ctfo/feed", FLAGS_api_port), ""));
  EXPECT_EQ(405, static_cast<int>(post_feed.code));
  EXPECT_EQ("METHOD NOT ALLOWED\n", post_feed.body);
}
