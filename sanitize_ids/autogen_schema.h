// The `current.h` file is the one from `https://github.com/C5T/Current`.
// Compile with `-std=c++11` or higher.

#ifndef CURRENT_USERSPACE_3B3BD18430C5A642
#define CURRENT_USERSPACE_3B3BD18430C5A642

#include "current.h"

// clang-format off

namespace current_userspace_3b3bd18430c5a642 {

CURRENT_STRUCT(TransactionMeta) {
  CURRENT_FIELD(timestamp, std::chrono::microseconds);
  CURRENT_FIELD(fields, (std::map<std::string, std::string>));
};
using T9201222851776140948 = TransactionMeta;

CURRENT_ENUM(UID, uint64_t) {};
using T9010000003309061260 = UID;

CURRENT_STRUCT(User) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(us, std::chrono::microseconds);
  CURRENT_FIELD(level, uint8_t);
  CURRENT_FIELD(score, uint64_t);
};
using T9202868187541756328 = User;

CURRENT_STRUCT(Persisted_UserUpdated) {
  CURRENT_FIELD(data, User);
};
using T9207818145033697791 = Persisted_UserUpdated;

CURRENT_ENUM(AUTH_TYPE, uint8_t) {};
using T9010000000670795382 = AUTH_TYPE;

CURRENT_STRUCT(AuthKey) {
  CURRENT_FIELD(key, std::string);
  CURRENT_FIELD(type, AUTH_TYPE);
};
using T9207012534981795218 = AuthKey;

CURRENT_STRUCT(AuthKeyTokenPair) {
  CURRENT_FIELD(auth_key, AuthKey);
  CURRENT_FIELD(token, std::string);
  CURRENT_FIELD(valid, bool);
};
using T9203642333820785279 = AuthKeyTokenPair;

CURRENT_STRUCT(Persisted_AuthKeyTokenPairUpdated) {
  CURRENT_FIELD(data, AuthKeyTokenPair);
};
using T9205410150733686162 = Persisted_AuthKeyTokenPairUpdated;

CURRENT_STRUCT(UIDAuthKeyPair) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(auth_key, AuthKey);
};
using T9201002821766233156 = UIDAuthKeyPair;

CURRENT_STRUCT(Persisted_UIDAuthKeyPairUpdated) {
  CURRENT_FIELD(data, UIDAuthKeyPair);
};
using T9203716786711585009 = Persisted_UIDAuthKeyPairUpdated;

CURRENT_ENUM(CID, uint64_t) {};
using T9010000003717700430 = CID;

CURRENT_STRUCT(Color) {
  CURRENT_FIELD(red, uint8_t);
  CURRENT_FIELD(green, uint8_t);
  CURRENT_FIELD(blue, uint8_t);
};
using T9204213868853430923 = Color;

CURRENT_STRUCT(Card) {
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(us, std::chrono::microseconds);
  CURRENT_FIELD(text, std::string);
  CURRENT_FIELD(color, Color);
  CURRENT_FIELD(ctfo_count, uint32_t);
  CURRENT_FIELD(tfu_count, uint32_t);
  CURRENT_FIELD(skip_count, uint32_t);
  CURRENT_FIELD(startup_index, double);
};
using T9203519592729350886 = Card;

CURRENT_STRUCT(Persisted_CardUpdated) {
  CURRENT_FIELD(data, Card);
};
using T9201095196899897770 = Persisted_CardUpdated;

CURRENT_STRUCT(AuthorCard) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(us, std::chrono::microseconds);
};
using T9200460999453184076 = AuthorCard;

CURRENT_STRUCT(Persisted_AuthorCardUpdated) {
  CURRENT_FIELD(data, AuthorCard);
};
using T9205006642177850523 = Persisted_AuthorCardUpdated;

CURRENT_ENUM(ANSWER, int8_t) {};
using T9010000000707104163 = ANSWER;

CURRENT_STRUCT(Answer) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(answer, ANSWER);
};
using T9206510697173592424 = Answer;

CURRENT_STRUCT(Persisted_AnswerUpdated) {
  CURRENT_FIELD(data, Answer);
};
using T9205054323780466699 = Persisted_AnswerUpdated;

CURRENT_STRUCT(Favorite) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(us, std::chrono::microseconds);
  CURRENT_FIELD(favorited, bool);
};
using T9200644221968698350 = Favorite;

CURRENT_STRUCT(Persisted_FavoriteUpdated) {
  CURRENT_FIELD(data, Favorite);
};
using T9205444805064299357 = Persisted_FavoriteUpdated;

CURRENT_ENUM(OID, uint64_t) {};
using T9010000003566035498 = OID;

CURRENT_STRUCT(Comment) {
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(oid, OID);
  CURRENT_FIELD(us, std::chrono::microseconds);
  CURRENT_FIELD(parent_oid, OID);
  CURRENT_FIELD(author_uid, UID);
  CURRENT_FIELD(text, std::string);
};
using T9203629003584948940 = Comment;

CURRENT_STRUCT(Persisted_CommentUpdated) {
  CURRENT_FIELD(data, Comment);
};
using T9202276132896903463 = Persisted_CommentUpdated;

CURRENT_STRUCT(CommentLike) {
  CURRENT_FIELD(oid, OID);
  CURRENT_FIELD(uid, UID);
};
using T9209939847913682852 = CommentLike;

CURRENT_STRUCT(Persisted_CommentLikeUpdated) {
  CURRENT_FIELD(data, CommentLike);
};
using T9208038049197843321 = Persisted_CommentLikeUpdated;

CURRENT_STRUCT(CardFlagAsInappropriate) {
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(uid, UID);
};
using T9208106292780677444 = CardFlagAsInappropriate;

CURRENT_STRUCT(Persisted_CardFlagAsInappropriateUpdated) {
  CURRENT_FIELD(data, CardFlagAsInappropriate);
};
using T9205179798578814994 = Persisted_CardFlagAsInappropriateUpdated;

CURRENT_STRUCT(CommentFlagAsInappropriate) {
  CURRENT_FIELD(oid, OID);
  CURRENT_FIELD(uid, UID);
};
using T9209939848218954266 = CommentFlagAsInappropriate;

CURRENT_STRUCT(Persisted_CommentFlagAsInappropriateUpdated) {
  CURRENT_FIELD(data, CommentFlagAsInappropriate);
};
using T9207999136950447999 = Persisted_CommentFlagAsInappropriateUpdated;

CURRENT_STRUCT(NotificationMyCardNewComment) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(oid, OID);
  CURRENT_FIELD(text, std::string);
};
using T9205139962535721888 = NotificationMyCardNewComment;

CURRENT_STRUCT(NotificationNewReplyToMyComment) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(oid, OID);
  CURRENT_FIELD(text, std::string);
};
using T9205139961216784732 = NotificationNewReplyToMyComment;

CURRENT_STRUCT(NotificationMyCommentLiked) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(oid, OID);
  CURRENT_FIELD(text, std::string);
};
using T9205139961216938388 = NotificationMyCommentLiked;

