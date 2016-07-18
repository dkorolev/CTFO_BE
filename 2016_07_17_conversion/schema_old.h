// The `current.h` file is the one from `https://github.com/C5T/Current`.
// Compile with `-std=c++11` or higher.

#ifndef CURRENT_USERSPACE_AA83BD4D7206705B
#define CURRENT_USERSPACE_AA83BD4D7206705B

#include "current.h"

// clang-format off

namespace current_userspace_aa83bd4d7206705b {

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

CURRENT_STRUCT(UserNotificationPlayerID) {
  CURRENT_FIELD(uid, UID);
  CURRENT_FIELD(player_id, std::string);
};
using T9207541387680683014 = UserNotificationPlayerID;

CURRENT_STRUCT(Persisted_UserNotificationPlayerIDUpdated) {
  CURRENT_FIELD(data, UserNotificationPlayerID);
};
using T9207186679306594142 = Persisted_UserNotificationPlayerIDUpdated;

CURRENT_STRUCT(PushNotificationsMarker) {
  CURRENT_FIELD(dummy_key, std::string);
  CURRENT_FIELD(last_pushed_notification_timestamp, std::chrono::microseconds);
  CURRENT_FIELD(notification_push_frozen_until_timestamp, std::chrono::microseconds);
};
using T9203263626333893460 = PushNotificationsMarker;

CURRENT_STRUCT(Persisted_PushNotificationsMarkerUpdated) {
  CURRENT_FIELD(data, PushNotificationsMarker);
};
using T9204009610109175183 = Persisted_PushNotificationsMarkerUpdated;

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

CURRENT_STRUCT(Persisted_UserNotificationPlayerIDDeleted) {
  CURRENT_FIELD(key, UID);
};
using T9209651556777130502 = Persisted_UserNotificationPlayerIDDeleted;

CURRENT_STRUCT(Persisted_PushNotificationsMarkerDeleted) {
  CURRENT_FIELD(key, std::string);
};
using T9202204059409325046 = Persisted_PushNotificationsMarkerDeleted;

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

CURRENT_VARIANT(Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E, Persisted_UserUpdated, Persisted_AuthKeyTokenPairUpdated, Persisted_UIDAuthKeyPairUpdated, Persisted_UserNotificationPlayerIDUpdated, Persisted_PushNotificationsMarkerUpdated, Persisted_CardUpdated, Persisted_AuthorCardUpdated, Persisted_AnswerUpdated, Persisted_FavoriteUpdated, Persisted_CommentUpdated, Persisted_CommentLikeUpdated, Persisted_CardFlagAsInappropriateUpdated, Persisted_CommentFlagAsInappropriateUpdated, Persisted_NotificationUpdated, Persisted_StarNotificationAlreadySentUpdated, Persisted_UserReportedUserUpdated, Persisted_UserBlockedUserUpdated, Persisted_BannedUserUpdated, Persisted_UserDeleted, Persisted_AuthKeyTokenPairDeleted, Persisted_UIDAuthKeyPairDeleted, Persisted_UserNotificationPlayerIDDeleted, Persisted_PushNotificationsMarkerDeleted, Persisted_CardDeleted, Persisted_AuthorCardDeleted, Persisted_AnswerDeleted, Persisted_FavoriteDeleted, Persisted_CommentDeleted, Persisted_CommentLikeDeleted, Persisted_CardFlagAsInappropriateDeleted, Persisted_CommentFlagAsInappropriateDeleted, Persisted_NotificationDeleted, Persisted_StarNotificationAlreadySentDeleted, Persisted_UserReportedUserDeleted, Persisted_UserBlockedUserDeleted, Persisted_BannedUserDeleted);
using T9220345170565313519 = Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E;

CURRENT_STRUCT(Transaction_T9220345170565313519) {
  CURRENT_FIELD(meta, TransactionMeta);
  CURRENT_FIELD(mutations, std::vector<Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E>);
};
using T9205024251036691492 = Transaction_T9220345170565313519;

}  // namespace current_userspace_aa83bd4d7206705b

CURRENT_NAMESPACE(USERSPACE_AA83BD4D7206705B) {
  CURRENT_NAMESPACE_TYPE(AUTH_TYPE, current_userspace_aa83bd4d7206705b::AUTH_TYPE);
  CURRENT_NAMESPACE_TYPE(ANSWER, current_userspace_aa83bd4d7206705b::ANSWER);
  CURRENT_NAMESPACE_TYPE(UID, current_userspace_aa83bd4d7206705b::UID);
  CURRENT_NAMESPACE_TYPE(OID, current_userspace_aa83bd4d7206705b::OID);
  CURRENT_NAMESPACE_TYPE(CID, current_userspace_aa83bd4d7206705b::CID);
  CURRENT_NAMESPACE_TYPE(Persisted_StarNotificationAlreadySentDeleted, current_userspace_aa83bd4d7206705b::Persisted_StarNotificationAlreadySentDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_FavoriteDeleted, current_userspace_aa83bd4d7206705b::Persisted_FavoriteDeleted);
  CURRENT_NAMESPACE_TYPE(StarNotificationAlreadySent, current_userspace_aa83bd4d7206705b::StarNotificationAlreadySent);
  CURRENT_NAMESPACE_TYPE(Persisted_AuthorCardDeleted, current_userspace_aa83bd4d7206705b::Persisted_AuthorCardDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_AnswerDeleted, current_userspace_aa83bd4d7206705b::Persisted_AnswerDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentDeleted, current_userspace_aa83bd4d7206705b::Persisted_CommentDeleted);
  CURRENT_NAMESPACE_TYPE(BannedUser, current_userspace_aa83bd4d7206705b::BannedUser);
  CURRENT_NAMESPACE_TYPE(AuthorCard, current_userspace_aa83bd4d7206705b::AuthorCard);
  CURRENT_NAMESPACE_TYPE(Persisted_UIDAuthKeyPairDeleted, current_userspace_aa83bd4d7206705b::Persisted_UIDAuthKeyPairDeleted);
  CURRENT_NAMESPACE_TYPE(Favorite, current_userspace_aa83bd4d7206705b::Favorite);
  CURRENT_NAMESPACE_TYPE(Notification, current_userspace_aa83bd4d7206705b::Notification);
  CURRENT_NAMESPACE_TYPE(Persisted_AuthKeyTokenPairDeleted, current_userspace_aa83bd4d7206705b::Persisted_AuthKeyTokenPairDeleted);
  CURRENT_NAMESPACE_TYPE(UIDAuthKeyPair, current_userspace_aa83bd4d7206705b::UIDAuthKeyPair);
  CURRENT_NAMESPACE_TYPE(Persisted_CardUpdated, current_userspace_aa83bd4d7206705b::Persisted_CardUpdated);
  CURRENT_NAMESPACE_TYPE(TransactionMeta, current_userspace_aa83bd4d7206705b::TransactionMeta);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentLikeDeleted, current_userspace_aa83bd4d7206705b::Persisted_CommentLikeDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentFlagAsInappropriateDeleted, current_userspace_aa83bd4d7206705b::Persisted_CommentFlagAsInappropriateDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_PushNotificationsMarkerDeleted, current_userspace_aa83bd4d7206705b::Persisted_PushNotificationsMarkerDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentUpdated, current_userspace_aa83bd4d7206705b::Persisted_CommentUpdated);
  CURRENT_NAMESPACE_TYPE(User, current_userspace_aa83bd4d7206705b::User);
  CURRENT_NAMESPACE_TYPE(PushNotificationsMarker, current_userspace_aa83bd4d7206705b::PushNotificationsMarker);
  CURRENT_NAMESPACE_TYPE(Card, current_userspace_aa83bd4d7206705b::Card);
  CURRENT_NAMESPACE_TYPE(Comment, current_userspace_aa83bd4d7206705b::Comment);
  CURRENT_NAMESPACE_TYPE(Persisted_CardFlagAsInappropriateDeleted, current_userspace_aa83bd4d7206705b::Persisted_CardFlagAsInappropriateDeleted);
  CURRENT_NAMESPACE_TYPE(AuthKeyTokenPair, current_userspace_aa83bd4d7206705b::AuthKeyTokenPair);
  CURRENT_NAMESPACE_TYPE(Persisted_UIDAuthKeyPairUpdated, current_userspace_aa83bd4d7206705b::Persisted_UIDAuthKeyPairUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_PushNotificationsMarkerUpdated, current_userspace_aa83bd4d7206705b::Persisted_PushNotificationsMarkerUpdated);
  CURRENT_NAMESPACE_TYPE(UserReportedUser, current_userspace_aa83bd4d7206705b::UserReportedUser);
  CURRENT_NAMESPACE_TYPE(UserBlockedUser, current_userspace_aa83bd4d7206705b::UserBlockedUser);
  CURRENT_NAMESPACE_TYPE(Color, current_userspace_aa83bd4d7206705b::Color);
  CURRENT_NAMESPACE_TYPE(Persisted_AuthorCardUpdated, current_userspace_aa83bd4d7206705b::Persisted_AuthorCardUpdated);
  CURRENT_NAMESPACE_TYPE(Transaction_T9220345170565313519, current_userspace_aa83bd4d7206705b::Transaction_T9220345170565313519);
  CURRENT_NAMESPACE_TYPE(Persisted_AnswerUpdated, current_userspace_aa83bd4d7206705b::Persisted_AnswerUpdated);
  CURRENT_NAMESPACE_TYPE(NotificationNewReplyToMyComment, current_userspace_aa83bd4d7206705b::NotificationNewReplyToMyComment);
  CURRENT_NAMESPACE_TYPE(NotificationMyCommentLiked, current_userspace_aa83bd4d7206705b::NotificationMyCommentLiked);
  CURRENT_NAMESPACE_TYPE(NotificationNewCommentOnCardIStarred, current_userspace_aa83bd4d7206705b::NotificationNewCommentOnCardIStarred);
  CURRENT_NAMESPACE_TYPE(NotificationMyCardNewComment, current_userspace_aa83bd4d7206705b::NotificationMyCardNewComment);
  CURRENT_NAMESPACE_TYPE(Persisted_CardFlagAsInappropriateUpdated, current_userspace_aa83bd4d7206705b::Persisted_CardFlagAsInappropriateUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_AuthKeyTokenPairUpdated, current_userspace_aa83bd4d7206705b::Persisted_AuthKeyTokenPairUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_FavoriteUpdated, current_userspace_aa83bd4d7206705b::Persisted_FavoriteUpdated);
  CURRENT_NAMESPACE_TYPE(NotificationMyCardStarred, current_userspace_aa83bd4d7206705b::NotificationMyCardStarred);
  CURRENT_NAMESPACE_TYPE(NotificationNewVotesOnMyCard, current_userspace_aa83bd4d7206705b::NotificationNewVotesOnMyCard);
  CURRENT_NAMESPACE_TYPE(Answer, current_userspace_aa83bd4d7206705b::Answer);
  CURRENT_NAMESPACE_TYPE(Persisted_NotificationUpdated, current_userspace_aa83bd4d7206705b::Persisted_NotificationUpdated);
  CURRENT_NAMESPACE_TYPE(AuthKey, current_userspace_aa83bd4d7206705b::AuthKey);
  CURRENT_NAMESPACE_TYPE(Persisted_UserNotificationPlayerIDUpdated, current_userspace_aa83bd4d7206705b::Persisted_UserNotificationPlayerIDUpdated);
  CURRENT_NAMESPACE_TYPE(UserNotificationPlayerID, current_userspace_aa83bd4d7206705b::UserNotificationPlayerID);
  CURRENT_NAMESPACE_TYPE(Persisted_UserBlockedUserUpdated, current_userspace_aa83bd4d7206705b::Persisted_UserBlockedUserUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_UserUpdated, current_userspace_aa83bd4d7206705b::Persisted_UserUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentFlagAsInappropriateUpdated, current_userspace_aa83bd4d7206705b::Persisted_CommentFlagAsInappropriateUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_CommentLikeUpdated, current_userspace_aa83bd4d7206705b::Persisted_CommentLikeUpdated);
  CURRENT_NAMESPACE_TYPE(CardFlagAsInappropriate, current_userspace_aa83bd4d7206705b::CardFlagAsInappropriate);
  CURRENT_NAMESPACE_TYPE(Persisted_NotificationDeleted, current_userspace_aa83bd4d7206705b::Persisted_NotificationDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_UserBlockedUserDeleted, current_userspace_aa83bd4d7206705b::Persisted_UserBlockedUserDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_UserReportedUserDeleted, current_userspace_aa83bd4d7206705b::Persisted_UserReportedUserDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_BannedUserUpdated, current_userspace_aa83bd4d7206705b::Persisted_BannedUserUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_UserReportedUserUpdated, current_userspace_aa83bd4d7206705b::Persisted_UserReportedUserUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_StarNotificationAlreadySentUpdated, current_userspace_aa83bd4d7206705b::Persisted_StarNotificationAlreadySentUpdated);
  CURRENT_NAMESPACE_TYPE(Persisted_UserNotificationPlayerIDDeleted, current_userspace_aa83bd4d7206705b::Persisted_UserNotificationPlayerIDDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_UserDeleted, current_userspace_aa83bd4d7206705b::Persisted_UserDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_BannedUserDeleted, current_userspace_aa83bd4d7206705b::Persisted_BannedUserDeleted);
  CURRENT_NAMESPACE_TYPE(Persisted_CardDeleted, current_userspace_aa83bd4d7206705b::Persisted_CardDeleted);
  CURRENT_NAMESPACE_TYPE(CommentLike, current_userspace_aa83bd4d7206705b::CommentLike);
  CURRENT_NAMESPACE_TYPE(CommentFlagAsInappropriate, current_userspace_aa83bd4d7206705b::CommentFlagAsInappropriate);
  CURRENT_NAMESPACE_TYPE(Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E, current_userspace_aa83bd4d7206705b::Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E);
  CURRENT_NAMESPACE_TYPE(Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E, current_userspace_aa83bd4d7206705b::Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E);
};  // CURRENT_NAMESPACE(USERSPACE_AA83BD4D7206705B)

