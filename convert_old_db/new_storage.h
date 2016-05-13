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

#ifndef NEW_STORAGE_H
#define NEW_STORAGE_H

#include "new_schema.h"

#include "../../Current/Storage/storage.h"
#include "../../Current/Storage/persister/sherlock.h"

CURRENT_STORAGE_FIELD_ENTRY(UnorderedDictionary, new_ctfo::User, Persisted_User);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedOneToUnorderedMany,
                            new_ctfo::AuthKeyTokenPair,
                            Persisted_AuthKeyTokenPair);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedOneToUnorderedMany, new_ctfo::UIDAuthKeyPair, Persisted_UIDAuthKeyPair);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedDictionary, new_ctfo::Card, Persisted_Card);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedOneToUnorderedMany, new_ctfo::AuthorCard, Persisted_AuthorCard);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedManyToUnorderedMany, new_ctfo::Answer, Persisted_Answer);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedManyToUnorderedMany, new_ctfo::Favorite, Persisted_Favorite);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedOneToUnorderedMany, new_ctfo::Comment, Persisted_Comment);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedManyToUnorderedMany, new_ctfo::CommentLike, Persisted_CommentLike);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedManyToUnorderedMany,
                            new_ctfo::CardFlagAsInappropriate,
                            Persisted_CardFlagAsInappropriate);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedManyToUnorderedMany,
                            new_ctfo::CommentFlagAsInappropriate,
                            Persisted_CommentFlagAsInappropriate);
CURRENT_STORAGE_FIELD_ENTRY(OrderedManyToOrderedMany, new_ctfo::Notification, Persisted_Notification);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedDictionary,
                            new_ctfo::StarNotificationAlreadySent,
                            Persisted_StarNotificationAlreadySent);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedManyToUnorderedMany,
                            new_ctfo::UserReportedUser,
                            Persisted_UserReportedUser);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedManyToUnorderedMany, new_ctfo::UserBlockedUser, Persisted_UserBlockedUser);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedDictionary, new_ctfo::BannedUser, Persisted_BannedUser);

CURRENT_STORAGE(NewCTFO) {
  CURRENT_STORAGE_FIELD(user, Persisted_User);
  CURRENT_STORAGE_FIELD(auth_token, Persisted_AuthKeyTokenPair);
  CURRENT_STORAGE_FIELD(uid_auth, Persisted_UIDAuthKeyPair);
  CURRENT_STORAGE_FIELD(card, Persisted_Card);
  CURRENT_STORAGE_FIELD(author_card, Persisted_AuthorCard);
  CURRENT_STORAGE_FIELD(answer, Persisted_Answer);
  CURRENT_STORAGE_FIELD(favorite, Persisted_Favorite);
  CURRENT_STORAGE_FIELD(comment, Persisted_Comment);
  CURRENT_STORAGE_FIELD(comment_like, Persisted_CommentLike);
  CURRENT_STORAGE_FIELD(flagged_card, Persisted_CardFlagAsInappropriate);
  CURRENT_STORAGE_FIELD(flagged_comment, Persisted_CommentFlagAsInappropriate);
  CURRENT_STORAGE_FIELD(notification, Persisted_Notification);
  CURRENT_STORAGE_FIELD(starred_notification_already_sent, Persisted_StarNotificationAlreadySent);
  CURRENT_STORAGE_FIELD(user_reported_user, Persisted_UserReportedUser);
  CURRENT_STORAGE_FIELD(user_blocked_user, Persisted_UserBlockedUser);
  CURRENT_STORAGE_FIELD(banned_user, Persisted_BannedUser);
};

#endif  // NEW_STORAGE_H