CURRENT_STRUCT(NotificationNewCommentOnCardIStarred) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
  CURRENT_FIELD(oid, OID);
  CURRENT_FIELD(text, std::string);
};
using T9205139961498515997 = NotificationNewCommentOnCardIStarred;

CURRENT_STRUCT(NotificationMyCardStarred) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
};
using T9206133354855243134 = NotificationMyCardStarred;

CURRENT_STRUCT(NotificationNewVotesOnMyCard) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(cid, CID);
};
using T9206133355116522510 = NotificationNewVotesOnMyCard;

CURRENT_VARIANT(Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E, NotificationMyCardNewComment, NotificationNewReplyToMyComment, NotificationMyCommentLiked, NotificationNewCommentOnCardIStarred, NotificationMyCardStarred, NotificationNewVotesOnMyCard);
using T9226204898011996251 = Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E;

CURRENT_STRUCT(Notification) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(timestamp, std::chrono::microseconds);
  CURRENT_FIELD(notification, Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E);
};
using T9200800801070359784 = Notification;

CURRENT_STRUCT(Persisted_NotificationUpdated) {
  CURRENT_FIELD(data, Notification);
};
using T9206511959490104291 = Persisted_NotificationUpdated;

CURRENT_STRUCT(StarNotificationAlreadySent) {
  CURRENT_FIELD(key, (std::pair<UID, CID>));
};
using T9200021088792210898 = StarNotificationAlreadySent;

CURRENT_STRUCT(Persisted_StarNotificationAlreadySentUpdated) {
  CURRENT_FIELD(data, StarNotificationAlreadySent);
};
using T9209371058047233410 = Persisted_StarNotificationAlreadySentUpdated;

CURRENT_STRUCT(UserReportedUser) {
  CURRENT_FIELD(who, UID);
  CURRENT_FIELD(whom, UID);
};
using T9204169224127870611 = UserReportedUser;

CURRENT_STRUCT(Persisted_UserReportedUserUpdated) {
  CURRENT_FIELD(data, UserReportedUser);
};
using T9208731901677440463 = Persisted_UserReportedUserUpdated;

CURRENT_STRUCT(UserBlockedUser) {
  CURRENT_FIELD(who, UID);
  CURRENT_FIELD(whom, UID);
};
using T9204169225399823167 = UserBlockedUser;

CURRENT_STRUCT(Persisted_UserBlockedUserUpdated) {
  CURRENT_FIELD(data, UserBlockedUser);
};
using T9207720766366811801 = Persisted_UserBlockedUserUpdated;

CURRENT_STRUCT(BannedUser) {
  CURRENT_FIELD(banned_uid, UID);
};
using T9200350026662591521 = BannedUser;

CURRENT_STRUCT(Persisted_BannedUserUpdated) {
  CURRENT_FIELD(data, BannedUser);
};
using T9208494679507983003 = Persisted_BannedUserUpdated;

CURRENT_STRUCT(Persisted_UserDeleted) {
  CURRENT_FIELD(key, UID);
};
using T9209651558455239750 = Persisted_UserDeleted;

CURRENT_STRUCT(Persisted_AuthKeyTokenPairDeleted) {
  CURRENT_FIELD(key, (std::pair<AuthKey, std::string>));
};
using T9200957569766293725 = Persisted_AuthKeyTokenPairDeleted;

CURRENT_STRUCT(Persisted_UIDAuthKeyPairDeleted) {
  CURRENT_FIELD(key, (std::pair<UID, AuthKey>));
};
using T9200484496849192063 = Persisted_UIDAuthKeyPairDeleted;

CURRENT_STRUCT(Persisted_CardDeleted) {
  CURRENT_FIELD(key, CID);
};
using T9209669513953389583 = Persisted_CardDeleted;

CURRENT_STRUCT(Persisted_AuthorCardDeleted) {
  CURRENT_FIELD(key, (std::pair<UID, CID>));
};
using T9200021088872110109 = Persisted_AuthorCardDeleted;

CURRENT_STRUCT(Persisted_AnswerDeleted) {
  CURRENT_FIELD(key, (std::pair<UID, CID>));
};
using T9200021089286200408 = Persisted_AnswerDeleted;

CURRENT_STRUCT(Persisted_FavoriteDeleted) {
  CURRENT_FIELD(key, (std::pair<UID, CID>));
};
using T9200021086404720600 = Persisted_FavoriteDeleted;

CURRENT_STRUCT(Persisted_CommentDeleted) {
  CURRENT_FIELD(key, (std::pair<CID, OID>));
};
using T9200217426274301327 = Persisted_CommentDeleted;

CURRENT_STRUCT(Persisted_CommentLikeDeleted) {
  CURRENT_FIELD(key, (std::pair<OID, UID>));
};
using T9201882479303051010 = Persisted_CommentLikeDeleted;

CURRENT_STRUCT(Persisted_CardFlagAsInappropriateDeleted) {
  CURRENT_FIELD(key, (std::pair<CID, UID>));
};
using T9203637737703425114 = Persisted_CardFlagAsInappropriateDeleted;

CURRENT_STRUCT(Persisted_CommentFlagAsInappropriateDeleted) {
  CURRENT_FIELD(key, (std::pair<OID, UID>));
};
using T9201882481404070660 = Persisted_CommentFlagAsInappropriateDeleted;

CURRENT_STRUCT(Persisted_NotificationDeleted) {
  CURRENT_FIELD(key, (std::pair<UID, std::chrono::microseconds>));
};
using T9208256149167877980 = Persisted_NotificationDeleted;

CURRENT_STRUCT(Persisted_StarNotificationAlreadySentDeleted) {
  CURRENT_FIELD(key, (std::pair<UID, CID>));
};
using T9200021085882747139 = Persisted_StarNotificationAlreadySentDeleted;

CURRENT_STRUCT(Persisted_UserReportedUserDeleted) {
  CURRENT_FIELD(key, (std::pair<UID, UID>));
};
using T9208287678788053347 = Persisted_UserReportedUserDeleted;

CURRENT_STRUCT(Persisted_UserBlockedUserDeleted) {
  CURRENT_FIELD(key, (std::pair<UID, UID>));
};
using T9208287676676579893 = Persisted_UserBlockedUserDeleted;

CURRENT_STRUCT(Persisted_BannedUserDeleted) {
  CURRENT_FIELD(key, UID);
};
using T9209651559338936080 = Persisted_BannedUserDeleted;