namespace current {
namespace type_evolution {

// Default evolution for `CURRENT_ENUM(AUTH_TYPE)`.
#ifndef DEFAULT_EVOLUTION_755EFE367CF2578281B8E60C05F001DF5962CE554C3886EABF1565E4E82EB941  // USERSPACE_AA83BD4D7206705B::AUTH_TYPE
#define DEFAULT_EVOLUTION_755EFE367CF2578281B8E60C05F001DF5962CE554C3886EABF1565E4E82EB941  // USERSPACE_AA83BD4D7206705B::AUTH_TYPE
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, USERSPACE_AA83BD4D7206705B::AUTH_TYPE, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_AA83BD4D7206705B::AUTH_TYPE from,
                 typename INTO::AUTH_TYPE& into) {
    into = static_cast<typename INTO::AUTH_TYPE>(from);
  }
};
#endif

// Default evolution for `CURRENT_ENUM(ANSWER)`.
#ifndef DEFAULT_EVOLUTION_FE6EAFF23F5B24D1EE38BC303754AD27326377383352C93FE6C4FBE781DF6F6A  // USERSPACE_AA83BD4D7206705B::ANSWER
#define DEFAULT_EVOLUTION_FE6EAFF23F5B24D1EE38BC303754AD27326377383352C93FE6C4FBE781DF6F6A  // USERSPACE_AA83BD4D7206705B::ANSWER
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, USERSPACE_AA83BD4D7206705B::ANSWER, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_AA83BD4D7206705B::ANSWER from,
                 typename INTO::ANSWER& into) {
    into = static_cast<typename INTO::ANSWER>(from);
  }
};
#endif

// Default evolution for `CURRENT_ENUM(UID)`.
#ifndef DEFAULT_EVOLUTION_6FCF3C83632169F8DC09F390D6215BD35D3AC7E30FDCA6AF4D6B4CD9F4933C06  // USERSPACE_AA83BD4D7206705B::UID
#define DEFAULT_EVOLUTION_6FCF3C83632169F8DC09F390D6215BD35D3AC7E30FDCA6AF4D6B4CD9F4933C06  // USERSPACE_AA83BD4D7206705B::UID
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, USERSPACE_AA83BD4D7206705B::UID, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_AA83BD4D7206705B::UID from,
                 typename INTO::UID& into) {
    into = static_cast<typename INTO::UID>(from);
  }
};
#endif

// Default evolution for `CURRENT_ENUM(OID)`.
#ifndef DEFAULT_EVOLUTION_25E8D64ECABC423A04D9002931DAB06F9A5D1BEDDABB33D08C1B90909B59B84D  // USERSPACE_AA83BD4D7206705B::OID
#define DEFAULT_EVOLUTION_25E8D64ECABC423A04D9002931DAB06F9A5D1BEDDABB33D08C1B90909B59B84D  // USERSPACE_AA83BD4D7206705B::OID
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, USERSPACE_AA83BD4D7206705B::OID, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_AA83BD4D7206705B::OID from,
                 typename INTO::OID& into) {
    into = static_cast<typename INTO::OID>(from);
  }
};
#endif

// Default evolution for `CURRENT_ENUM(CID)`.
#ifndef DEFAULT_EVOLUTION_420FE089F40C69A470DE749CC07E346978D830113A42B66267EACAF25AB656BC  // USERSPACE_AA83BD4D7206705B::CID
#define DEFAULT_EVOLUTION_420FE089F40C69A470DE749CC07E346978D830113A42B66267EACAF25AB656BC  // USERSPACE_AA83BD4D7206705B::CID
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, USERSPACE_AA83BD4D7206705B::CID, EVOLUTOR> {
  template <typename INTO>
  static void Go(USERSPACE_AA83BD4D7206705B::CID from,
                 typename INTO::CID& into) {
    into = static_cast<typename INTO::CID>(from);
  }
};
#endif

// Default evolution for struct `Persisted_StarNotificationAlreadySentDeleted`.
#ifndef DEFAULT_EVOLUTION_D66C2A7C01B64F214F88A071B40700C28C09216A12F2916429FA2B7EDBF42FCA  // typename USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentDeleted
#define DEFAULT_EVOLUTION_D66C2A7C01B64F214F88A071B40700C28C09216A12F2916429FA2B7EDBF42FCA  // typename USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentDeleted& from,
                 typename INTO::Persisted_StarNotificationAlreadySentDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_StarNotificationAlreadySentDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_FavoriteDeleted`.
#ifndef DEFAULT_EVOLUTION_318C525138D6E32D4BF1F8B91FE295C2C88FDFB91E410ADB663F0A408D14E299  // typename USERSPACE_AA83BD4D7206705B::Persisted_FavoriteDeleted
#define DEFAULT_EVOLUTION_318C525138D6E32D4BF1F8B91FE295C2C88FDFB91E410ADB663F0A408D14E299  // typename USERSPACE_AA83BD4D7206705B::Persisted_FavoriteDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_FavoriteDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_FavoriteDeleted& from,
                 typename INTO::Persisted_FavoriteDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_FavoriteDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `StarNotificationAlreadySent`.
#ifndef DEFAULT_EVOLUTION_ED77DAF807C3AE083CC87A2A72CB30CBCC1CBED23A8A336D6BADC14BBCA56798  // typename USERSPACE_AA83BD4D7206705B::StarNotificationAlreadySent
#define DEFAULT_EVOLUTION_ED77DAF807C3AE083CC87A2A72CB30CBCC1CBED23A8A336D6BADC14BBCA56798  // typename USERSPACE_AA83BD4D7206705B::StarNotificationAlreadySent
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::StarNotificationAlreadySent, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::StarNotificationAlreadySent& from,
                 typename INTO::StarNotificationAlreadySent& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::StarNotificationAlreadySent>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_AuthorCardDeleted`.
