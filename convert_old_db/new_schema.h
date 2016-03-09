/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Dmitry "Dima" Korolev <dmitry.korolev@gmail.com>

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

#ifndef NEW_SCHEMA_H
#define NEW_SCHEMA_H

#include "new_schema_base.h"

#if 0
// Original schema:
OrderedDictionary<User, POLICY> users{"users", instance};
OrderedDictionary<Card, POLICY> cards{"cards", instance};
LightweightMatrix<AuthKeyTokenPair, POLICY> auth_token{"auth_token", instance};
LightweightMatrix<AuthKeyUIDPair, POLICY> auth_uid{"auth_uid", instance};
LightweightMatrix<CardAuthor, POLICY> card_authors{"card_authors", instance};
LightweightMatrix<Answer, POLICY> answers{"answers", instance};
LightweightMatrix<Favorite, POLICY> favorites{"favorites", instance};
LightweightMatrix<Comment, POLICY> comments{"comments", instance};
LightweightMatrix<CommentLike, POLICY> comment_likes{"comment_likes", instance};
LightweightMatrix<CardFlagAsInappropriate, POLICY> flagged_cards{"flagged_cards", instance};
LightweightMatrix<CommentFlagAsInappropriate, POLICY> flagged_comments{"flagged_comments", instance};
LightweightMatrix<Notification, POLICY> notifications{"notifications", instance};
OrderedDictionary<StarNotificationAlreadySent, POLICY> starred_notification_already_sent{
    "starred_notification_already_sent", instance};
LightweightMatrix<UserReportedUser, POLICY> user_reported_user{"user_reported_user", instance};
LightweightMatrix<UserBlockedUser, POLICY> user_blocked_user{"user_blocked_user", instance};
OrderedDictionary<BannedUser, POLICY> banned_users{"banned_users", instance};
#endif

CURRENT_STRUCT(User) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_KEY(uid);
  CURRENT_FIELD(ms, std::chrono::milliseconds, 0);
  CURRENT_FIELD(level, uint8_t, 0u);           // User level [0, 9].
  CURRENT_FIELD(score, uint64_t, score = 0u);  // User score.

  CURRENT_DEFAULT_CONSTRUCTOR(User) {}

  void InitializeOwnKey() {}
};

CURRENT_STRUCT(Color) {
  CURRENT_FIELD(red, uint8_t);
  CURRENT_FIELD(green, uint8_t);
  CURRENT_FIELD(blue, uint8_t);

  CURRENT_DEFAULT_CONSTRUCTOR(Color) : red(0u), green(0u), blue(0u) {}
  CURRENT_CONSTRUCTOR(Color)(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
};

CURRENT_STRUCT(Card) {
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);
  CURRENT_USE_FIELD_AS_KEY(cid);
  CURRENT_FIELD(ms, std::chrono::milliseconds, 0);
  CURRENT_FIELD(text, std::string);
  CURRENT_FIELD(color, Color);
  CURRENT_FIELD(ctfo_count, uint32_t, 0u);    // Number of users, who said "CTFO" on this card.
  CURRENT_FIELD(tfu_count, uint32_t, 0u);     // Number of users, who said "TFU" on this card.
  CURRENT_FIELD(skip_count, uint32_t, 0u);    // Number of users, who said "SKIP" on this card.
  CURRENT_FIELD(startup_index, double, 0.0);  // Cards with `startup_index != 0` will be on the top.

  CURRENT_DEFAULT_CONSTRUCTOR(Card) {}

  void InitializeOwnKey() {}
};

// To make sure the "star-unstar-repeat" actions sequence only sends the notification once.
CURRENT_STRUCT(StarNotificationAlreadySent) {
  CURRENT_FIELD(key, UIDAndCID, (UID::INVALID_USER, CID::INVALID_CARD));

  CURRENT_DEFAULT_CONSTRUCTOR(StarNotificationAlreadySent) {}
  CURRENT_CONSTRUCTOR(StarNotificationAlreadySent)(const UIDAndCID& key) : key(key) {}

  void InitializeOwnKey() {}
};

// To enable banned users support.
CURRENT_STRUCT(BannedUser) {
  CURRENT_FIELD(banned_uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_KEY(banned_uid);

  CURRENT_DEFAULT_CONSTRUCTOR(BannedUser) {}
  CURRENT_CONSTRUCTOR(BannedUser)(const UID uid) : banned_uid(uid) {}

  void InitializeOwnKey() {}
};

#endif  // NEW_SCHEMA_H