CURRENT_VARIANT(Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E, Persisted_UserUpdated, Persisted_AuthKeyTokenPairUpdated, Persisted_UIDAuthKeyPairUpdated, Persisted_CardUpdated, Persisted_AuthorCardUpdated, Persisted_AnswerUpdated, Persisted_FavoriteUpdated, Persisted_CommentUpdated, Persisted_CommentLikeUpdated, Persisted_CardFlagAsInappropriateUpdated, Persisted_CommentFlagAsInappropriateUpdated, Persisted_NotificationUpdated, Persisted_StarNotificationAlreadySentUpdated, Persisted_UserReportedUserUpdated, Persisted_UserBlockedUserUpdated, Persisted_BannedUserUpdated, Persisted_UserDeleted, Persisted_AuthKeyTokenPairDeleted, Persisted_UIDAuthKeyPairDeleted, Persisted_CardDeleted, Persisted_AuthorCardDeleted, Persisted_AnswerDeleted, Persisted_FavoriteDeleted, Persisted_CommentDeleted, Persisted_CommentLikeDeleted, Persisted_CardFlagAsInappropriateDeleted, Persisted_CommentFlagAsInappropriateDeleted, Persisted_NotificationDeleted, Persisted_StarNotificationAlreadySentDeleted, Persisted_UserReportedUserDeleted, Persisted_UserBlockedUserDeleted, Persisted_BannedUserDeleted);
using T9226272004857575759 = Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E;

CURRENT_STRUCT(Transaction_T9226272004857575759) {
  CURRENT_FIELD(meta, TransactionMeta);
  CURRENT_FIELD(mutations, std::vector<Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E>);
};
using T9203386479240949157 = Transaction_T9226272004857575759;

}  // namespace current_userspace_3b3bd18430c5a642

CURRENT_NAMESPACE(USERSPACE_3B3BD18430C5A642) {
  CURRENT_NAMESPACE_TYPE(AUTH_TYPE, current_userspace_3b3bd18430c5a642::AUTH_TYPE);
  CURRENT_NAMESPACE_TYPE(ANSWER, current_userspace_3b3bd18430c5a642::ANSWER);
  CURRENT_NAMESPACE_TYPE(UID, current_userspace_3b3bd18430c5a642::UID);
  CURRENT_NAMESPACE_TYPE(OID, current_userspace_3b3bd18430c5a642::OID);
  CURRENT_NAMESPACE_TYPE(CID, current_userspace_3b3bd18430c5a642::CID);
  CURRENT_NAMESPACE_TYPE(Persisted_StarNotificationAlreadySentDeleted, current_userspace_3b3bd18430c5a642::Persisted_StarNotificationAlreadySentDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_FavoriteDeleted, current_userspace_3b3bd18430c5a642::Persisted_FavoriteDeleted);
  CURRENT_NAMESPACE_TYPE(StarNotificationAlreadySent, current_userspace_3b3bd18430c5a642::StarNotificationAlreadySent);
  CURRENT_NAMESPACE_TYPE(Persisted_AuthorCardDeleted, current_userspace_3b3bd18430c5a642::Persisted_AuthorCardDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_AnswerDeleted, current_userspace_3b3bd18430c5a642::Persisted_AnswerDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentDeleted, current_userspace_3b3bd18430c5a642::Persisted_CommentDeleted);
  CURRENT_NAMESPACE_TYPE(BannedUser, current_userspace_3b3bd18430c5a642::BannedUser);
  CURRENT_NAMESPACE_TYPE(AuthorCard, current_userspace_3b3bd18430c5a642::AuthorCard);
  CURRENT_NAMESPACE_TYPE(Persisted_UIDAuthKeyPairDeleted, current_userspace_3b3bd18430c5a642::Persisted_UIDAuthKeyPairDeleted);
  CURRENT_NAMESPACE_TYPE(Favorite, current_userspace_3b3bd18430c5a642::Favorite);
  CURRENT_NAMESPACE_TYPE(Notification, current_userspace_3b3bd18430c5a642::Notification);
  CURRENT_NAMESPACE_TYPE(Persisted_AuthKeyTokenPairDeleted, current_userspace_3b3bd18430c5a642::Persisted_AuthKeyTokenPairDeleted);
  CURRENT_NAMESPACE_TYPE(UIDAuthKeyPair, current_userspace_3b3bd18430c5a642::UIDAuthKeyPair);
  CURRENT_NAMESPACE_TYPE(Persisted_CardUpdated, current_userspace_3b3bd18430c5a642::Persisted_CardUpdated);
  CURRENT_NAMESPACE_TYPE(TransactionMeta, current_userspace_3b3bd18430c5a642::TransactionMeta);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentLikeDeleted, current_userspace_3b3bd18430c5a642::Persisted_CommentLikeDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentFlagAsInappropriateDeleted, current_userspace_3b3bd18430c5a642::Persisted_CommentFlagAsInappropriateDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentUpdated, current_userspace_3b3bd18430c5a642::Persisted_CommentUpdated);
  CURRENT_NAMESPACE_TYPE(User, current_userspace_3b3bd18430c5a642::User);
  CURRENT_NAMESPACE_TYPE(Transaction_T9226272004857575759, current_userspace_3b3bd18430c5a642::Transaction_T9226272004857575759);
  CURRENT_NAMESPACE_TYPE(Card, current_userspace_3b3bd18430c5a642::Card);
  CURRENT_NAMESPACE_TYPE(Comment, current_userspace_3b3bd18430c5a642::Comment);
  CURRENT_NAMESPACE_TYPE(Persisted_CardFlagAsInappropriateDeleted, current_userspace_3b3bd18430c5a642::Persisted_CardFlagAsInappropriateDeleted);
  CURRENT_NAMESPACE_TYPE(AuthKeyTokenPair, current_userspace_3b3bd18430c5a642::AuthKeyTokenPair);
  CURRENT_NAMESPACE_TYPE(Persisted_UIDAuthKeyPairUpdated, current_userspace_3b3bd18430c5a642::Persisted_UIDAuthKeyPairUpdated);
  CURRENT_NAMESPACE_TYPE(UserReportedUser, current_userspace_3b3bd18430c5a642::UserReportedUser);
  CURRENT_NAMESPACE_TYPE(UserBlockedUser, current_userspace_3b3bd18430c5a642::UserBlockedUser);
  CURRENT_NAMESPACE_TYPE(Color, current_userspace_3b3bd18430c5a642::Color);
  CURRENT_NAMESPACE_TYPE(Persisted_AuthorCardUpdated, current_userspace_3b3bd18430c5a642::Persisted_AuthorCardUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_AnswerUpdated, current_userspace_3b3bd18430c5a642::Persisted_AnswerUpdated);
  CURRENT_NAMESPACE_TYPE(NotificationNewReplyToMyComment, current_userspace_3b3bd18430c5a642::NotificationNewReplyToMyComment);
  CURRENT_NAMESPACE_TYPE(NotificationMyCommentLiked, current_userspace_3b3bd18430c5a642::NotificationMyCommentLiked);
  CURRENT_NAMESPACE_TYPE(NotificationNewCommentOnCardIStarred, current_userspace_3b3bd18430c5a642::NotificationNewCommentOnCardIStarred);
  CURRENT_NAMESPACE_TYPE(NotificationMyCardNewComment, current_userspace_3b3bd18430c5a642::NotificationMyCardNewComment);
  CURRENT_NAMESPACE_TYPE(Persisted_CardFlagAsInappropriateUpdated, current_userspace_3b3bd18430c5a642::Persisted_CardFlagAsInappropriateUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_AuthKeyTokenPairUpdated, current_userspace_3b3bd18430c5a642::Persisted_AuthKeyTokenPairUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_FavoriteUpdated, current_userspace_3b3bd18430c5a642::Persisted_FavoriteUpdated);
  CURRENT_NAMESPACE_TYPE(NotificationMyCardStarred, current_userspace_3b3bd18430c5a642::NotificationMyCardStarred);
  CURRENT_NAMESPACE_TYPE(NotificationNewVotesOnMyCard, current_userspace_3b3bd18430c5a642::NotificationNewVotesOnMyCard);
  CURRENT_NAMESPACE_TYPE(Answer, current_userspace_3b3bd18430c5a642::Answer);
  CURRENT_NAMESPACE_TYPE(Persisted_NotificationUpdated, current_userspace_3b3bd18430c5a642::Persisted_NotificationUpdated);
  CURRENT_NAMESPACE_TYPE(AuthKey, current_userspace_3b3bd18430c5a642::AuthKey);
  CURRENT_NAMESPACE_TYPE(Persisted_UserBlockedUserUpdated, current_userspace_3b3bd18430c5a642::Persisted_UserBlockedUserUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_UserUpdated, current_userspace_3b3bd18430c5a642::Persisted_UserUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentFlagAsInappropriateUpdated, current_userspace_3b3bd18430c5a642::Persisted_CommentFlagAsInappropriateUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentLikeUpdated, current_userspace_3b3bd18430c5a642::Persisted_CommentLikeUpdated);
  CURRENT_NAMESPACE_TYPE(CardFlagAsInappropriate, current_userspace_3b3bd18430c5a642::CardFlagAsInappropriate);
  CURRENT_NAMESPACE_TYPE(Persisted_NotificationDeleted, current_userspace_3b3bd18430c5a642::Persisted_NotificationDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_UserBlockedUserDeleted, current_userspace_3b3bd18430c5a642::Persisted_UserBlockedUserDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_UserReportedUserDeleted, current_userspace_3b3bd18430c5a642::Persisted_UserReportedUserDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_BannedUserUpdated, current_userspace_3b3bd18430c5a642::Persisted_BannedUserUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_UserReportedUserUpdated, current_userspace_3b3bd18430c5a642::Persisted_UserReportedUserUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_StarNotificationAlreadySentUpdated, current_userspace_3b3bd18430c5a642::Persisted_StarNotificationAlreadySentUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_UserDeleted, current_userspace_3b3bd18430c5a642::Persisted_UserDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_BannedUserDeleted, current_userspace_3b3bd18430c5a642::Persisted_BannedUserDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_CardDeleted, current_userspace_3b3bd18430c5a642::Persisted_CardDeleted);
  CURRENT_NAMESPACE_TYPE(CommentLike, current_userspace_3b3bd18430c5a642::CommentLike);
  CURRENT_NAMESPACE_TYPE(CommentFlagAsInappropriate, current_userspace_3b3bd18430c5a642::CommentFlagAsInappropriate);
  CURRENT_NAMESPACE_TYPE(Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E, current_userspace_3b3bd18430c5a642::Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E);
  CURRENT_NAMESPACE_TYPE(Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E, current_userspace_3b3bd18430c5a642::Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E);
};  // CURRENT_NAMESPACE(USERSPACE_3B3BD18430C5A642)