#ifndef DEFAULT_EVOLUTION_F25A99B06363B0AE7951143025F79088ACB6487DF10AA0B5AA569CA262A31C27  // typename USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardDeleted
#define DEFAULT_EVOLUTION_F25A99B06363B0AE7951143025F79088ACB6487DF10AA0B5AA569CA262A31C27  // typename USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardDeleted& from,
                 typename INTO::Persisted_AuthorCardDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_AuthorCardDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_AnswerDeleted`.
#ifndef DEFAULT_EVOLUTION_8F715438181011C4C26818EAC7C75E1F40F67863257F3FB071887B7E92A586B1  // typename USERSPACE_AA83BD4D7206705B::Persisted_AnswerDeleted
#define DEFAULT_EVOLUTION_8F715438181011C4C26818EAC7C75E1F40F67863257F3FB071887B7E92A586B1  // typename USERSPACE_AA83BD4D7206705B::Persisted_AnswerDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_AnswerDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_AnswerDeleted& from,
                 typename INTO::Persisted_AnswerDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_AnswerDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_CommentDeleted`.
#ifndef DEFAULT_EVOLUTION_4B88055BFAB90B309E0A1C32A878FC1B2CC0F5338B9120C951402570E3AB9922  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentDeleted
#define DEFAULT_EVOLUTION_4B88055BFAB90B309E0A1C32A878FC1B2CC0F5338B9120C951402570E3AB9922  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CommentDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CommentDeleted& from,
                 typename INTO::Persisted_CommentDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CommentDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `BannedUser`.
#ifndef DEFAULT_EVOLUTION_EFCC9B7E2191344E26816E99A5DDB2754AA4519FCA83396D9A0DD233673C09FB  // typename USERSPACE_AA83BD4D7206705B::BannedUser
#define DEFAULT_EVOLUTION_EFCC9B7E2191344E26816E99A5DDB2754AA4519FCA83396D9A0DD233673C09FB  // typename USERSPACE_AA83BD4D7206705B::BannedUser
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::BannedUser, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::BannedUser& from,
                 typename INTO::BannedUser& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::BannedUser>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.banned_uid), EVOLUTOR>::template Go<INTO>(from.banned_uid, into.banned_uid);
  }
};
#endif

// Default evolution for struct `AuthorCard`.
#ifndef DEFAULT_EVOLUTION_B74FCC8EDA39D90ED374F13A5608EA0EA0F8D12A092F5A9D0AE840AAEEBA65F7  // typename USERSPACE_AA83BD4D7206705B::AuthorCard
#define DEFAULT_EVOLUTION_B74FCC8EDA39D90ED374F13A5608EA0EA0F8D12A092F5A9D0AE840AAEEBA65F7  // typename USERSPACE_AA83BD4D7206705B::AuthorCard
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::AuthorCard, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::AuthorCard& from,
                 typename INTO::AuthorCard& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::AuthorCard>::value == 3,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
  }
};
#endif

// Default evolution for struct `Persisted_UIDAuthKeyPairDeleted`.
#ifndef DEFAULT_EVOLUTION_1988EBD6B3BAF2A8D7FC413835FCC128BD80D7B959C74464BA94CDC02445CE10  // typename USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairDeleted
#define DEFAULT_EVOLUTION_1988EBD6B3BAF2A8D7FC413835FCC128BD80D7B959C74464BA94CDC02445CE10  // typename USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairDeleted& from,
                 typename INTO::Persisted_UIDAuthKeyPairDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UIDAuthKeyPairDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Favorite`.
#ifndef DEFAULT_EVOLUTION_FB865E7A18356E2F77CADB62D17B4BFF6E8AFB68E36E184D1DF25DC34F8D8F16  // typename USERSPACE_AA83BD4D7206705B::Favorite
#define DEFAULT_EVOLUTION_FB865E7A18356E2F77CADB62D17B4BFF6E8AFB68E36E184D1DF25DC34F8D8F16  // typename USERSPACE_AA83BD4D7206705B::Favorite
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Favorite, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Favorite& from,
                 typename INTO::Favorite& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Favorite>::value == 4,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
      Evolve<FROM, decltype(from.favorited), EVOLUTOR>::template Go<INTO>(from.favorited, into.favorited);
  }
};
#endif

// Default evolution for struct `Notification`.
#ifndef DEFAULT_EVOLUTION_9726071E8D6D0D0A04F3205312D72D0BF0282D5B536349B907B7A353565231A5  // typename USERSPACE_AA83BD4D7206705B::Notification
#define DEFAULT_EVOLUTION_9726071E8D6D0D0A04F3205312D72D0BF0282D5B536349B907B7A353565231A5  // typename USERSPACE_AA83BD4D7206705B::Notification
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Notification, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Notification& from,
                 typename INTO::Notification& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Notification>::value == 3,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.timestamp), EVOLUTOR>::template Go<INTO>(from.timestamp, into.timestamp);
      Evolve<FROM, decltype(from.notification), EVOLUTOR>::template Go<INTO>(from.notification, into.notification);
  }
};
#endif

// Default evolution for struct `Persisted_AuthKeyTokenPairDeleted`.
#ifndef DEFAULT_EVOLUTION_B2CFFEF2BDDC2C3AECBC9DACB8A675BDC581E846F06CD20CE30FC63F7359580E  // typename USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairDeleted
#define DEFAULT_EVOLUTION_B2CFFEF2BDDC2C3AECBC9DACB8A675BDC581E846F06CD20CE30FC63F7359580E  // typename USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairDeleted& from,
                 typename INTO::Persisted_AuthKeyTokenPairDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_AuthKeyTokenPairDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `UIDAuthKeyPair`.
#ifndef DEFAULT_EVOLUTION_F9CF439312048B3420B04E5FB08EE1A97F1806437C91B4216AEE5FF1D27AFC65  // typename USERSPACE_AA83BD4D7206705B::UIDAuthKeyPair
#define DEFAULT_EVOLUTION_F9CF439312048B3420B04E5FB08EE1A97F1806437C91B4216AEE5FF1D27AFC65  // typename USERSPACE_AA83BD4D7206705B::UIDAuthKeyPair
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::UIDAuthKeyPair, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::UIDAuthKeyPair& from,
                 typename INTO::UIDAuthKeyPair& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::UIDAuthKeyPair>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.auth_key), EVOLUTOR>::template Go<INTO>(from.auth_key, into.auth_key);
  }
};
#endif

// Default evolution for struct `Persisted_CardUpdated`.
#ifndef DEFAULT_EVOLUTION_D7017CCFD29245C58A0E9EF847FFCEE5105C6625CB943BAB341811834022F8FD  // typename USERSPACE_AA83BD4D7206705B::Persisted_CardUpdated
#define DEFAULT_EVOLUTION_D7017CCFD29245C58A0E9EF847FFCEE5105C6625CB943BAB341811834022F8FD  // typename USERSPACE_AA83BD4D7206705B::Persisted_CardUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CardUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CardUpdated& from,
                 typename INTO::Persisted_CardUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CardUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `TransactionMeta`.
#ifndef DEFAULT_EVOLUTION_997777A423C5D8A9BE1D10F3B89DF1BE932AB0CB4C21F91634F311051FA48BA6  // typename USERSPACE_AA83BD4D7206705B::TransactionMeta
#define DEFAULT_EVOLUTION_997777A423C5D8A9BE1D10F3B89DF1BE932AB0CB4C21F91634F311051FA48BA6  // typename USERSPACE_AA83BD4D7206705B::TransactionMeta
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::TransactionMeta, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::TransactionMeta& from,
                 typename INTO::TransactionMeta& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::TransactionMeta>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.timestamp), EVOLUTOR>::template Go<INTO>(from.timestamp, into.timestamp);
      Evolve<FROM, decltype(from.fields), EVOLUTOR>::template Go<INTO>(from.fields, into.fields);
  }
};
#endif

// Default evolution for struct `Persisted_CommentLikeDeleted`.
#ifndef DEFAULT_EVOLUTION_CBAF7DF5FA43BA3CD1B91132B5023F1183BC88379642705282AB3302A47C6C63  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeDeleted
#define DEFAULT_EVOLUTION_CBAF7DF5FA43BA3CD1B91132B5023F1183BC88379642705282AB3302A47C6C63  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeDeleted& from,
                 typename INTO::Persisted_CommentLikeDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CommentLikeDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_CommentFlagAsInappropriateDeleted`.
#ifndef DEFAULT_EVOLUTION_335027AF71FAD18469E51435BA04CC8CBCA6D420983C09E87A11C475CB19541B  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateDeleted
#define DEFAULT_EVOLUTION_335027AF71FAD18469E51435BA04CC8CBCA6D420983C09E87A11C475CB19541B  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateDeleted& from,
                 typename INTO::Persisted_CommentFlagAsInappropriateDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CommentFlagAsInappropriateDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_PushNotificationsMarkerDeleted`.
#ifndef DEFAULT_EVOLUTION_0F690C0F960811F73047106F35032F98F6FD6B131A31D48A7DE956311CC4F232  // typename USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerDeleted
#define DEFAULT_EVOLUTION_0F690C0F960811F73047106F35032F98F6FD6B131A31D48A7DE956311CC4F232  // typename USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerDeleted& from,
                 typename INTO::Persisted_PushNotificationsMarkerDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_PushNotificationsMarkerDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_CommentUpdated`.
#ifndef DEFAULT_EVOLUTION_66656F17C6C89055C4CF6E9C462BF42F2C820CDAFAF7505C3B4CB95EACEA1AF9  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentUpdated
#define DEFAULT_EVOLUTION_66656F17C6C89055C4CF6E9C462BF42F2C820CDAFAF7505C3B4CB95EACEA1AF9  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CommentUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CommentUpdated& from,
                 typename INTO::Persisted_CommentUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CommentUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `User`.
