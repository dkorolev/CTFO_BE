/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Dmitry "Dima" Korolev <dmitry.korolev@gmail.com>
          (c) 2016 Maxim Zhurovich <zhurovich@gmail.com>

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

#ifndef NEW_SCHEMA_PUBLIC_H
#define NEW_SCHEMA_PUBLIC_H

#include "new_schema_base.h"

// Data structures for generating RESTful responses.
CURRENT_STRUCT(ResponseUserEntry) {
  CURRENT_FIELD(uid, std::string, "uINVALID");    // User id, format 'u01XXX...'.
  CURRENT_FIELD(token, std::string, "");          // User token.
  CURRENT_FIELD(level, uint8_t, 0u);              // User level, [0, 9].
  CURRENT_FIELD(score, uint64_t, 0u);             // User score.
  CURRENT_FIELD(next_level_score, uint32_t, 0u);  // Score value when user is promoted to the next level.
  CURRENT_FIELD(banned, bool, false);             // Whether the user is banned.
};

CURRENT_STRUCT(ResponseCardEntry) {
  CURRENT_FIELD(cid, std::string, "cINVALID");         // Card id, format 'c02XXX...'.
  CURRENT_FIELD(author_uid, std::string, "uINVALID");  // The author of this comment.
  CURRENT_FIELD(text, std::string, "");                // Card text.
  CURRENT_FIELD(us, std::chrono::microseconds, 0);     // Card timestamp, microseconds from epoch.
  CURRENT_FIELD(color, Color);                         // Card color.
  CURRENT_FIELD(relevance, double, 0.0);               // Card relevance for particular user, [0.0, 1.0].
  CURRENT_FIELD(ctfo_score, uint32_t, 0u);             // Number of points, which user gets for "CTFO" answer.
  CURRENT_FIELD(tfu_score, uint32_t, 0u);              // Number of points, which user gets for "TFU" answer.
  CURRENT_FIELD(ctfo_count, uint32_t, 0u);             // Number of users, who said "CTFO" on this card.
  CURRENT_FIELD(tfu_count, uint32_t, 0u);              // Number of users, who said "TFU" on this card.
  CURRENT_FIELD(skip_count, uint32_t, 0u);             // Number of users, who said "SKIP" on this card.
  CURRENT_FIELD(vote, std::string, "");                // "CTFO", "TFU", or empty string.
  CURRENT_FIELD(favorited, bool, false);               // True if the current user has favorited this card.
  CURRENT_FIELD(is_my_card, bool, false);              // True if this card has been created by this user.
  CURRENT_FIELD(number_of_comments, uint32_t, 0u);     // The total number of comments left for this card.
};

// Favorites response schema.
CURRENT_STRUCT(ResponseFavs) {
  CURRENT_FIELD(us, std::chrono::microseconds, 0);       // Server timestamp, microseconds from epoch.
  CURRENT_FIELD(user, ResponseUserEntry);                // User information.
  CURRENT_FIELD(cards, std::vector<ResponseCardEntry>);  // Favorited cards.
};

// "My cards" response schema. Yes, it's the same as favorites. -- D.K.
CURRENT_STRUCT(ResponseMyCards) {
  CURRENT_FIELD(us, std::chrono::microseconds, 0);       // Server timestamp, microseconds from epoch.
  CURRENT_FIELD(user, ResponseUserEntry);                // User information.
  CURRENT_FIELD(cards, std::vector<ResponseCardEntry>);  // Cards created by this user.
};

// Schema for the POST request to add a new card.
CURRENT_STRUCT(AddCardRequest) {
  CURRENT_FIELD(text, std::string, "");  // Plain text.
  CURRENT_FIELD(color, Color);           // Color.
};

// A shortened version of `AddCardRequest`.
CURRENT_STRUCT(AddCardShortRequest) {
  CURRENT_FIELD(text, std::string, "");  // Plain text.
};