namespace current {
namespace type_evolution {

// Default evolution for `CURRENT_ENUM(AUTH_TYPE)`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::AUTH_TYPE, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_3B3BD18430C5A642::AUTH_TYPE from,
                 typename INTO::AUTH_TYPE& into) {
    // TODO(dkorolev): Check enum underlying type, but not too strictly to be extensible.
    into = static_cast<typename INTO::AUTH_TYPE>(from);
  }
};

// Default evolution for `CURRENT_ENUM(ANSWER)`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::ANSWER, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_3B3BD18430C5A642::ANSWER from,
                 typename INTO::ANSWER& into) {
    // TODO(dkorolev): Check enum underlying type, but not too strictly to be extensible.
    into = static_cast<typename INTO::ANSWER>(from);
  }
};

// Default evolution for `CURRENT_ENUM(UID)`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::UID, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_3B3BD18430C5A642::UID from,
                 typename INTO::UID& into) {
    // TODO(dkorolev): Check enum underlying type, but not too strictly to be extensible.
    into = static_cast<typename INTO::UID>(from);
  }
};

// Default evolution for `CURRENT_ENUM(OID)`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::OID, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_3B3BD18430C5A642::OID from,
                 typename INTO::OID& into) {
    // TODO(dkorolev): Check enum underlying type, but not too strictly to be extensible.
    into = static_cast<typename INTO::OID>(from);
  }
};

// Default evolution for `CURRENT_ENUM(CID)`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::CID, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_3B3BD18430C5A642::CID from,
                 typename INTO::CID& into) {
    // TODO(dkorolev): Check enum underlying type, but not too strictly to be extensible.
    into = static_cast<typename INTO::CID>(from);
  }
};

// Default evolution for struct `Persisted_StarNotificationAlreadySentDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_StarNotificationAlreadySentDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_StarNotificationAlreadySentDeleted& from,
                 typename INTO::Persisted_StarNotificationAlreadySentDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_StarNotificationAlreadySentDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_FavoriteDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_FavoriteDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_FavoriteDeleted& from,
                 typename INTO::Persisted_FavoriteDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_FavoriteDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `StarNotificationAlreadySent`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::StarNotificationAlreadySent, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::StarNotificationAlreadySent& from,
                 typename INTO::StarNotificationAlreadySent& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::StarNotificationAlreadySent>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_AuthorCardDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_AuthorCardDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_AuthorCardDeleted& from,
                 typename INTO::Persisted_AuthorCardDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_AuthorCardDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_AnswerDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_AnswerDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_AnswerDeleted& from,
                 typename INTO::Persisted_AnswerDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_AnswerDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_CommentDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CommentDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CommentDeleted& from,
                 typename INTO::Persisted_CommentDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CommentDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `BannedUser`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::BannedUser, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::BannedUser& from,
                 typename INTO::BannedUser& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::BannedUser>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.banned_uid), EVOLUTOR>::template Go<INTO>(from.banned_uid, into.banned_uid);
  }
};

// Default evolution for struct `AuthorCard`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::AuthorCard, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::AuthorCard& from,
                 typename INTO::AuthorCard& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::AuthorCard>::value == 3,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
  }
};