#ifndef DEFAULT_EVOLUTION_27C7737B1C6A43D953644674C82EBA1A4E89ACD4474A3B4DFFA04B6291ECC8AA  // typename USERSPACE_AA83BD4D7206705B::User
#define DEFAULT_EVOLUTION_27C7737B1C6A43D953644674C82EBA1A4E89ACD4474A3B4DFFA04B6291ECC8AA  // typename USERSPACE_AA83BD4D7206705B::User
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::User, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::User& from,
                 typename INTO::User& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::User>::value == 4,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
      Evolve<FROM, decltype(from.level), EVOLUTOR>::template Go<INTO>(from.level, into.level);
      Evolve<FROM, decltype(from.score), EVOLUTOR>::template Go<INTO>(from.score, into.score);
  }
};
#endif

// Default evolution for struct `PushNotificationsMarker`.
#ifndef DEFAULT_EVOLUTION_71CD62395EB4656BA2B0B5AA2F08B2E5D3BFB9EDE811FB08FA0FDDEB2D42E092  // typename USERSPACE_AA83BD4D7206705B::PushNotificationsMarker
#define DEFAULT_EVOLUTION_71CD62395EB4656BA2B0B5AA2F08B2E5D3BFB9EDE811FB08FA0FDDEB2D42E092  // typename USERSPACE_AA83BD4D7206705B::PushNotificationsMarker
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::PushNotificationsMarker, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::PushNotificationsMarker& from,
                 typename INTO::PushNotificationsMarker& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::PushNotificationsMarker>::value == 3,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.dummy_key), EVOLUTOR>::template Go<INTO>(from.dummy_key, into.dummy_key);
      Evolve<FROM, decltype(from.last_pushed_notification_timestamp), EVOLUTOR>::template Go<INTO>(from.last_pushed_notification_timestamp, into.last_pushed_notification_timestamp);
      Evolve<FROM, decltype(from.notification_push_frozen_until_timestamp), EVOLUTOR>::template Go<INTO>(from.notification_push_frozen_until_timestamp, into.notification_push_frozen_until_timestamp);
  }
};
#endif

// Default evolution for struct `Card`.
#ifndef DEFAULT_EVOLUTION_28BA8A0D7AA3606B7358FFDC92847BCA16F9DBFCBF09863055E016953502C447  // typename USERSPACE_AA83BD4D7206705B::Card
#define DEFAULT_EVOLUTION_28BA8A0D7AA3606B7358FFDC92847BCA16F9DBFCBF09863055E016953502C447  // typename USERSPACE_AA83BD4D7206705B::Card
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Card, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Card& from,
                 typename INTO::Card& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Card>::value == 8,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
      Evolve<FROM, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
      Evolve<FROM, decltype(from.color), EVOLUTOR>::template Go<INTO>(from.color, into.color);
      Evolve<FROM, decltype(from.ctfo_count), EVOLUTOR>::template Go<INTO>(from.ctfo_count, into.ctfo_count);
      Evolve<FROM, decltype(from.tfu_count), EVOLUTOR>::template Go<INTO>(from.tfu_count, into.tfu_count);
      Evolve<FROM, decltype(from.skip_count), EVOLUTOR>::template Go<INTO>(from.skip_count, into.skip_count);
      Evolve<FROM, decltype(from.startup_index), EVOLUTOR>::template Go<INTO>(from.startup_index, into.startup_index);
  }
};
#endif

// Default evolution for struct `Comment`.
#ifndef DEFAULT_EVOLUTION_1668A475D8D34B3BCDA2CAC1784FAFA9AD8A6A552051A23FC8A272BCE7945DCC  // typename USERSPACE_AA83BD4D7206705B::Comment
#define DEFAULT_EVOLUTION_1668A475D8D34B3BCDA2CAC1784FAFA9AD8A6A552051A23FC8A272BCE7945DCC  // typename USERSPACE_AA83BD4D7206705B::Comment
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Comment, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Comment& from,
                 typename INTO::Comment& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Comment>::value == 6,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<FROM, decltype(from.us), EVOLUTOR>::template Go<INTO>(from.us, into.us);
      Evolve<FROM, decltype(from.parent_oid), EVOLUTOR>::template Go<INTO>(from.parent_oid, into.parent_oid);
      Evolve<FROM, decltype(from.author_uid), EVOLUTOR>::template Go<INTO>(from.author_uid, into.author_uid);
      Evolve<FROM, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};
#endif

// Default evolution for struct `Persisted_CardFlagAsInappropriateDeleted`.
#ifndef DEFAULT_EVOLUTION_8020E4DAFEE2C3F340C8B0E8A38FD67A692392D03AF3DDB103C2817BA88482FA  // typename USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateDeleted
#define DEFAULT_EVOLUTION_8020E4DAFEE2C3F340C8B0E8A38FD67A692392D03AF3DDB103C2817BA88482FA  // typename USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateDeleted& from,
                 typename INTO::Persisted_CardFlagAsInappropriateDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CardFlagAsInappropriateDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `AuthKeyTokenPair`.
#ifndef DEFAULT_EVOLUTION_F6E607F379321DB29F390A98164353F4346BC76DDD6FC68CE37202ACFFA22098  // typename USERSPACE_AA83BD4D7206705B::AuthKeyTokenPair
#define DEFAULT_EVOLUTION_F6E607F379321DB29F390A98164353F4346BC76DDD6FC68CE37202ACFFA22098  // typename USERSPACE_AA83BD4D7206705B::AuthKeyTokenPair
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::AuthKeyTokenPair, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::AuthKeyTokenPair& from,
                 typename INTO::AuthKeyTokenPair& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::AuthKeyTokenPair>::value == 3,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.auth_key), EVOLUTOR>::template Go<INTO>(from.auth_key, into.auth_key);
      Evolve<FROM, decltype(from.token), EVOLUTOR>::template Go<INTO>(from.token, into.token);
      Evolve<FROM, decltype(from.valid), EVOLUTOR>::template Go<INTO>(from.valid, into.valid);
  }
};
#endif

// Default evolution for struct `Persisted_UIDAuthKeyPairUpdated`.
#ifndef DEFAULT_EVOLUTION_65F93D5479C9F43ACC16995981687E87C61A35620BDAEBA3C615F2BC57A54007  // typename USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairUpdated
#define DEFAULT_EVOLUTION_65F93D5479C9F43ACC16995981687E87C61A35620BDAEBA3C615F2BC57A54007  // typename USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairUpdated& from,
                 typename INTO::Persisted_UIDAuthKeyPairUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UIDAuthKeyPairUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_PushNotificationsMarkerUpdated`.
#ifndef DEFAULT_EVOLUTION_009C10E35C8A516016A5C72E0EC2AA0CB8F700805807E5043D02F2C3EB5BAC7D  // typename USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerUpdated
#define DEFAULT_EVOLUTION_009C10E35C8A516016A5C72E0EC2AA0CB8F700805807E5043D02F2C3EB5BAC7D  // typename USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerUpdated& from,
                 typename INTO::Persisted_PushNotificationsMarkerUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_PushNotificationsMarkerUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `UserReportedUser`.
#ifndef DEFAULT_EVOLUTION_16A672C1CB44F7DA62803EE18EE792DD4601E6A33884D608670FE1214E3C2A6D  // typename USERSPACE_AA83BD4D7206705B::UserReportedUser
#define DEFAULT_EVOLUTION_16A672C1CB44F7DA62803EE18EE792DD4601E6A33884D608670FE1214E3C2A6D  // typename USERSPACE_AA83BD4D7206705B::UserReportedUser
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::UserReportedUser, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::UserReportedUser& from,
                 typename INTO::UserReportedUser& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::UserReportedUser>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.who), EVOLUTOR>::template Go<INTO>(from.who, into.who);
      Evolve<FROM, decltype(from.whom), EVOLUTOR>::template Go<INTO>(from.whom, into.whom);
  }
};
#endif

// Default evolution for struct `UserBlockedUser`.
#ifndef DEFAULT_EVOLUTION_F085D897E5A030B5EAE6D0B9385D7D8FE93066ADE0821EB10BEC06F8C7C902AC  // typename USERSPACE_AA83BD4D7206705B::UserBlockedUser
#define DEFAULT_EVOLUTION_F085D897E5A030B5EAE6D0B9385D7D8FE93066ADE0821EB10BEC06F8C7C902AC  // typename USERSPACE_AA83BD4D7206705B::UserBlockedUser
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::UserBlockedUser, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::UserBlockedUser& from,
                 typename INTO::UserBlockedUser& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::UserBlockedUser>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.who), EVOLUTOR>::template Go<INTO>(from.who, into.who);
      Evolve<FROM, decltype(from.whom), EVOLUTOR>::template Go<INTO>(from.whom, into.whom);
  }
};
#endif

// Default evolution for struct `Color`.
#ifndef DEFAULT_EVOLUTION_C4ACF1DB80D79FAD181513FA17C889F48CB9339F1800A01DB6A269605125741D  // typename USERSPACE_AA83BD4D7206705B::Color
#define DEFAULT_EVOLUTION_C4ACF1DB80D79FAD181513FA17C889F48CB9339F1800A01DB6A269605125741D  // typename USERSPACE_AA83BD4D7206705B::Color
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Color, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Color& from,
                 typename INTO::Color& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Color>::value == 3,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.red), EVOLUTOR>::template Go<INTO>(from.red, into.red);
      Evolve<FROM, decltype(from.green), EVOLUTOR>::template Go<INTO>(from.green, into.green);
      Evolve<FROM, decltype(from.blue), EVOLUTOR>::template Go<INTO>(from.blue, into.blue);
  }
};
#endif

// Default evolution for struct `Persisted_AuthorCardUpdated`.
#ifndef DEFAULT_EVOLUTION_7BE53F098AF25E96E3175AB9750D4567C53F70B86B5B12BED733A8EBBE6D750D  // typename USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardUpdated
#define DEFAULT_EVOLUTION_7BE53F098AF25E96E3175AB9750D4567C53F70B86B5B12BED733A8EBBE6D750D  // typename USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardUpdated& from,
                 typename INTO::Persisted_AuthorCardUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_AuthorCardUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Transaction_T9220345170565313519`.
