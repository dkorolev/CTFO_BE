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

#ifndef CTFO_SCHEMA_H
#define CTFO_SCHEMA_H

#include <vector>

#include "schema_public.h"
#include "util.h"

namespace CTFO {

// Constants.
const std::vector<unsigned int> LEVEL_SCORES{
    0,        // "Fish"
    15000,    // "Turkey"
    30000,    // "Rat"
    60000,    // "Pig"
    120000,   // "Octopus"
    240000,   // "Raven"
    480000,   // "Dolphin"
    960000,   // "Elephant"
    1920000,  // "Chimp"
    3840000   // "Skrik"
};

const std::vector<Color> CARD_COLORS{{0x0A, 0xB2, 0xCB},
                                     {0x0A, 0xCB, 0xCB},
                                     {0x0A, 0xCB, 0xAB},
                                     {0x0B, 0xE6, 0x9D},
                                     {0x0B, 0xBA, 0xE6},
                                     {0x0B, 0x95, 0xE6},
                                     {0x0F, 0xCE, 0xB9},
                                     {0x10, 0xDE, 0x81},
                                     {0x10, 0xDE, 0x96},
                                     {0x11, 0xE8, 0x79},
                                     {0x31, 0xDE, 0x83},
                                     {0x4C, 0xED, 0x61},
                                     {0x1F, 0xCD, 0x85},
                                     {0x8F, 0xD6, 0x24},
                                     {0x48, 0xD6, 0x24},
                                     {0x42, 0xDE, 0x1B},
                                     {0x1B, 0xDE, 0x42},
                                     {0x27, 0xE3, 0x7B},
                                     {0x6E, 0xD9, 0x3A},
                                     {0x78, 0xD6, 0x2F},
                                     {0x84, 0xD1, 0x2C},
                                     {0xA9, 0xDE, 0x37},
                                     {0xB0, 0xDB, 0x39},
                                     {0xC1, 0xE3, 0x39},
                                     {0xEC, 0xDE, 0x00},
                                     {0xED, 0xE9, 0x09},
                                     {0xED, 0xDE, 0x11},
                                     {0xF2, 0xF2, 0x00},
                                     {0xF5, 0xE9, 0x00},
                                     {0xFC, 0xE7, 0x00},
                                     {0xEC, 0x60, 0x3F},
                                     {0xF0, 0x6E, 0x43},
                                     {0xFA, 0x6E, 0x4B},
                                     {0xFF, 0x81, 0x69},
                                     {0xFF, 0xA6, 0x91},
                                     {0xFF, 0x99, 0x85},
                                     {0xD2, 0x37, 0x67},
                                     {0xE6, 0x30, 0x68},
                                     {0xF5, 0x42, 0x79},
                                     {0xE3, 0x5B, 0x85},
                                     {0xF2, 0x8F, 0xAE},
                                     {0xFF, 0x3B, 0x77},
                                     {0xB8, 0x38, 0x7D},
                                     {0xEB, 0x38, 0x9A},
                                     {0xFA, 0x57, 0xB1},
                                     {0xFF, 0x40, 0xA9},
                                     {0xED, 0x1F, 0x90},
                                     {0xFF, 0x08, 0x90},
                                     {0xA0, 0x4F, 0xDE},
                                     {0xB9, 0x64, 0xFA},
                                     {0xB6, 0x69, 0xFF},
                                     {0xCA, 0x91, 0xFF},
                                     {0xC4, 0x9E, 0xE8},
                                     {0xB3, 0x97, 0xDE},
                                     {0x74, 0x4A, 0xCA},
                                     {0x8B, 0x5D, 0xD9},
                                     {0x9D, 0x73, 0xE6},
                                     {0xA6, 0x79, 0xF2},
                                     {0xB3, 0x88, 0xFC},
                                     {0x98, 0x88, 0xFC},
                                     {0x4C, 0x5C, 0xC0},
                                     {0x55, 0x66, 0xD4},
                                     {0x63, 0x82, 0xEB},
                                     {0x67, 0x79, 0xEB},
                                     {0x72, 0x84, 0xF2},
                                     {0x83, 0x93, 0xF7},
                                     {0x2B, 0x73, 0xDB},
                                     {0x2B, 0x7A, 0xED},
                                     {0x38, 0x85, 0xF5},
                                     {0x4B, 0x93, 0xFA},
                                     {0x60, 0x99, 0xEB},
                                     {0x7D, 0xA9, 0xE8},
                                     {0xC9, 0x49, 0xC7},
                                     {0xF0, 0x46, 0xED},
                                     {0xF0, 0x5D, 0xED},
                                     {0xDE, 0x6F, 0xDC},
                                     {0xE8, 0x87, 0xE7},
                                     {0xCC, 0x81, 0xCB},
                                     {0x3E, 0xEE, 0x3E},
                                     {0x43, 0xDE, 0x43},
                                     {0x37, 0xD4, 0x37},
                                     {0x6A, 0xF7, 0x6A},
                                     {0x6A, 0xE6, 0x6A},
                                     {0x79, 0xED, 0x8E},
                                     {0xEC, 0x3F, 0x3F},
                                     {0xFC, 0x44, 0x44},
                                     {0xFC, 0x35, 0x35},
                                     {0xEB, 0x10, 0x10},
                                     {0xE3, 0x09, 0x09},
                                     {0xF2, 0x00, 0x00},
                                     {0x02, 0x98, 0xE4},
                                     {0x00, 0xA6, 0xFA},
                                     {0x21, 0xB4, 0xFF},
                                     {0x3D, 0xB8, 0xFF},
                                     {0x49, 0xA2, 0xD6},
                                     {0x54, 0xA0, 0xCC},
                                     {0xFF, 0x5E, 0x9A},
                                     {0xFF, 0x7A, 0x9C},
                                     {0xFF, 0x8C, 0xA9},
                                     {0xED, 0x98, 0xAD},
                                     {0xE3, 0xA8, 0xB7},
                                     {0xF0, 0xBD, 0xCA},
                                     {0xC2, 0xEC, 0x00},
                                     {0xB8, 0xDB, 0x16},
                                     {0xC8, 0xED, 0x1F},
                                     {0xAC, 0xED, 0x1F},
                                     {0xB5, 0xF2, 0x30},
                                     {0xAB, 0xE3, 0x32},
                                     {0xF2, 0x81, 0x01},
                                     {0xFF, 0x87, 0x00},
                                     {0xFF, 0x9D, 0x00},
                                     {0xFF, 0xAC, 0x26},
                                     {0xF0, 0xB2, 0x4F},
                                     {0xF5, 0xC6, 0x7A}};

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