// Default evolution for struct `Persisted_UIDAuthKeyPairDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_UIDAuthKeyPairDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_UIDAuthKeyPairDeleted& from,
                 typename INTO::Persisted_UIDAuthKeyPairDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_UIDAuthKeyPairDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Favorite`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Favorite, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Favorite& from,
                 typename INTO::Favorite& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Favorite>::value == 4,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
      Evolve<NAMESPACE, decltype(from.favorited), EVOLUTOR>::template Go<INTO>(from.favorited, into.favorited);
  }
};

// Default evolution for struct `Notification`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Notification, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Notification& from,
                 typename INTO::Notification& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Notification>::value == 3,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.timestamp), EVOLUTOR>::template Go<INTO>(from.timestamp, into.timestamp);
      Evolve<NAMESPACE, decltype(from.notification), EVOLUTOR>::template Go<INTO>(from.notification, into.notification);
  }
};

// Default evolution for struct `Persisted_AuthKeyTokenPairDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_AuthKeyTokenPairDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_AuthKeyTokenPairDeleted& from,
                 typename INTO::Persisted_AuthKeyTokenPairDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_AuthKeyTokenPairDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `UIDAuthKeyPair`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::UIDAuthKeyPair, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::UIDAuthKeyPair& from,
                 typename INTO::UIDAuthKeyPair& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::UIDAuthKeyPair>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.auth_key), EVOLUTOR>::template Go<INTO>(from.auth_key, into.auth_key);
  }
};

// Default evolution for struct `Persisted_CardUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CardUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CardUpdated& from,
                 typename INTO::Persisted_CardUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CardUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `TransactionMeta`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::TransactionMeta, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::TransactionMeta& from,
                 typename INTO::TransactionMeta& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::TransactionMeta>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.timestamp), EVOLUTOR>::template Go<INTO>(from.timestamp, into.timestamp);
      Evolve<NAMESPACE, decltype(from.fields), EVOLUTOR>::template Go<INTO>(from.fields, into.fields);
  }
};

// Default evolution for struct `Persisted_CommentLikeDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CommentLikeDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CommentLikeDeleted& from,
                 typename INTO::Persisted_CommentLikeDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CommentLikeDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_CommentFlagAsInappropriateDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CommentFlagAsInappropriateDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CommentFlagAsInappropriateDeleted& from,
                 typename INTO::Persisted_CommentFlagAsInappropriateDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CommentFlagAsInappropriateDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_CommentUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CommentUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CommentUpdated& from,
                 typename INTO::Persisted_CommentUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CommentUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `User`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::User, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::User& from,
                 typename INTO::User& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::User>::value == 4,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
      Evolve<NAMESPACE, decltype(from.level), EVOLUTOR>::template Go<INTO>(from.level, into.level);
      Evolve<NAMESPACE, decltype(from.score), EVOLUTOR>::template Go<INTO>(from.score, into.score);
  }
};

// Default evolution for struct `Transaction_T9226272004857575759`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Transaction_T9226272004857575759, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Transaction_T9226272004857575759& from,
                 typename INTO::Transaction_T9226272004857575759& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Transaction_T9226272004857575759>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.meta), EVOLUTOR>::template Go<INTO>(from.meta, into.meta);
      Evolve<NAMESPACE, decltype(from.mutations), EVOLUTOR>::template Go<INTO>(from.mutations, into.mutations);
  }
};

// Default evolution for struct `Card`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Card, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Card& from,
                 typename INTO::Card& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Card>::value == 8,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
      Evolve<NAMESPACE, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
      Evolve<NAMESPACE, decltype(from.color), EVOLUTOR>::template Go<INTO>(from.color, into.color);
      Evolve<NAMESPACE, decltype(from.ctfo_count), EVOLUTOR>::template Go<INTO>(from.ctfo_count, into.ctfo_count);
      Evolve<NAMESPACE, decltype(from.tfu_count), EVOLUTOR>::template Go<INTO>(from.tfu_count, into.tfu_count);
      Evolve<NAMESPACE, decltype(from.skip_count), EVOLUTOR>::template Go<INTO>(from.skip_count, into.skip_count);
      Evolve<NAMESPACE, decltype(from.startup_index), EVOLUTOR>::template Go<INTO>(from.startup_index, into.startup_index);
  }
};

// Default evolution for struct `Comment`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Comment, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Comment& from,
                 typename INTO::Comment& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Comment>::value == 6,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<NAMESPACE, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
      Evolve<NAMESPACE, decltype(from.parent_oid), EVOLUTOR>::template Go<INTO>(from.parent_oid, into.parent_oid);
      Evolve<NAMESPACE, decltype(from.author_uid), EVOLUTOR>::template Go<INTO>(from.author_uid, into.author_uid);
      Evolve<NAMESPACE, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};

// Default evolution for struct `Persisted_CardFlagAsInappropriateDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CardFlagAsInappropriateDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CardFlagAsInappropriateDeleted& from,
                 typename INTO::Persisted_CardFlagAsInappropriateDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CardFlagAsInappropriateDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `AuthKeyTokenPair`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::AuthKeyTokenPair, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::AuthKeyTokenPair& from,
                 typename INTO::AuthKeyTokenPair& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::AuthKeyTokenPair>::value == 3,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.auth_key), EVOLUTOR>::template Go<INTO>(from.auth_key, into.auth_key);
      Evolve<NAMESPACE, decltype(from.token), EVOLUTOR>::template Go<INTO>(from.token, into.token);
      Evolve<NAMESPACE, decltype(from.valid), EVOLUTOR>::template Go<INTO>(from.valid, into.valid);
  }
};

// Default evolution for struct `Persisted_UIDAuthKeyPairUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_UIDAuthKeyPairUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_UIDAuthKeyPairUpdated& from,
                 typename INTO::Persisted_UIDAuthKeyPairUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_UIDAuthKeyPairUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `UserReportedUser`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::UserReportedUser, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::UserReportedUser& from,
                 typename INTO::UserReportedUser& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::UserReportedUser>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.who), EVOLUTOR>::template Go<INTO>(from.who, into.who);
      Evolve<NAMESPACE, decltype(from.whom), EVOLUTOR>::template Go<INTO>(from.whom, into.whom);
  }
};

// Default evolution for struct `UserBlockedUser`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::UserBlockedUser, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::UserBlockedUser& from,
                 typename INTO::UserBlockedUser& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::UserBlockedUser>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.who), EVOLUTOR>::template Go<INTO>(from.who, into.who);
      Evolve<NAMESPACE, decltype(from.whom), EVOLUTOR>::template Go<INTO>(from.whom, into.whom);
  }
};

// Default evolution for struct `Color`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Color, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Color& from,
                 typename INTO::Color& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Color>::value == 3,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.red), EVOLUTOR>::template Go<INTO>(from.red, into.red);
      Evolve<NAMESPACE, decltype(from.green), EVOLUTOR>::template Go<INTO>(from.green, into.green);
      Evolve<NAMESPACE, decltype(from.blue), EVOLUTOR>::template Go<INTO>(from.blue, into.blue);
  }
};

// Default evolution for struct `Persisted_AuthorCardUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_AuthorCardUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_AuthorCardUpdated& from,
                 typename INTO::Persisted_AuthorCardUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_AuthorCardUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_AnswerUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_AnswerUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_AnswerUpdated& from,
                 typename INTO::Persisted_AnswerUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_AnswerUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `NotificationNewReplyToMyComment`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::NotificationNewReplyToMyComment, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::NotificationNewReplyToMyComment& from,
                 typename INTO::NotificationNewReplyToMyComment& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::NotificationNewReplyToMyComment>::value == 4,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<NAMESPACE, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};