#ifndef DEFAULT_EVOLUTION_9EC381642246645139C3BC4A2550C8509D04D9E6823B7A0FD30409F6545E0AEA  // typename USERSPACE_AA83BD4D7206705B::Transaction_T9220345170565313519
#define DEFAULT_EVOLUTION_9EC381642246645139C3BC4A2550C8509D04D9E6823B7A0FD30409F6545E0AEA  // typename USERSPACE_AA83BD4D7206705B::Transaction_T9220345170565313519
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Transaction_T9220345170565313519, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Transaction_T9220345170565313519& from,
                 typename INTO::Transaction_T9220345170565313519& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Transaction_T9220345170565313519>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.meta), EVOLUTOR>::template Go<INTO>(from.meta, into.meta);
      Evolve<FROM, decltype(from.mutations), EVOLUTOR>::template Go<INTO>(from.mutations, into.mutations);
  }
};
#endif

// Default evolution for struct `Persisted_AnswerUpdated`.
#ifndef DEFAULT_EVOLUTION_2111220BC924A35B2F7D03832ACCDACD14CAE39F318301ECC23613A8B356F876  // typename USERSPACE_AA83BD4D7206705B::Persisted_AnswerUpdated
#define DEFAULT_EVOLUTION_2111220BC924A35B2F7D03832ACCDACD14CAE39F318301ECC23613A8B356F876  // typename USERSPACE_AA83BD4D7206705B::Persisted_AnswerUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_AnswerUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_AnswerUpdated& from,
                 typename INTO::Persisted_AnswerUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_AnswerUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `NotificationNewReplyToMyComment`.
#ifndef DEFAULT_EVOLUTION_9BC646C5938BB34A2228792B3BB5476AB7BE8D0D578B19E0A2D9630C91DB5C65  // typename USERSPACE_AA83BD4D7206705B::NotificationNewReplyToMyComment
#define DEFAULT_EVOLUTION_9BC646C5938BB34A2228792B3BB5476AB7BE8D0D578B19E0A2D9630C91DB5C65  // typename USERSPACE_AA83BD4D7206705B::NotificationNewReplyToMyComment
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::NotificationNewReplyToMyComment, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::NotificationNewReplyToMyComment& from,
                 typename INTO::NotificationNewReplyToMyComment& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::NotificationNewReplyToMyComment>::value == 4,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<FROM, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};
#endif

// Default evolution for struct `NotificationMyCommentLiked`.
#ifndef DEFAULT_EVOLUTION_9E2044EC3A0518DA8C8571A593937818E667DA265878F24B5F4497EFCD1C2C2B  // typename USERSPACE_AA83BD4D7206705B::NotificationMyCommentLiked
#define DEFAULT_EVOLUTION_9E2044EC3A0518DA8C8571A593937818E667DA265878F24B5F4497EFCD1C2C2B  // typename USERSPACE_AA83BD4D7206705B::NotificationMyCommentLiked
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::NotificationMyCommentLiked, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::NotificationMyCommentLiked& from,
                 typename INTO::NotificationMyCommentLiked& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::NotificationMyCommentLiked>::value == 4,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<FROM, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};
#endif

// Default evolution for struct `NotificationNewCommentOnCardIStarred`.
#ifndef DEFAULT_EVOLUTION_18C77CA89C9430F8395012F207E1F52709D4AC8AD03E330BBCD56ABA9B50FB1E  // typename USERSPACE_AA83BD4D7206705B::NotificationNewCommentOnCardIStarred
#define DEFAULT_EVOLUTION_18C77CA89C9430F8395012F207E1F52709D4AC8AD03E330BBCD56ABA9B50FB1E  // typename USERSPACE_AA83BD4D7206705B::NotificationNewCommentOnCardIStarred
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::NotificationNewCommentOnCardIStarred, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::NotificationNewCommentOnCardIStarred& from,
                 typename INTO::NotificationNewCommentOnCardIStarred& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::NotificationNewCommentOnCardIStarred>::value == 4,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<FROM, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};
#endif

// Default evolution for struct `NotificationMyCardNewComment`.
#ifndef DEFAULT_EVOLUTION_6A8F6D6917E2245E3F760F55D00306C0AE732DB28152CE0DDF8D6A59CDF3F519  // typename USERSPACE_AA83BD4D7206705B::NotificationMyCardNewComment
#define DEFAULT_EVOLUTION_6A8F6D6917E2245E3F760F55D00306C0AE732DB28152CE0DDF8D6A59CDF3F519  // typename USERSPACE_AA83BD4D7206705B::NotificationMyCardNewComment
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::NotificationMyCardNewComment, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::NotificationMyCardNewComment& from,
                 typename INTO::NotificationMyCardNewComment& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::NotificationMyCardNewComment>::value == 4,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<FROM, decltype(from.text), EVOLUTOR>::template Go<INTO>(from.text, into.text);
  }
};
#endif

// Default evolution for struct `Persisted_CardFlagAsInappropriateUpdated`.
#ifndef DEFAULT_EVOLUTION_19BAAA4C0A39ECF11346391C45E93FAD1C7463E4A0760C671B57B983E778E548  // typename USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateUpdated
#define DEFAULT_EVOLUTION_19BAAA4C0A39ECF11346391C45E93FAD1C7463E4A0760C671B57B983E778E548  // typename USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateUpdated& from,
                 typename INTO::Persisted_CardFlagAsInappropriateUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CardFlagAsInappropriateUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_AuthKeyTokenPairUpdated`.
#ifndef DEFAULT_EVOLUTION_35186FA778F48AF8A2598B6FD7DF217BA337FF08A72CB87BE2291453E157D97A  // typename USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairUpdated
#define DEFAULT_EVOLUTION_35186FA778F48AF8A2598B6FD7DF217BA337FF08A72CB87BE2291453E157D97A  // typename USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairUpdated& from,
                 typename INTO::Persisted_AuthKeyTokenPairUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_AuthKeyTokenPairUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_FavoriteUpdated`.
#ifndef DEFAULT_EVOLUTION_2FC3DBC5C5E9EFFDE5CDE278A884E04328147D78FB0BBD20F07EBE2727C0CF35  // typename USERSPACE_AA83BD4D7206705B::Persisted_FavoriteUpdated
#define DEFAULT_EVOLUTION_2FC3DBC5C5E9EFFDE5CDE278A884E04328147D78FB0BBD20F07EBE2727C0CF35  // typename USERSPACE_AA83BD4D7206705B::Persisted_FavoriteUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_FavoriteUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_FavoriteUpdated& from,
                 typename INTO::Persisted_FavoriteUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_FavoriteUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `NotificationMyCardStarred`.
#ifndef DEFAULT_EVOLUTION_54A9C63F2E5267480A874A6797260434409BD173F7BBCC2FD4257FBC48B8DF61  // typename USERSPACE_AA83BD4D7206705B::NotificationMyCardStarred
#define DEFAULT_EVOLUTION_54A9C63F2E5267480A874A6797260434409BD173F7BBCC2FD4257FBC48B8DF61  // typename USERSPACE_AA83BD4D7206705B::NotificationMyCardStarred
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::NotificationMyCardStarred, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::NotificationMyCardStarred& from,
                 typename INTO::NotificationMyCardStarred& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::NotificationMyCardStarred>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
  }
};
#endif

// Default evolution for struct `NotificationNewVotesOnMyCard`.
#ifndef DEFAULT_EVOLUTION_F30EFA7BCF69202EDA23E11CC7396021EBEAA854CBF0B972DCF9108097475233  // typename USERSPACE_AA83BD4D7206705B::NotificationNewVotesOnMyCard
#define DEFAULT_EVOLUTION_F30EFA7BCF69202EDA23E11CC7396021EBEAA854CBF0B972DCF9108097475233  // typename USERSPACE_AA83BD4D7206705B::NotificationNewVotesOnMyCard
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::NotificationNewVotesOnMyCard, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::NotificationNewVotesOnMyCard& from,
                 typename INTO::NotificationNewVotesOnMyCard& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::NotificationNewVotesOnMyCard>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
  }
};
#endif

// Default evolution for struct `Answer`.
#ifndef DEFAULT_EVOLUTION_B7C8B42E3C2196EA50495EB1F2B5BD7D632469A0B0902963BD1D01C2B1895A52  // typename USERSPACE_AA83BD4D7206705B::Answer
#define DEFAULT_EVOLUTION_B7C8B42E3C2196EA50495EB1F2B5BD7D632469A0B0902963BD1D01C2B1895A52  // typename USERSPACE_AA83BD4D7206705B::Answer
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Answer, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Answer& from,
                 typename INTO::Answer& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Answer>::value == 3,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.answer), EVOLUTOR>::template Go<INTO>(from.answer, into.answer);
  }
};
#endif

// Default evolution for struct `Persisted_NotificationUpdated`.
#ifndef DEFAULT_EVOLUTION_5B3A7925BF4811D7AEE46D431EBCA42BEBBBE0A096F39C43A1D7C1B4EDF73D7F  // typename USERSPACE_AA83BD4D7206705B::Persisted_NotificationUpdated
#define DEFAULT_EVOLUTION_5B3A7925BF4811D7AEE46D431EBCA42BEBBBE0A096F39C43A1D7C1B4EDF73D7F  // typename USERSPACE_AA83BD4D7206705B::Persisted_NotificationUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_NotificationUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_NotificationUpdated& from,
                 typename INTO::Persisted_NotificationUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_NotificationUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `AuthKey`.
#ifndef DEFAULT_EVOLUTION_85E34E49323CBAE102F11F721E1C67D87F7AC15E4105A62AA7371546F401F8D9  // typename USERSPACE_AA83BD4D7206705B::AuthKey
#define DEFAULT_EVOLUTION_85E34E49323CBAE102F11F721E1C67D87F7AC15E4105A62AA7371546F401F8D9  // typename USERSPACE_AA83BD4D7206705B::AuthKey
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::AuthKey, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::AuthKey& from,
                 typename INTO::AuthKey& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::AuthKey>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
      Evolve<FROM, decltype(from.type), EVOLUTOR>::template Go<INTO>(from.type, into.type);
  }
};
#endif

