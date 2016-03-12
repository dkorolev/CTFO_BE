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

#include "new_schema_public.h"
#include "util.h"

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

namespace new_ctfo {

CURRENT_STRUCT(User) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_KEY(uid);
  CURRENT_FIELD(ms, std::chrono::milliseconds, 0);
  CURRENT_FIELD(level, uint8_t, 0u);   // User level [0, 9].
  CURRENT_FIELD(score, uint64_t, 0u);  // User score.

  CURRENT_DEFAULT_CONSTRUCTOR(User) {}

  void InitializeOwnKey() {}
};

// AuthKey structure defines generic authentication key.
// `key` is supposed to be a long string, combined from several parameters (depending on the authentication
// type) with "::" as a delimiter.
// For `type = AUTH_TYPE::IOS`:
//   key = "iOS::" + <Device ID> + "::" + <Application Key>
//   Application key is a random number, generated once in a lifetime in iOS application on its first launch.
CURRENT_STRUCT(AuthKey) {
  CURRENT_FIELD(key, std::string, "");
  CURRENT_FIELD(type, AUTH_TYPE, AUTH_TYPE::UNDEFINED);

  CURRENT_DEFAULT_CONSTRUCTOR(AuthKey) {}
  CURRENT_CONSTRUCTOR(AuthKey)(const std::string& key, AUTH_TYPE type) : key(key), type(type) {}

  // TODO(dkorolev): Revisit this. For now, I just assume all `key`-s are distinct.
  size_t Hash() const { return std::hash<std::string>()(key); }
  bool operator==(const AuthKey& rhs) const { return key == rhs.key && type == rhs.type; }
};

CURRENT_STRUCT(AuthKeyTokenPair) {
  CURRENT_FIELD(auth_key, AuthKey);
  CURRENT_USE_FIELD_AS_ROW(auth_key);
  CURRENT_FIELD(token, std::string, "");
  CURRENT_USE_FIELD_AS_COL(token);
  CURRENT_FIELD(valid, bool, false);

  CURRENT_DEFAULT_CONSTRUCTOR(AuthKeyTokenPair) {}
  CURRENT_CONSTRUCTOR(AuthKeyTokenPair)(const AuthKeyTokenPair& rhs)
      : auth_key(rhs.auth_key), token(rhs.token), valid(rhs.valid) {}
  CURRENT_CONSTRUCTOR(AuthKeyTokenPair)(const AuthKey& auth_key, const std::string& token, bool valid = false)
      : auth_key(auth_key), token(token), valid(valid) {}
};

CURRENT_STRUCT(AuthKeyUIDPair) {
  CURRENT_FIELD(auth_key, AuthKey);
  CURRENT_USE_FIELD_AS_ROW(auth_key);
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_COL(uid);

  CURRENT_DEFAULT_CONSTRUCTOR(AuthKeyUIDPair) {}
  CURRENT_CONSTRUCTOR(AuthKeyUIDPair)(const AuthKey& auth_key, const UID uid) : auth_key(auth_key), uid(uid) {}
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

CURRENT_STRUCT(CardAuthor) {
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);
  CURRENT_USE_FIELD_AS_ROW(cid);
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_COL(uid);

  CURRENT_DEFAULT_CONSTRUCTOR(CardAuthor) {}
  CURRENT_CONSTRUCTOR(CardAuthor)(CID cid, UID uid) : cid(cid), uid(uid) {}
};

CURRENT_STRUCT(Answer) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_ROW(uid);
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);
  CURRENT_USE_FIELD_AS_COL(cid);
  CURRENT_FIELD(answer, ANSWER, ANSWER::UNSEEN);

  CURRENT_DEFAULT_CONSTRUCTOR(Answer) {}
  CURRENT_CONSTRUCTOR(Answer)(UID uid, CID cid, ANSWER answer) : uid(uid), cid(cid), answer(answer) {}
};

CURRENT_STRUCT(Favorite) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_ROW(uid);
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);
  CURRENT_USE_FIELD_AS_COL(cid);
  CURRENT_FIELD(favorited, bool, false);

  CURRENT_DEFAULT_CONSTRUCTOR(Favorite) {}
  CURRENT_CONSTRUCTOR(Favorite)(const UID uid, const CID cid, bool favorited = false)
      : uid(uid), cid(cid), favorited(favorited) {}
};