// Default evolution for struct `NotificationMyCommentLiked`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::NotificationMyCommentLiked, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::NotificationMyCommentLiked& from,
                 typename INTO::NotificationMyCommentLiked& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::NotificationMyCommentLiked>::value == 4,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<NAMESPACE, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};

// Default evolution for struct `NotificationNewCommentOnCardIStarred`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::NotificationNewCommentOnCardIStarred, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::NotificationNewCommentOnCardIStarred& from,
                 typename INTO::NotificationNewCommentOnCardIStarred& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::NotificationNewCommentOnCardIStarred>::value == 4,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<NAMESPACE, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};

// Default evolution for struct `NotificationMyCardNewComment`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::NotificationMyCardNewComment, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::NotificationMyCardNewComment& from,
                 typename INTO::NotificationMyCardNewComment& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::NotificationMyCardNewComment>::value == 4,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<NAMESPACE, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};

// Default evolution for struct `Persisted_CardFlagAsInappropriateUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CardFlagAsInappropriateUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CardFlagAsInappropriateUpdated& from,
                 typename INTO::Persisted_CardFlagAsInappropriateUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CardFlagAsInappropriateUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_AuthKeyTokenPairUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_AuthKeyTokenPairUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_AuthKeyTokenPairUpdated& from,
                 typename INTO::Persisted_AuthKeyTokenPairUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_AuthKeyTokenPairUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_FavoriteUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_FavoriteUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_FavoriteUpdated& from,
                 typename INTO::Persisted_FavoriteUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_FavoriteUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `NotificationMyCardStarred`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::NotificationMyCardStarred, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::NotificationMyCardStarred& from,
                 typename INTO::NotificationMyCardStarred& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::NotificationMyCardStarred>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
  }
};

// Default evolution for struct `NotificationNewVotesOnMyCard`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::NotificationNewVotesOnMyCard, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::NotificationNewVotesOnMyCard& from,
                 typename INTO::NotificationNewVotesOnMyCard& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::NotificationNewVotesOnMyCard>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
  }
};

// Default evolution for struct `Answer`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Answer, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Answer& from,
                 typename INTO::Answer& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Answer>::value == 3,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.answer), EVOLUTOR>::template Go<INTO>(from.answer, into.answer);
  }
};

// Default evolution for struct `Persisted_NotificationUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_NotificationUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_NotificationUpdated& from,
                 typename INTO::Persisted_NotificationUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_NotificationUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `AuthKey`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::AuthKey, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::AuthKey& from,
                 typename INTO::AuthKey& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::AuthKey>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
      Evolve<NAMESPACE, decltype(from.type), EVOLUTOR>::template Go<INTO>(from.type, into.type);
  }
};

// Default evolution for struct `Persisted_UserBlockedUserUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_UserBlockedUserUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_UserBlockedUserUpdated& from,
                 typename INTO::Persisted_UserBlockedUserUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_UserBlockedUserUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_UserUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_UserUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_UserUpdated& from,
                 typename INTO::Persisted_UserUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_UserUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_CommentFlagAsInappropriateUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CommentFlagAsInappropriateUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CommentFlagAsInappropriateUpdated& from,
                 typename INTO::Persisted_CommentFlagAsInappropriateUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CommentFlagAsInappropriateUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_CommentLikeUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CommentLikeUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CommentLikeUpdated& from,
                 typename INTO::Persisted_CommentLikeUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CommentLikeUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `CardFlagAsInappropriate`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::CardFlagAsInappropriate, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::CardFlagAsInappropriate& from,
                 typename INTO::CardFlagAsInappropriate& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::CardFlagAsInappropriate>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
  }
};

// Default evolution for struct `Persisted_NotificationDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_NotificationDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_NotificationDeleted& from,
                 typename INTO::Persisted_NotificationDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_NotificationDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_UserBlockedUserDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_UserBlockedUserDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_UserBlockedUserDeleted& from,
                 typename INTO::Persisted_UserBlockedUserDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_UserBlockedUserDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_UserReportedUserDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_UserReportedUserDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_UserReportedUserDeleted& from,
                 typename INTO::Persisted_UserReportedUserDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_UserReportedUserDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_BannedUserUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_BannedUserUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_BannedUserUpdated& from,
                 typename INTO::Persisted_BannedUserUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_BannedUserUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_UserReportedUserUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_UserReportedUserUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_UserReportedUserUpdated& from,
                 typename INTO::Persisted_UserReportedUserUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_UserReportedUserUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_StarNotificationAlreadySentUpdated`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_StarNotificationAlreadySentUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_StarNotificationAlreadySentUpdated& from,
                 typename INTO::Persisted_StarNotificationAlreadySentUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_StarNotificationAlreadySentUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};

// Default evolution for struct `Persisted_UserDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_UserDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_UserDeleted& from,
                 typename INTO::Persisted_UserDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_UserDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_BannedUserDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_BannedUserDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_BannedUserDeleted& from,
                 typename INTO::Persisted_BannedUserDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_BannedUserDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `Persisted_CardDeleted`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::Persisted_CardDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::Persisted_CardDeleted& from,
                 typename INTO::Persisted_CardDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::Persisted_CardDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};

// Default evolution for struct `CommentLike`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::CommentLike, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::CommentLike& from,
                 typename INTO::CommentLike& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::CommentLike>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
  }
};

// Default evolution for struct `CommentFlagAsInappropriate`.
template <typename NAMESPACE, typename EVOLUTOR>
struct Evolve<NAMESPACE, USERSPACE_3B3BD18430C5A642::CommentFlagAsInappropriate, EVOLUTOR> {
  template <typename INTO,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const typename NAMESPACE::CommentFlagAsInappropriate& from,
                 typename INTO::CommentFlagAsInappropriate& into) {
      static_assert(::current::reflection::FieldCounter<typename USERSPACE_3B3BD18430C5A642::CommentFlagAsInappropriate>::value == 2,
                    "Custom evolutor required.");
      Evolve<NAMESPACE, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<NAMESPACE, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
  }
};