// Default evolution for struct `Persisted_UserNotificationPlayerIDUpdated`.
#ifndef DEFAULT_EVOLUTION_CECDAF082DD43A38D4BE5DD5CF444FAA6230BF05BE186D859883B4395EA0C09C  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDUpdated
#define DEFAULT_EVOLUTION_CECDAF082DD43A38D4BE5DD5CF444FAA6230BF05BE186D859883B4395EA0C09C  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDUpdated& from,
                 typename INTO::Persisted_UserNotificationPlayerIDUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UserNotificationPlayerIDUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `UserNotificationPlayerID`.
#ifndef DEFAULT_EVOLUTION_6A777EC4E4A3C64F3778843A373BF8F6FF8007D409D0A83FB661829040D66CCC  // typename USERSPACE_AA83BD4D7206705B::UserNotificationPlayerID
#define DEFAULT_EVOLUTION_6A777EC4E4A3C64F3778843A373BF8F6FF8007D409D0A83FB661829040D66CCC  // typename USERSPACE_AA83BD4D7206705B::UserNotificationPlayerID
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::UserNotificationPlayerID, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::UserNotificationPlayerID& from,
                 typename INTO::UserNotificationPlayerID& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::UserNotificationPlayerID>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
      Evolve<FROM, decltype(from.player_id), EVOLUTOR>::template Go<INTO>(from.player_id, into.player_id);
  }
};
#endif

// Default evolution for struct `Persisted_UserBlockedUserUpdated`.
#ifndef DEFAULT_EVOLUTION_5CB9FBFF6D389DD8583A515F51DFC13175B4224B570F3E2CA82C8B17AFC0D9D5  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserUpdated
#define DEFAULT_EVOLUTION_5CB9FBFF6D389DD8583A515F51DFC13175B4224B570F3E2CA82C8B17AFC0D9D5  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserUpdated& from,
                 typename INTO::Persisted_UserBlockedUserUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UserBlockedUserUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_UserUpdated`.
#ifndef DEFAULT_EVOLUTION_B0B1511553DB9E5BC1BCFBDBF35BB33FFE5F8A11C1C7D6BC86CEA417DA9506E3  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserUpdated
#define DEFAULT_EVOLUTION_B0B1511553DB9E5BC1BCFBDBF35BB33FFE5F8A11C1C7D6BC86CEA417DA9506E3  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UserUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UserUpdated& from,
                 typename INTO::Persisted_UserUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UserUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_CommentFlagAsInappropriateUpdated`.
#ifndef DEFAULT_EVOLUTION_145995FB7944F01EF26C874B1088EC38D7E4B7A107DA49B00C45709D453EB1B5  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateUpdated
#define DEFAULT_EVOLUTION_145995FB7944F01EF26C874B1088EC38D7E4B7A107DA49B00C45709D453EB1B5  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateUpdated& from,
                 typename INTO::Persisted_CommentFlagAsInappropriateUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CommentFlagAsInappropriateUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_CommentLikeUpdated`.
#ifndef DEFAULT_EVOLUTION_32333CE7CC3ED8B5309B9F4025B3941BF0D55EA4C291250A444B211EF8033BDE  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeUpdated
#define DEFAULT_EVOLUTION_32333CE7CC3ED8B5309B9F4025B3941BF0D55EA4C291250A444B211EF8033BDE  // typename USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeUpdated& from,
                 typename INTO::Persisted_CommentLikeUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CommentLikeUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `CardFlagAsInappropriate`.
#ifndef DEFAULT_EVOLUTION_B1D33B908BFE741B816A0D504560A360EFD5298CF4920E21E83C5ADD4E4E0C4B  // typename USERSPACE_AA83BD4D7206705B::CardFlagAsInappropriate
#define DEFAULT_EVOLUTION_B1D33B908BFE741B816A0D504560A360EFD5298CF4920E21E83C5ADD4E4E0C4B  // typename USERSPACE_AA83BD4D7206705B::CardFlagAsInappropriate
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::CardFlagAsInappropriate, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::CardFlagAsInappropriate& from,
                 typename INTO::CardFlagAsInappropriate& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::CardFlagAsInappropriate>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.cid), EVOLUTOR>::template Go<INTO>(from.cid, into.cid);
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
  }
};
#endif

// Default evolution for struct `Persisted_NotificationDeleted`.
#ifndef DEFAULT_EVOLUTION_30D849B8B5EF9621D81AA684C4749C4D25B8E26A4C96893C62E0092772FD5BD7  // typename USERSPACE_AA83BD4D7206705B::Persisted_NotificationDeleted
#define DEFAULT_EVOLUTION_30D849B8B5EF9621D81AA684C4749C4D25B8E26A4C96893C62E0092772FD5BD7  // typename USERSPACE_AA83BD4D7206705B::Persisted_NotificationDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_NotificationDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_NotificationDeleted& from,
                 typename INTO::Persisted_NotificationDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_NotificationDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_UserBlockedUserDeleted`.
#ifndef DEFAULT_EVOLUTION_BB894EA405BA48FC0AC4A469431ADAB3A3B7DAABD43666DB09FBFEBB623C9855  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserDeleted
#define DEFAULT_EVOLUTION_BB894EA405BA48FC0AC4A469431ADAB3A3B7DAABD43666DB09FBFEBB623C9855  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserDeleted& from,
                 typename INTO::Persisted_UserBlockedUserDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UserBlockedUserDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_UserReportedUserDeleted`.
#ifndef DEFAULT_EVOLUTION_20902E10BFB1E9F556327687A87A1C1D838D9BF443C13CB8B27F4E5106A6B494  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserDeleted
#define DEFAULT_EVOLUTION_20902E10BFB1E9F556327687A87A1C1D838D9BF443C13CB8B27F4E5106A6B494  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserDeleted& from,
                 typename INTO::Persisted_UserReportedUserDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UserReportedUserDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_BannedUserUpdated`.
#ifndef DEFAULT_EVOLUTION_37B9BC2BF79985DB6D43BD2C09DE6417D21C4C5E20018FA695F6C6179EF083A6  // typename USERSPACE_AA83BD4D7206705B::Persisted_BannedUserUpdated
#define DEFAULT_EVOLUTION_37B9BC2BF79985DB6D43BD2C09DE6417D21C4C5E20018FA695F6C6179EF083A6  // typename USERSPACE_AA83BD4D7206705B::Persisted_BannedUserUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_BannedUserUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_BannedUserUpdated& from,
                 typename INTO::Persisted_BannedUserUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_BannedUserUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_UserReportedUserUpdated`.
#ifndef DEFAULT_EVOLUTION_3E341E0857D3C8E6D90CD0126AF08D2A6FD589D0DE8F59CC7437F3D140874A00  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserUpdated
#define DEFAULT_EVOLUTION_3E341E0857D3C8E6D90CD0126AF08D2A6FD589D0DE8F59CC7437F3D140874A00  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserUpdated& from,
                 typename INTO::Persisted_UserReportedUserUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UserReportedUserUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_StarNotificationAlreadySentUpdated`.
#ifndef DEFAULT_EVOLUTION_7C9EE09AB695429F89B278F83D0FD78B3A609D74B0359454B99A9EC4FEFDE76A  // typename USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentUpdated
#define DEFAULT_EVOLUTION_7C9EE09AB695429F89B278F83D0FD78B3A609D74B0359454B99A9EC4FEFDE76A  // typename USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentUpdated
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentUpdated, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentUpdated& from,
                 typename INTO::Persisted_StarNotificationAlreadySentUpdated& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_StarNotificationAlreadySentUpdated>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.data), EVOLUTOR>::template Go<INTO>(from.data, into.data);
  }
};
#endif

// Default evolution for struct `Persisted_UserNotificationPlayerIDDeleted`.
#ifndef DEFAULT_EVOLUTION_1CDE3D54B0F123EBE582BD38EDD8F5249B8D66BCC0FA8164FAD63C01BDBB0BC5  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDDeleted
#define DEFAULT_EVOLUTION_1CDE3D54B0F123EBE582BD38EDD8F5249B8D66BCC0FA8164FAD63C01BDBB0BC5  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDDeleted& from,
                 typename INTO::Persisted_UserNotificationPlayerIDDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UserNotificationPlayerIDDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_UserDeleted`.
#ifndef DEFAULT_EVOLUTION_5A9C16446AA72D28F729F77500D7A9CCC25DA5E632BF5848BD67655E0CFECD1A  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserDeleted
#define DEFAULT_EVOLUTION_5A9C16446AA72D28F729F77500D7A9CCC25DA5E632BF5848BD67655E0CFECD1A  // typename USERSPACE_AA83BD4D7206705B::Persisted_UserDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_UserDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_UserDeleted& from,
                 typename INTO::Persisted_UserDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_UserDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_BannedUserDeleted`.
#ifndef DEFAULT_EVOLUTION_E8A6189D5E6D2C8105F3DF66223CBB29479C6B5E019420336DE604F48E1DCBD5  // typename USERSPACE_AA83BD4D7206705B::Persisted_BannedUserDeleted
#define DEFAULT_EVOLUTION_E8A6189D5E6D2C8105F3DF66223CBB29479C6B5E019420336DE604F48E1DCBD5  // typename USERSPACE_AA83BD4D7206705B::Persisted_BannedUserDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_BannedUserDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_BannedUserDeleted& from,
                 typename INTO::Persisted_BannedUserDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_BannedUserDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `Persisted_CardDeleted`.
#ifndef DEFAULT_EVOLUTION_14D0870C5269F33583790C53AC915254EA8494D5478A2F70125C2B38EAF6B8A1  // typename USERSPACE_AA83BD4D7206705B::Persisted_CardDeleted
#define DEFAULT_EVOLUTION_14D0870C5269F33583790C53AC915254EA8494D5478A2F70125C2B38EAF6B8A1  // typename USERSPACE_AA83BD4D7206705B::Persisted_CardDeleted
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::Persisted_CardDeleted, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::Persisted_CardDeleted& from,
                 typename INTO::Persisted_CardDeleted& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::Persisted_CardDeleted>::value == 1,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.key), EVOLUTOR>::template Go<INTO>(from.key, into.key);
  }
};
#endif