// Schema for the response of the POST request to add a new card.
CURRENT_STRUCT(AddCardResponse) {
  CURRENT_FIELD(us, std::chrono::microseconds, 0);
  CURRENT_FIELD(cid, std::string, "");
};

// Schema for the response of the DELETE request for a card.
CURRENT_STRUCT(DeleteCardResponse) { CURRENT_FIELD(us, std::chrono::microseconds, 0); };

// Schema for the POST request to add a new comment.
CURRENT_STRUCT(AddCommentRequest) {
  CURRENT_FIELD(text, std::string, "");  // Plain text.
  CURRENT_FIELD(parent_oid, std::string, "");
};

// A shortened version of `AddCommentRequest`.
CURRENT_STRUCT(AddCommentShortRequest) {
  CURRENT_FIELD(text, std::string, "");  // Plain text.
};

// Schema for the response of the POST request to add a new comment.
CURRENT_STRUCT(AddCommentResponse) {
  CURRENT_FIELD(us, std::chrono::microseconds, 0);
  CURRENT_FIELD(oid, std::string);
};

// Schema for the response of the DELETE request for a comment.
CURRENT_STRUCT(DeleteCommentResponse) { CURRENT_FIELD(us, std::chrono::microseconds, 0); };

// Comments response schema.
CURRENT_STRUCT(ResponseComment) {
  CURRENT_FIELD(oid, std::string, "oINVALID");  // Comment id, format 'o05XXX...'.
  CURRENT_FIELD(
      parent_oid, std::string, "");  // Empty string or parent comment id. NOTE: Two levels of comments only.
  CURRENT_FIELD(author_uid, std::string, "uINVALID");  // User id, format 'u01XXX...'.
  CURRENT_FIELD(author_level, uint8_t, 0u);            // Author user level, [0, 9].
  CURRENT_FIELD(text, std::string);                    // Comment text.
  CURRENT_FIELD(number_of_likes, uint32_t, 0u);        // Number of likes in this comment.
  CURRENT_FIELD(liked, bool, false);                   // Whether the current user has liked this comment.
  CURRENT_FIELD(flagged_inappropriate,
                bool,
                false);  // Whether the current user has flagged this comment as inappropriate.
  CURRENT_FIELD(us, std::chrono::microseconds, 0);  // Comment timestamp, microseconds from epoch.
  // TODO(dkorolev): User name? Tier status?
  // TODO(dkorolev): Color?
};

CURRENT_STRUCT(ResponseComments) {
  CURRENT_FIELD(us, std::chrono::microseconds, 0);        // Server timestamp, microseconds from epoch.
  CURRENT_FIELD(comments, std::vector<ResponseComment>);  // Comments.
};

// TODO(dkorolev): Constraints on comment length when adding them?

CURRENT_STRUCT(ResponseNotification) {
  CURRENT_FIELD(nid, std::string, "");
  CURRENT_FIELD(type, std::string, "");
  CURRENT_FIELD(us, std::chrono::microseconds, 0);
  CURRENT_FIELD(uid, std::string, "");
  CURRENT_FIELD(cid, std::string, "");
  CURRENT_FIELD(oid, std::string, "");
  CURRENT_FIELD(text, std::string, "");
  CURRENT_FIELD(card, ResponseCardEntry);
  CURRENT_FIELD(n, uint32_t, 1u);
};

// Universal response structure, combining user info & cards payload.
CURRENT_STRUCT(ResponseFeed) {
  CURRENT_FIELD(us, std::chrono::microseconds, 0);             // Server timestamp, microseconds from epoch.
  CURRENT_FIELD(user, ResponseUserEntry);                      // User information.
  CURRENT_FIELD(feed_hot, std::vector<ResponseCardEntry>);     // "Hot" cards feeds.
  CURRENT_FIELD(feed_recent, std::vector<ResponseCardEntry>);  // "Recent" cards feeds.
  CURRENT_FIELD(notifications, std::vector<ResponseNotification>);  // Notifications.
};

#endif  // NEW_SCHEMA_PUBLIC_H