// Default evolution for `Variant<NotificationMyCardNewComment, NotificationNewReplyToMyComment, NotificationMyCommentLiked, NotificationNewCommentOnCardIStarred, NotificationMyCardStarred, NotificationNewVotesOnMyCard>`.
template <typename DST, typename FROM_NAMESPACE, typename INTO, typename EVOLUTOR>
struct USERSPACE_3B3BD18430C5A642_Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E_Cases {
  DST& into;
  explicit USERSPACE_3B3BD18430C5A642_Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E_Cases(DST& into) : into(into) {}
  void operator()(const typename FROM_NAMESPACE::NotificationMyCardNewComment& value) const {
    using into_t = typename INTO::NotificationMyCardNewComment;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::NotificationMyCardNewComment, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::NotificationNewReplyToMyComment& value) const {
    using into_t = typename INTO::NotificationNewReplyToMyComment;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::NotificationNewReplyToMyComment, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::NotificationMyCommentLiked& value) const {
    using into_t = typename INTO::NotificationMyCommentLiked;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::NotificationMyCommentLiked, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::NotificationNewCommentOnCardIStarred& value) const {
    using into_t = typename INTO::NotificationNewCommentOnCardIStarred;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::NotificationNewCommentOnCardIStarred, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::NotificationMyCardStarred& value) const {
    using into_t = typename INTO::NotificationMyCardStarred;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::NotificationMyCardStarred, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::NotificationNewVotesOnMyCard& value) const {
    using into_t = typename INTO::NotificationNewVotesOnMyCard;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::NotificationNewVotesOnMyCard, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
};
template <typename NAMESPACE, typename EVOLUTOR, typename VARIANT_NAME_HELPER>
struct Evolve<NAMESPACE, ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_3B3BD18430C5A642::NotificationMyCardNewComment, USERSPACE_3B3BD18430C5A642::NotificationNewReplyToMyComment, USERSPACE_3B3BD18430C5A642::NotificationMyCommentLiked, USERSPACE_3B3BD18430C5A642::NotificationNewCommentOnCardIStarred, USERSPACE_3B3BD18430C5A642::NotificationMyCardStarred, USERSPACE_3B3BD18430C5A642::NotificationNewVotesOnMyCard>>, EVOLUTOR> {
  // TODO(dkorolev): A `static_assert` to ensure the number of cases is the same.
  template <typename INTO,
            typename CUSTOM_INTO_VARIANT_TYPE,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_3B3BD18430C5A642::NotificationMyCardNewComment, USERSPACE_3B3BD18430C5A642::NotificationNewReplyToMyComment, USERSPACE_3B3BD18430C5A642::NotificationMyCommentLiked, USERSPACE_3B3BD18430C5A642::NotificationNewCommentOnCardIStarred, USERSPACE_3B3BD18430C5A642::NotificationMyCardStarred, USERSPACE_3B3BD18430C5A642::NotificationNewVotesOnMyCard>>& from,
                 CUSTOM_INTO_VARIANT_TYPE& into) {
    from.Call(USERSPACE_3B3BD18430C5A642_Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E_Cases<decltype(into), NAMESPACE, INTO, EVOLUTOR>(into));
  }
};