// Default evolution for struct `CommentLike`.
#ifndef DEFAULT_EVOLUTION_0FF8D8F7707FC84854D257449B8D2C6FB992C045B01B6B61D0FFF3FD679600A3  // typename USERSPACE_AA83BD4D7206705B::CommentLike
#define DEFAULT_EVOLUTION_0FF8D8F7707FC84854D257449B8D2C6FB992C045B01B6B61D0FFF3FD679600A3  // typename USERSPACE_AA83BD4D7206705B::CommentLike
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::CommentLike, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::CommentLike& from,
                 typename INTO::CommentLike& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::CommentLike>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
  }
};
#endif

// Default evolution for struct `CommentFlagAsInappropriate`.
#ifndef DEFAULT_EVOLUTION_A2801ED8AB8771EE2DA37378D20F94D143C102A9BDCEA21F03AD3914A2C1447E  // typename USERSPACE_AA83BD4D7206705B::CommentFlagAsInappropriate
#define DEFAULT_EVOLUTION_A2801ED8AB8771EE2DA37378D20F94D143C102A9BDCEA21F03AD3914A2C1447E  // typename USERSPACE_AA83BD4D7206705B::CommentFlagAsInappropriate
template <typename FROM, typename EVOLUTOR>
struct Evolve<FROM, typename USERSPACE_AA83BD4D7206705B::CommentFlagAsInappropriate, EVOLUTOR> {
  template <typename INTO,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const typename USERSPACE_AA83BD4D7206705B::CommentFlagAsInappropriate& from,
                 typename INTO::CommentFlagAsInappropriate& into) {
      static_assert(::current::reflection::FieldCounter<typename INTO::CommentFlagAsInappropriate>::value == 2,
                    "Custom evolutor required.");
      Evolve<FROM, decltype(from.oid), EVOLUTOR>::template Go<INTO>(from.oid, into.oid);
      Evolve<FROM, decltype(from.uid), EVOLUTOR>::template Go<INTO>(from.uid, into.uid);
  }
};
#endif

// Default evolution for `Variant<Persisted_UserUpdated, Persisted_AuthKeyTokenPairUpdated, Persisted_UIDAuthKeyPairUpdated, Persisted_UserNotificationPlayerIDUpdated, Persisted_PushNotificationsMarkerUpdated, Persisted_CardUpdated, Persisted_AuthorCardUpdated, Persisted_AnswerUpdated, Persisted_FavoriteUpdated, Persisted_CommentUpdated, Persisted_CommentLikeUpdated, Persisted_CardFlagAsInappropriateUpdated, Persisted_CommentFlagAsInappropriateUpdated, Persisted_NotificationUpdated, Persisted_StarNotificationAlreadySentUpdated, Persisted_UserReportedUserUpdated, Persisted_UserBlockedUserUpdated, Persisted_BannedUserUpdated, Persisted_UserDeleted, Persisted_AuthKeyTokenPairDeleted, Persisted_UIDAuthKeyPairDeleted, Persisted_UserNotificationPlayerIDDeleted, Persisted_PushNotificationsMarkerDeleted, Persisted_CardDeleted, Persisted_AuthorCardDeleted, Persisted_AnswerDeleted, Persisted_FavoriteDeleted, Persisted_CommentDeleted, Persisted_CommentLikeDeleted, Persisted_CardFlagAsInappropriateDeleted, Persisted_CommentFlagAsInappropriateDeleted, Persisted_NotificationDeleted, Persisted_StarNotificationAlreadySentDeleted, Persisted_UserReportedUserDeleted, Persisted_UserBlockedUserDeleted, Persisted_BannedUserDeleted>`.
#ifndef DEFAULT_EVOLUTION_3D6B71FA951DA61C2119E078F18D893CF373C2F478F873A38B397036896000DB  // ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_AA83BD4D7206705B::Persisted_UserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDUpdated, USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CardUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AnswerUpdated, USERSPACE_AA83BD4D7206705B::Persisted_FavoriteUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateUpdated, USERSPACE_AA83BD4D7206705B::Persisted_NotificationUpdated, USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_BannedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDDeleted, USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CardDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AnswerDeleted, USERSPACE_AA83BD4D7206705B::Persisted_FavoriteDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateDeleted, USERSPACE_AA83BD4D7206705B::Persisted_NotificationDeleted, USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_BannedUserDeleted>>
#define DEFAULT_EVOLUTION_3D6B71FA951DA61C2119E078F18D893CF373C2F478F873A38B397036896000DB  // ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_AA83BD4D7206705B::Persisted_UserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDUpdated, USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CardUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AnswerUpdated, USERSPACE_AA83BD4D7206705B::Persisted_FavoriteUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateUpdated, USERSPACE_AA83BD4D7206705B::Persisted_NotificationUpdated, USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_BannedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDDeleted, USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CardDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AnswerDeleted, USERSPACE_AA83BD4D7206705B::Persisted_FavoriteDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateDeleted, USERSPACE_AA83BD4D7206705B::Persisted_NotificationDeleted, USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_BannedUserDeleted>>
template <typename DST, typename FROM_NAMESPACE, typename INTO, typename EVOLUTOR>
struct USERSPACE_AA83BD4D7206705B_Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E_Cases {
  DST& into;
  explicit USERSPACE_AA83BD4D7206705B_Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E_Cases(DST& into) : into(into) {}
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
  void operator()(const typename FROM_NAMESPACE::Persisted_UserNotificationPlayerIDUpdated& value) const {
    using into_t = typename INTO::Persisted_UserNotificationPlayerIDUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UserNotificationPlayerIDUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_PushNotificationsMarkerUpdated& value) const {
    using into_t = typename INTO::Persisted_PushNotificationsMarkerUpdated;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_PushNotificationsMarkerUpdated, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
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
  void operator()(const typename FROM_NAMESPACE::Persisted_UserNotificationPlayerIDDeleted& value) const {
    using into_t = typename INTO::Persisted_UserNotificationPlayerIDDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_UserNotificationPlayerIDDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
  }
  void operator()(const typename FROM_NAMESPACE::Persisted_PushNotificationsMarkerDeleted& value) const {
    using into_t = typename INTO::Persisted_PushNotificationsMarkerDeleted;
    into = into_t();
    Evolve<FROM_NAMESPACE, typename FROM_NAMESPACE::Persisted_PushNotificationsMarkerDeleted, EVOLUTOR>::template Go<INTO>(value, Value<into_t>(into));
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
template <typename FROM, typename EVOLUTOR, typename VARIANT_NAME_HELPER>
struct Evolve<FROM, ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_AA83BD4D7206705B::Persisted_UserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDUpdated, USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CardUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AnswerUpdated, USERSPACE_AA83BD4D7206705B::Persisted_FavoriteUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateUpdated, USERSPACE_AA83BD4D7206705B::Persisted_NotificationUpdated, USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_BannedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDDeleted, USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CardDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AnswerDeleted, USERSPACE_AA83BD4D7206705B::Persisted_FavoriteDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateDeleted, USERSPACE_AA83BD4D7206705B::Persisted_NotificationDeleted, USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_BannedUserDeleted>>, EVOLUTOR> {
  template <typename INTO,
            typename CUSTOM_INTO_VARIANT_TYPE,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_AA83BD4D7206705B::Persisted_UserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDUpdated, USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CardUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardUpdated, USERSPACE_AA83BD4D7206705B::Persisted_AnswerUpdated, USERSPACE_AA83BD4D7206705B::Persisted_FavoriteUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateUpdated, USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateUpdated, USERSPACE_AA83BD4D7206705B::Persisted_NotificationUpdated, USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_BannedUserUpdated, USERSPACE_AA83BD4D7206705B::Persisted_UserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AuthKeyTokenPairDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UIDAuthKeyPairDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserNotificationPlayerIDDeleted, USERSPACE_AA83BD4D7206705B::Persisted_PushNotificationsMarkerDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CardDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AuthorCardDeleted, USERSPACE_AA83BD4D7206705B::Persisted_AnswerDeleted, USERSPACE_AA83BD4D7206705B::Persisted_FavoriteDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentLikeDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CardFlagAsInappropriateDeleted, USERSPACE_AA83BD4D7206705B::Persisted_CommentFlagAsInappropriateDeleted, USERSPACE_AA83BD4D7206705B::Persisted_NotificationDeleted, USERSPACE_AA83BD4D7206705B::Persisted_StarNotificationAlreadySentDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserReportedUserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_UserBlockedUserDeleted, USERSPACE_AA83BD4D7206705B::Persisted_BannedUserDeleted>>& from,
                 CUSTOM_INTO_VARIANT_TYPE& into) {
    from.Call(USERSPACE_AA83BD4D7206705B_Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E_Cases<decltype(into), FROM, INTO, EVOLUTOR>(into));
  }
};
#endif

// Default evolution for `Variant<NotificationMyCardNewComment, NotificationNewReplyToMyComment, NotificationMyCommentLiked, NotificationNewCommentOnCardIStarred, NotificationMyCardStarred, NotificationNewVotesOnMyCard>`.
#ifndef DEFAULT_EVOLUTION_A84700AAD78BADD508A3BEDBA08D064856C085DF6D7705B9D0DB5FD2517D22D1  // ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_AA83BD4D7206705B::NotificationMyCardNewComment, USERSPACE_AA83BD4D7206705B::NotificationNewReplyToMyComment, USERSPACE_AA83BD4D7206705B::NotificationMyCommentLiked, USERSPACE_AA83BD4D7206705B::NotificationNewCommentOnCardIStarred, USERSPACE_AA83BD4D7206705B::NotificationMyCardStarred, USERSPACE_AA83BD4D7206705B::NotificationNewVotesOnMyCard>>
#define DEFAULT_EVOLUTION_A84700AAD78BADD508A3BEDBA08D064856C085DF6D7705B9D0DB5FD2517D22D1  // ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_AA83BD4D7206705B::NotificationMyCardNewComment, USERSPACE_AA83BD4D7206705B::NotificationNewReplyToMyComment, USERSPACE_AA83BD4D7206705B::NotificationMyCommentLiked, USERSPACE_AA83BD4D7206705B::NotificationNewCommentOnCardIStarred, USERSPACE_AA83BD4D7206705B::NotificationMyCardStarred, USERSPACE_AA83BD4D7206705B::NotificationNewVotesOnMyCard>>
template <typename DST, typename FROM_NAMESPACE, typename INTO, typename EVOLUTOR>
struct USERSPACE_AA83BD4D7206705B_Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E_Cases {
  DST& into;
  explicit USERSPACE_AA83BD4D7206705B_Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E_Cases(DST& into) : into(into) {}
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
template <typename FROM, typename EVOLUTOR, typename VARIANT_NAME_HELPER>
struct Evolve<FROM, ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_AA83BD4D7206705B::NotificationMyCardNewComment, USERSPACE_AA83BD4D7206705B::NotificationNewReplyToMyComment, USERSPACE_AA83BD4D7206705B::NotificationMyCommentLiked, USERSPACE_AA83BD4D7206705B::NotificationNewCommentOnCardIStarred, USERSPACE_AA83BD4D7206705B::NotificationMyCardStarred, USERSPACE_AA83BD4D7206705B::NotificationNewVotesOnMyCard>>, EVOLUTOR> {
  template <typename INTO,
            typename CUSTOM_INTO_VARIANT_TYPE,
            class CHECK = FROM,
            class = std::enable_if_t<::current::is_same_or_base_of<USERSPACE_AA83BD4D7206705B, CHECK>::value>>
  static void Go(const ::current::VariantImpl<VARIANT_NAME_HELPER, TypeListImpl<USERSPACE_AA83BD4D7206705B::NotificationMyCardNewComment, USERSPACE_AA83BD4D7206705B::NotificationNewReplyToMyComment, USERSPACE_AA83BD4D7206705B::NotificationMyCommentLiked, USERSPACE_AA83BD4D7206705B::NotificationNewCommentOnCardIStarred, USERSPACE_AA83BD4D7206705B::NotificationMyCardStarred, USERSPACE_AA83BD4D7206705B::NotificationNewVotesOnMyCard>>& from,
                 CUSTOM_INTO_VARIANT_TYPE& into) {
    from.Call(USERSPACE_AA83BD4D7206705B_Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E_Cases<decltype(into), FROM, INTO, EVOLUTOR>(into));
  }
};
#endif

}  // namespace current::type_evolution
}  // namespace current

#if 0  // Boilerplate evolutors.

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_StarNotificationAlreadySentDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_FavoriteDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, StarNotificationAlreadySent, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_AuthorCardDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_AnswerDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CommentDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, BannedUser, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.banned_uid, into.banned_uid);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, AuthorCard, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.us, into.us);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UIDAuthKeyPairDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Favorite, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.us, into.us);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.favorited, into.favorited);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Notification, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.timestamp, into.timestamp);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.notification, into.notification);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_AuthKeyTokenPairDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, UIDAuthKeyPair, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.auth_key, into.auth_key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CardUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, TransactionMeta, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.timestamp, into.timestamp);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.fields, into.fields);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CommentLikeDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CommentFlagAsInappropriateDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_PushNotificationsMarkerDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CommentUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, User, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.us, into.us);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.level, into.level);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.score, into.score);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, PushNotificationsMarker, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.dummy_key, into.dummy_key);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.last_pushed_notification_timestamp, into.last_pushed_notification_timestamp);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.notification_push_frozen_until_timestamp, into.notification_push_frozen_until_timestamp);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Card, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.us, into.us);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.text, into.text);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.color, into.color);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.ctfo_count, into.ctfo_count);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.tfu_count, into.tfu_count);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.skip_count, into.skip_count);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.startup_index, into.startup_index);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Comment, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.oid, into.oid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.us, into.us);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.parent_oid, into.parent_oid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.author_uid, into.author_uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.text, into.text);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CardFlagAsInappropriateDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, AuthKeyTokenPair, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.auth_key, into.auth_key);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.token, into.token);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.valid, into.valid);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UIDAuthKeyPairUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_PushNotificationsMarkerUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, UserReportedUser, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.who, into.who);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.whom, into.whom);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, UserBlockedUser, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.who, into.who);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.whom, into.whom);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Color, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.red, into.red);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.green, into.green);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.blue, into.blue);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_AuthorCardUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Transaction_T9220345170565313519, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.meta, into.meta);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.mutations, into.mutations);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_AnswerUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, NotificationNewReplyToMyComment, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.oid, into.oid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.text, into.text);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, NotificationMyCommentLiked, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.oid, into.oid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.text, into.text);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, NotificationNewCommentOnCardIStarred, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.oid, into.oid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.text, into.text);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, NotificationMyCardNewComment, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.oid, into.oid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.text, into.text);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CardFlagAsInappropriateUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_AuthKeyTokenPairUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_FavoriteUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, NotificationMyCardStarred, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, NotificationNewVotesOnMyCard, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Answer, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.answer, into.answer);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_NotificationUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, AuthKey, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.type, into.type);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UserNotificationPlayerIDUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, UserNotificationPlayerID, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.player_id, into.player_id);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UserBlockedUserUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UserUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CommentFlagAsInappropriateUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CommentLikeUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, CardFlagAsInappropriate, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.cid, into.cid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_NotificationDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UserBlockedUserDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UserReportedUserDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_BannedUserUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UserReportedUserUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_StarNotificationAlreadySentUpdated, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.data, into.data);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UserNotificationPlayerIDDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_UserDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_BannedUserDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, Persisted_CardDeleted, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.key, into.key);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, CommentLike, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.oid, into.oid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
});