  const std::string& ToString() const { return key; }

  void FromString(const std::string& s) {
    key = s;
    type = AUTH_TYPE::UNDEFINED;  // TODO(dkorolev): Wrong, just to have this REST compile. -- D.K.
  }
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

CURRENT_STRUCT(UIDAuthKeyPair) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_ROW(uid);
  CURRENT_FIELD(auth_key, AuthKey);
  CURRENT_USE_FIELD_AS_COL(auth_key);

  CURRENT_DEFAULT_CONSTRUCTOR(UIDAuthKeyPair) {}
  CURRENT_CONSTRUCTOR(UIDAuthKeyPair)(const UID uid, const AuthKey& auth_key) : uid(uid), auth_key(auth_key) {}
};

CURRENT_STRUCT(Card) {
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);
  CURRENT_USE_FIELD_AS_KEY(cid);
  // CURRENT_FIELD(ms, std::chrono::milliseconds, 0);
  CURRENT_FIELD(ms, uint64_t, 0);  // To have REST compile, `{To/From}String`. -- D.K.
  CURRENT_FIELD(text, std::string);
  CURRENT_FIELD(color, Color);
  CURRENT_FIELD(ctfo_count, uint32_t, 0u);    // Number of users, who said "CTFO" on this card.
  CURRENT_FIELD(tfu_count, uint32_t, 0u);     // Number of users, who said "TFU" on this card.
  CURRENT_FIELD(skip_count, uint32_t, 0u);    // Number of users, who said "SKIP" on this card.
  CURRENT_FIELD(startup_index, double, 0.0);  // Cards with `startup_index != 0` will be on the top.

  CURRENT_DEFAULT_CONSTRUCTOR(Card) {}
  CURRENT_CONSTRUCTOR(Card)(CID cid, const std::string& text, const Color& color)
      : cid(cid), text(text), color(color) {}

  void InitializeOwnKey() {}
};

CURRENT_STRUCT(AuthorCard) {
  CURRENT_FIELD(uid, UID, UID::INVALID_USER);
  CURRENT_USE_FIELD_AS_ROW(uid);
  CURRENT_FIELD(cid, CID, CID::INVALID_CARD);
  CURRENT_USE_FIELD_AS_COL(cid);
  CURRENT_FIELD(ms, std::chrono::milliseconds, 0);

  CURRENT_DEFAULT_CONSTRUCTOR(AuthorCard) {}
  CURRENT_CONSTRUCTOR(AuthorCard)(UID uid, CID cid) : uid(uid), cid(cid) {}
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
  CURRENT_FIELD(ms, std::chrono::milliseconds, 0);
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
CURRENT_STRUCT(AbstractNotification){// Sadly, can't be pure virtual with `g++`. -- @dkorolev, @mzhurovich.
                                     virtual void PopulateResponseNotification(ResponseNotification&)const {}
                                     // To return full card bodies in the payload; can be CID::INVALID_CARD.
                                     virtual CID GetCID() const {return CID::INVALID_CARD;
}
}
;

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
  // CURRENT_FIELD(timestamp, std::chrono::milliseconds);
  CURRENT_FIELD(timestamp, uint64_t, 0);  // To have REST compile, `{To/From}String`. -- D.K.
  CURRENT_USE_FIELD_AS_COL(timestamp);
  CURRENT_FIELD(notification, T_NOTIFICATIONS_VARIANT);
};

}  // namespace CTFO

#endif  // CTFO_SCHEMA_H