// Default evolution for `Variant<Persisted_UserUpdated, Persisted_AuthKeyTokenPairUpdated, Persisted_UIDAuthKeyPairUpdated, Persisted_CardUpdated, Persisted_AuthorCardUpdated, Persisted_AnswerUpdated, Persisted_FavoriteUpdated, Persisted_CommentUpdated, Persisted_CommentLikeUpdated, Persisted_CardFlagAsInappropriateUpdated, Persisted_CommentFlagAsInappropriateUpdated, Persisted_NotificationUpdated, Persisted_StarNotificationAlreadySentUpdated, Persisted_UserReportedUserUpdated, Persisted_UserBlockedUserUpdated, Persisted_BannedUserUpdated, Persisted_UserDeleted, Persisted_AuthKeyTokenPairDeleted, Persisted_UIDAuthKeyPairDeleted, Persisted_CardDeleted, Persisted_AuthorCardDeleted, Persisted_AnswerDeleted, Persisted_FavoriteDeleted, Persisted_CommentDeleted, Persisted_CommentLikeDeleted, Persisted_CardFlagAsInappropriateDeleted, Persisted_CommentFlagAsInappropriateDeleted, Persisted_NotificationDeleted, Persisted_StarNotificationAlreadySentDeleted, Persisted_UserReportedUserDeleted, Persisted_UserBlockedUserDeleted, Persisted_BannedUserDeleted>`.
template <typename DST, typename FROM_NAMESPACE, typename INTO, typename EVOLUTOR>
struct USERSPACE_3B3BD18430C5A642_Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E_Cases {
  DST& into;
  explicit USERSPACE_3B3BD18430C5A642_Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E_Cases(DST& into) : into(into) {}
  void operator()(const typename FROM_NAMESPACE::Persisted_UserUpdated& value) const {
    using into_t = typename INTO::Persisted_UserUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UserUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_AuthKeyTokenPairUpdated& value) const {
    using into_t = typename INTO::Persisted_AuthKeyTokenPairUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_AuthKeyTokenPairUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_UIDAuthKeyPairUpdated& value) const {
    using into_t = typename INTO::Persisted_UIDAuthKeyPairUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UIDAuthKeyPairUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CardUpdated& value) const {
    using into_t = typename INTO::Persisted_CardUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CardUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_AuthorCardUpdated& value) const {
    using into_t = typename INTO::Persisted_AuthorCardUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_AuthorCardUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_AnswerUpdated& value) const {
    using into_t = typename INTO::Persisted_AnswerUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_AnswerUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_FavoriteUpdated& value) const {
    using into_t = typename INTO::Persisted_FavoriteUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_FavoriteUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CommentUpdated& value) const {
    using into_t = typename INTO::Persisted_CommentUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CommentUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CommentLikeUpdated& value) const {
    using into_t = typename INTO::Persisted_CommentLikeUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CommentLikeUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CardFlagAsInappropriateUpdated& value) const {
    using into_t = typename INTO::Persisted_CardFlagAsInappropriateUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CardFlagAsInappropriateUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CommentFlagAsInappropriateUpdated& value) const {
    using into_t = typename INTO::Persisted_CommentFlagAsInappropriateUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CommentFlagAsInappropriateUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_NotificationUpdated& value) const {
    using into_t = typename INTO::Persisted_NotificationUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_NotificationUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_StarNotificationAlreadySentUpdated& value) const {
    using into_t = typename INTO::Persisted_StarNotificationAlreadySentUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_StarNotificationAlreadySentUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_UserReportedUserUpdated& value) const {
    using into_t = typename INTO::Persisted_UserReportedUserUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UserReportedUserUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_UserBlockedUserUpdated& value) const {
    using into_t = typename INTO::Persisted_UserBlockedUserUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UserBlockedUserUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_BannedUserUpdated& value) const {
    using into_t = typename INTO::Persisted_BannedUserUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_BannedUserUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_UserDeleted& value) const {
    using into_t = typename INTO::Persisted_UserDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UserDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_AuthKeyTokenPairDeleted& value) const {
    using into_t = typename INTO::Persisted_AuthKeyTokenPairDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_AuthKeyTokenPairDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_UIDAuthKeyPairDeleted& value) const {
    using into_t = typename INTO::Persisted_UIDAuthKeyPairDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UIDAuthKeyPairDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CardDeleted& value) const {
    using into_t = typename INTO::Persisted_CardDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CardDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_AuthorCardDeleted& value) const {
    using into_t = typename INTO::Persisted_AuthorCardDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_AuthorCardDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_AnswerDeleted& value) const {
    using into_t = typename INTO::Persisted_AnswerDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_AnswerDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_FavoriteDeleted& value) const {
    using into_t = typename INTO::Persisted_FavoriteDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_FavoriteDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CommentDeleted& value) const {
    using into_t = typename INTO::Persisted_CommentDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CommentDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CommentLikeDeleted& value) const {
    using into_t = typename INTO::Persisted_CommentLikeDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CommentLikeDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CardFlagAsInappropriateDeleted& value) const {
    using into_t = typename INTO::Persisted_CardFlagAsInappropriateDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CardFlagAsInappropriateDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_CommentFlagAsInappropriateDeleted& value) const {
    using into_t = typename INTO::Persisted_CommentFlagAsInappropriateDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_CommentFlagAsInappropriateDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_NotificationDeleted& value) const {
    using into_t = typename INTO::Persisted_NotificationDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_NotificationDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_StarNotificationAlreadySentDeleted& value) const {
    using into_t = typename INTO::Persisted_StarNotificationAlreadySentDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_StarNotificationAlreadySentDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_UserReportedUserDeleted& value) const {
    using into_t = typename INTO::Persisted_UserReportedUserDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UserReportedUserDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_UserBlockedUserDeleted& value) const {
    using into_t = typename INTO::Persisted_UserBlockedUserDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UserBlockedUserDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_BannedUserDeleted& value) const {
    using into_t = typename INTO::Persisted_BannedUserDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_BannedUserDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
};
template <typename NAMESPACE, typename EVOLUTOR, typename VARIANT_NAME_HELPER>
struct Evolve<NAMESPACE, ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_3B3BD18430C5A642::Persisted_UserUpdated, USERSPACE_3B3BD18430C5A642::Persisted_AuthKeyTokenPairUpdated, USERSPACE_3B3BD18430C5A642::Persisted_UIDAuthKeyPairUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CardUpdated, USERSPACE_3B3BD18430C5A642::Persisted_AuthorCardUpdated, USERSPACE_3B3BD18430C5A642::Persisted_AnswerUpdated, USERSPACE_3B3BD18430C5A642::Persisted_FavoriteUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CommentUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CommentLikeUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CardFlagAsInappropriateUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CommentFlagAsInappropriateUpdated, USERSPACE_3B3BD18430C5A642::Persisted_NotificationUpdated, USERSPACE_3B3BD18430C5A642::Persisted_StarNotificationAlreadySentUpdated, USERSPACE_3B3BD18430C5A642::Persisted_UserReportedUserUpdated, USERSPACE_3B3BD18430C5A642::Persisted_UserBlockedUserUpdated, USERSPACE_3B3BD18430C5A642::Persisted_BannedUserUpdated, USERSPACE_3B3BD18430C5A642::Persisted_UserDeleted, USERSPACE_3B3BD18430C5A642::Persisted_AuthKeyTokenPairDeleted, USERSPACE_3B3BD18430C5A642::Persisted_UIDAuthKeyPairDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CardDeleted, USERSPACE_3B3BD18430C5A642::Persisted_AuthorCardDeleted, USERSPACE_3B3BD18430C5A642::Persisted_AnswerDeleted, USERSPACE_3B3BD18430C5A642::Persisted_FavoriteDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CommentDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CommentLikeDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CardFlagAsInappropriateDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CommentFlagAsInappropriateDeleted, USERSPACE_3B3BD18430C5A642::Persisted_NotificationDeleted, USERSPACE_3B3BD18430C5A642::Persisted_StarNotificationAlreadySentDeleted, USERSPACE_3B3BD18430C5A642::Persisted_UserReportedUserDeleted, USERSPACE_3B3BD18430C5A642::Persisted_UserBlockedUserDeleted, USERSPACE_3B3BD18430C5A642::Persisted_BannedUserDeleted>>, EVOLUTOR> {
  // TODO(dkorolev): A `static_assert` to ensure the number of cases is the same.
  template <typename INTO,
            typename CUSTOM_INTO_VARIANT_TYPE,
            class CHECK = NAMESPACE,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_3B3BD18430C5A642, CHECK>::value>>
  static void Go(const ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_3B3BD18430C5A642::Persisted_UserUpdated, USERSPACE_3B3BD18430C5A642::Persisted_AuthKeyTokenPairUpdated, USERSPACE_3B3BD18430C5A642::Persisted_UIDAuthKeyPairUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CardUpdated, USERSPACE_3B3BD18430C5A642::Persisted_AuthorCardUpdated, USERSPACE_3B3BD18430C5A642::Persisted_AnswerUpdated, USERSPACE_3B3BD18430C5A642::Persisted_FavoriteUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CommentUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CommentLikeUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CardFlagAsInappropriateUpdated, USERSPACE_3B3BD18430C5A642::Persisted_CommentFlagAsInappropriateUpdated, USERSPACE_3B3BD18430C5A642::Persisted_NotificationUpdated, USERSPACE_3B3BD18430C5A642::Persisted_StarNotificationAlreadySentUpdated, USERSPACE_3B3BD18430C5A642::Persisted_UserReportedUserUpdated, USERSPACE_3B3BD18430C5A642::Persisted_UserBlockedUserUpdated, USERSPACE_3B3BD18430C5A642::Persisted_BannedUserUpdated, USERSPACE_3B3BD18430C5A642::Persisted_UserDeleted, USERSPACE_3B3BD18430C5A642::Persisted_AuthKeyTokenPairDeleted, USERSPACE_3B3BD18430C5A642::Persisted_UIDAuthKeyPairDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CardDeleted, USERSPACE_3B3BD18430C5A642::Persisted_AuthorCardDeleted, USERSPACE_3B3BD18430C5A642::Persisted_AnswerDeleted, USERSPACE_3B3BD18430C5A642::Persisted_FavoriteDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CommentDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CommentLikeDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CardFlagAsInappropriateDeleted, USERSPACE_3B3BD18430C5A642::Persisted_CommentFlagAsInappropriateDeleted, USERSPACE_3B3BD18430C5A642::Persisted_NotificationDeleted, USERSPACE_3B3BD18430C5A642::Persisted_StarNotificationAlreadySentDeleted, USERSPACE_3B3BD18430C5A642::Persisted_UserReportedUserDeleted, USERSPACE_3B3BD18430C5A642::Persisted_UserBlockedUserDeleted, USERSPACE_3B3BD18430C5A642::Persisted_BannedUserDeleted>>& from,
                 CUSTOM_INTO_VARIANT_TYPE& into) {
    from.Call(USERSPACE_3B3BD18430C5A642_Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E_Cases<decltype(into), NAMESPACE, INTO, EVOLUTOR>(into));
  }
};

}  // namespace current::type_evolution
}  // namespace current

// Privileged types.
CURRENT_DERIVED_NAMESPACE(OldCTFOStorage, USERSPACE_3B3BD18430C5A642) {
  CURRENT_NAMESPACE_TYPE(TopLevelTransaction, current_userspace_3b3bd18430c5a642::Transaction_T9226272004857575759);
};  // CURRENT_NAMESPACE(OldCTFOStorage)

// clang-format on

#endif  // CURRENT_USERSPACE_3B3BD18430C5A642