CURRENT_TYPE_EVOLUTOR(CustomEvolutor, OldCTFOStorage, CommentFlagAsInappropriate, {
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.oid, into.oid);
  CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from.uid, into.uid);
});

CURRENT_TYPE_EVOLUTOR_VARIANT(CustomEvolutor, OldCTFOStorage, Variant_B_Persisted_UserUpdated_Persisted_AuthKeyTokenPairUpdated_Persisted_UIDAuthKeyPairUpdated_Persisted_UserNotificationPlayerIDUpdated_Persisted_PushNotificationsMarkerUpdated_Persisted_CardUpdated_Persisted_AuthorCardUpdated_Persisted_AnswerUpdated_Persisted_FavoriteUpdated_Persisted_CommentUpdated_Persisted_CommentLikeUpdated_Persisted_CardFlagAsInappropriateUpdated_Persisted_CommentFlagAsInappropriateUpdated_Persisted_NotificationUpdated_Persisted_StarNotificationAlreadySentUpdated_Persisted_UserReportedUserUpdated_Persisted_UserBlockedUserUpdated_Persisted_BannedUserUpdated_Persisted_UserDeleted_Persisted_AuthKeyTokenPairDeleted_Persisted_UIDAuthKeyPairDeleted_Persisted_UserNotificationPlayerIDDeleted_Persisted_PushNotificationsMarkerDeleted_Persisted_CardDeleted_Persisted_AuthorCardDeleted_Persisted_AnswerDeleted_Persisted_FavoriteDeleted_Persisted_CommentDeleted_Persisted_CommentLikeDeleted_Persisted_CardFlagAsInappropriateDeleted_Persisted_CommentFlagAsInappropriateDeleted_Persisted_NotificationDeleted_Persisted_StarNotificationAlreadySentDeleted_Persisted_UserReportedUserDeleted_Persisted_UserBlockedUserDeleted_Persisted_BannedUserDeleted_E, CustomDestinationNamespace) {
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UserUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_AuthKeyTokenPairUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UIDAuthKeyPairUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UserNotificationPlayerIDUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_PushNotificationsMarkerUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CardUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_AuthorCardUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_AnswerUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_FavoriteUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CommentUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CommentLikeUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CardFlagAsInappropriateUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CommentFlagAsInappropriateUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_NotificationUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_StarNotificationAlreadySentUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UserReportedUserUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UserBlockedUserUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_BannedUserUpdated, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UserDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_AuthKeyTokenPairDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UIDAuthKeyPairDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UserNotificationPlayerIDDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_PushNotificationsMarkerDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CardDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_AuthorCardDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_AnswerDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_FavoriteDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CommentDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CommentLikeDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CardFlagAsInappropriateDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_CommentFlagAsInappropriateDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_NotificationDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_StarNotificationAlreadySentDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UserReportedUserDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_UserBlockedUserDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(Persisted_BannedUserDeleted, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
};

CURRENT_TYPE_EVOLUTOR_VARIANT(CustomEvolutor, OldCTFOStorage, Variant_B_NotificationMyCardNewComment_NotificationNewReplyToMyComment_NotificationMyCommentLiked_NotificationNewCommentOnCardIStarred_NotificationMyCardStarred_NotificationNewVotesOnMyCard_E, CustomDestinationNamespace) {
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(NotificationMyCardNewComment, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(NotificationNewReplyToMyComment, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(NotificationMyCommentLiked, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(NotificationNewCommentOnCardIStarred, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(NotificationMyCardStarred, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
  CURRENT_TYPE_EVOLUTOR_NATURAL_VARIANT_CASE(NotificationNewVotesOnMyCard, CURRENT_NATURAL_EVOLVE(OldCTFOStorage, CustomDestinationNamespace, from, into));
};

#endif  // Boilerplate evolutors.

// Privileged types.
CURRENT_DERIVED_NAMESPACE(OldCTFOStorage, USERSPACE_AA83BD4D7206705B) {
  CURRENT_NAMESPACE_TYPE(TopLevelTransaction, current_userspace_aa83bd4d7206705b::Transaction_T9220345170565313519);
};  // CURRENT_NAMESPACE(OldCTFOStorage)

// clang-format on

#endif  // CURRENT_USERSPACE_AA83BD4D7206705B