CURRENT_STRUCT(Comment) {
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);
  CURRENT_USE_FIELD_AS_ROW(cid);
  CURRENT_FIELD(oid, OID, OID::INVALID_COMMENT);
  CURRENT_USE_FIELD_AS_COL(oid);
  CURRENT_FIELD(parent_oid,
                OID,
                OID::INVALID_COMMENT);  // `INVALID_COMMENT` for a top-level comment, parent OID otherwise.
  CURRENT_FIELD(author_uid, UID, UID::INVALID_USER);
  CURRENT_FIELD(text, std::string, "");

  CURRENT_DEFAULT_CONSTRUCTOR(Comment) {}
  CURRENT_CONSTRUCTOR(Comment)(
      const CID cid, const OID oid, const OID parent_oid, const UID author_uid, const std::string& text)
      : cid(cid), oid(oid), parent_oid(parent_oid), author_uid(author_uid), text(text) {}
};

CURRENT_STRUCT(CommentLike) {
  CURRENT_FIELD(oid, OID, OID::INVALID_COMMENT);
  CURRENT_USE_FIELD_AS_ROW(oid);
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_COL(uid);

  CURRENT_DEFAULT_CONSTRUCTOR(CommentLike) {}
};

CURRENT_STRUCT(CardFlagAsInappropriate) {
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);
  CURRENT_USE_FIELD_AS_ROW(cid);
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_COL(uid);

  CURRENT_DEFAULT_CONSTRUCTOR(CardFlagAsInappropriate) {}
};

CURRENT_STRUCT(CommentFlagAsInappropriate) {
  CURRENT_FIELD(oid, OID, OID::INVALID_COMMENT);
  CURRENT_USE_FIELD_AS_ROW(oid);
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_COL(uid);

  CURRENT_DEFAULT_CONSTRUCTOR(CommentFlagAsInappropriate) {}
};

// To enable reporting users.
CURRENT_STRUCT(UserReportedUser) {
  CURRENT_FIELD(who, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_ROW(who);
  CURRENT_FIELD(whom, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_COL(whom);

  CURRENT_DEFAULT_CONSTRUCTOR(UserReportedUser) {}
  CURRENT_CONSTRUCTOR(UserReportedUser)(UID who, UID whom) : who(who), whom(whom) {}
};

// To enable blocking users.
CURRENT_STRUCT(UserBlockedUser) {
  CURRENT_FIELD(who, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_ROW(who);
  CURRENT_FIELD(whom, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_COL(whom);

  CURRENT_DEFAULT_CONSTRUCTOR(UserBlockedUser) {}
  CURRENT_CONSTRUCTOR(UserBlockedUser)(UID who, UID whom) : who(who), whom(whom) {}
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

// Notifications.
CURRENT_STRUCT(AbstractNotification) {
  // Sadly, can't be pure virtual with `g++`. -- @dkorolev, @mzhurovich.
  virtual void PopulateResponseNotification(ResponseNotification&) const {}
  // To return full card bodies in the payload; can be CID::INVALID_CARD.
  virtual CID GetCID() const { return CID::INVALID_CARD; }
};

CURRENT_STRUCT(NotificationMyCardNewComment, AbstractNotification) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);     // Who left that comment.
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);     // To what card.
  CURRENT_FIELD(oid, OID, OID::INVALID_COMMENT);  // Which comment.
  CURRENT_FIELD(text, std::string, "");           // Text of the newly added comment.

  CURRENT_DEFAULT_CONSTRUCTOR(NotificationMyCardNewComment) {}
  CURRENT_CONSTRUCTOR(NotificationMyCardNewComment)(UID uid, CID cid, OID oid, std::string text)
      : uid(uid), cid(cid), oid(oid), text(text) {}
  CURRENT_CONSTRUCTOR(NotificationMyCardNewComment)(const Comment& comment)
      : uid(comment.author_uid), cid(comment.cid), oid(comment.oid), text(comment.text) {}

  void PopulateResponseNotification(ResponseNotification & output) const {
    output.type = "MyCardNewComment";
    output.uid = UIDToString(uid);
    output.cid = CIDToString(cid);
    output.oid = OIDToString(oid);
    output.text = text;
  }
  CID GetCID() const { return cid; }
};

CURRENT_STRUCT(NotificationNewReplyToMyComment, AbstractNotification) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);     // Who left that reply.
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);     // To what card.
  CURRENT_FIELD(oid, OID, OID::INVALID_COMMENT);  // Which comment.
  CURRENT_FIELD(text, std::string, "");           // Text of the newly added comment.

  CURRENT_DEFAULT_CONSTRUCTOR(NotificationNewReplyToMyComment) {}
  CURRENT_CONSTRUCTOR(NotificationNewReplyToMyComment)(const Comment& comment)
      : uid(comment.author_uid), cid(comment.cid), oid(comment.oid), text(comment.text) {}

  void PopulateResponseNotification(ResponseNotification & output) const {
    output.type = "NewReplyToMyComment";
    output.uid = UIDToString(uid);
    output.cid = CIDToString(cid);
    output.oid = OIDToString(oid);
    output.text = text;
  }
  CID GetCID() const { return cid; }
};

CURRENT_STRUCT(NotificationMyCommentLiked, AbstractNotification) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);     // Who liked my comment.
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);     // On what card.
  CURRENT_FIELD(oid, OID, OID::INVALID_COMMENT);  // Which comment of mine.
  CURRENT_FIELD(text, std::string, "");           // Text of that comment of mine.

  CURRENT_DEFAULT_CONSTRUCTOR(NotificationMyCommentLiked) {}
  CURRENT_CONSTRUCTOR(NotificationMyCommentLiked)(UID liker_uid, const Comment& comment)
      : uid(liker_uid), cid(comment.cid), oid(comment.oid), text(comment.text) {}

  void PopulateResponseNotification(ResponseNotification & output) const {
    output.type = "MyCommentLiked";
    output.uid = UIDToString(uid);
    output.cid = CIDToString(cid);
    output.oid = OIDToString(oid);
    output.text = text;
  }
  CID GetCID() const { return cid; }
};

CURRENT_STRUCT(NotificationNewCommentOnCardIStarred, AbstractNotification) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);     // Who left that comment.
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);     // On what card.
  CURRENT_FIELD(oid, OID, OID::INVALID_COMMENT);  // Which comment.
  CURRENT_FIELD(text, std::string, "");           // Text of the comment.
  CURRENT_DEFAULT_CONSTRUCTOR(NotificationNewCommentOnCardIStarred) {}
  CURRENT_CONSTRUCTOR(NotificationNewCommentOnCardIStarred)(UID new_comment_author_uid, const Comment& comment)
      : uid(new_comment_author_uid), cid(comment.cid), oid(comment.oid), text(comment.text) {}

  void PopulateResponseNotification(ResponseNotification & output) const {
    output.type = "NewCommentOnCardIStarred";
    output.uid = UIDToString(uid);
    output.cid = CIDToString(cid);
    output.oid = OIDToString(oid);
    output.text = text;
  }
  CID GetCID() const { return cid; }
};

CURRENT_STRUCT(NotificationMyCardStarred, AbstractNotification) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);  // Who starred my card.
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);  // Which card.

  CURRENT_DEFAULT_CONSTRUCTOR(NotificationMyCardStarred) {}
  CURRENT_CONSTRUCTOR(NotificationMyCardStarred)(UID starrer_uid, CID cid) : uid(starrer_uid), cid(cid) {}

  void PopulateResponseNotification(ResponseNotification & output) const {
    output.type = "MyCardStarred";
    output.uid = UIDToString(uid);
    output.cid = CIDToString(cid);
    output.oid = "";
    output.text = "";
  }
  CID GetCID() const { return cid; }
};

CURRENT_STRUCT(NotificationNewVotesOnMyCard, AbstractNotification) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);  // Who (grouped later at transmission phase).
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);  // On which card.

  CURRENT_DEFAULT_CONSTRUCTOR(NotificationNewVotesOnMyCard) {}
  CURRENT_CONSTRUCTOR(NotificationNewVotesOnMyCard)(UID uid, CID cid) : uid(uid), cid(cid) {}

  void PopulateResponseNotification(ResponseNotification & output) const {
    output.type = "NewVotesOnMyCard";
    output.cid = CIDToString(cid);
    output.uid = UIDToString(uid);
    output.oid = "";
    output.text = "";
  }
  CID GetCID() const { return cid; }
};

using T_NOTIFICATIONS_VARIANT = Variant<NotificationMyCardNewComment,
                                        NotificationNewReplyToMyComment,
                                        NotificationMyCommentLiked,
                                        NotificationNewCommentOnCardIStarred,
                                        NotificationMyCardStarred,
                                        NotificationNewVotesOnMyCard>;

CURRENT_STRUCT(Notification) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_ROW(uid);
  CURRENT_FIELD(timestamp, std::chrono::milliseconds);
  CURRENT_USE_FIELD_AS_COL(timestamp);
  CURRENT_FIELD(notification, T_NOTIFICATIONS_VARIANT);
};

}  // namespace new_ctfo

#endif  // NEW_SCHEMA_H
