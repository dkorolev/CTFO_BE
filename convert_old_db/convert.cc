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

#include <cassert>
#include <fstream>
#include <regex>

#include "intermediate_types.h"
#include "new_schema.h"
#include "new_storage.h"

#include "../../Current/Bricks/dflags/dflags.h"
#include "../../Current/Bricks/time/chrono.h"

#include "../../Current/TypeSystem/struct.h"
#include "../../Current/TypeSystem/Serialization/json.h"

DEFINE_string(input, "db.json", "The name of the input data to convert.");
DEFINE_string(output, "new_db.json", "The name of the input data to convert.");

using T_PERSISTED_VARIANT = typename NewCTFO<SherlockStreamPersister>::transaction_t::variant_t;

template <typename T_RECORD, typename T_PERSISTED_RECORD>
std::string GenericUpdate(const std::chrono::microseconds timestamp, const std::vector<std::string>& tsv) {
  assert(tsv.size() == 3u);

  current::storage::Transaction<T_PERSISTED_VARIANT> transaction;

  transaction.meta.timestamp = timestamp;
  transaction.mutations.emplace_back(T_PERSISTED_RECORD());

  std::string json = JSON(transaction);
  std::string subjson = "{\"data\":" + JSON(T_RECORD()) + "}";
  const size_t offset = json.find(subjson);
  assert(offset != std::string::npos);

  std::string converted_json = json.substr(0u, offset) + tsv[2] + json.substr(offset + subjson.length());
  const std::string sub_ms = "\"ms\":";
  const size_t offset_ms = converted_json.find(sub_ms, offset);
  if (offset != std::string::npos) {
    const size_t offset_ms_start = offset_ms + sub_ms.length();
    const size_t offset_ms_end = converted_json.find(',', offset_ms);
    assert(offset_ms_end != std::string::npos);
    std::string ms_value = converted_json.substr(offset_ms_start, offset_ms_end - offset_ms_start);
    std::string sub_us = "\"us\":" + ms_value + "000";  // convert `ms` to `us` by adding 000 to the end
    converted_json = converted_json.replace(offset_ms, offset_ms_end - offset_ms, sub_us);
  }
  return converted_json;
}

template <>
std::string GenericUpdate<new_ctfo::Card, Persisted_CardUpdated>(const std::chrono::microseconds timestamp,
                                                                 const std::vector<std::string>& tsv) {
  assert(tsv.size() == 3u);

  current::storage::Transaction<T_PERSISTED_VARIANT> transaction;

  transaction.meta.timestamp = timestamp;
  transaction.mutations.emplace_back(Persisted_CardUpdated());

  std::string json = JSON(transaction);
  std::string subjson = "{\"data\":" + JSON(new_ctfo::Card()) + "}";
  const size_t offset = json.find(subjson);
  assert(offset != std::string::npos);

  std::string converted_json = json.substr(0u, offset) + tsv[2] + json.substr(offset + subjson.length());
  const std::string sub_ms = "\"ms\":";
  const size_t offset_ms = converted_json.find(sub_ms, offset);
  if (offset != std::string::npos) {
    const size_t offset_ms_start = offset_ms + sub_ms.length();
    const size_t offset_ms_end = converted_json.find(',', offset_ms);
    assert(offset_ms_end != std::string::npos);
    std::string ms_value = converted_json.substr(offset_ms_start, offset_ms_end - offset_ms_start);
    std::string sub_us = "\"us\":" + ms_value + "000";  // convert `ms` to `us` by adding 000 to the end
    converted_json = converted_json.replace(offset_ms, offset_ms_end - offset_ms, sub_us);
  }

  // Now, the real thing: Detect those moments the counters go down.
  using T_TRANSACTION = current::storage::Transaction<T_PERSISTED_VARIANT>;
  auto result = ParseJSON<T_TRANSACTION>(converted_json);
  auto& data = Value<Persisted_CardUpdated>(result.mutations[0]).data;
  auto cid = data.cid;

  static std::map<CID, size_t> ctfo_count_map;
  static std::map<CID, size_t> tfu_count_map;
  static std::map<CID, size_t> skip_count_map;

  size_t& ctfo_count = ctfo_count_map[cid];
  size_t& tfu_count = tfu_count_map[cid];
  size_t& skip_count = skip_count_map[cid];

  if (ctfo_count && data.ctfo_count <= ctfo_count) {
    std::cerr << "ctfo_count: " << ctfo_count << " => " << data.ctfo_count << std::endl;
    data.ctfo_count = ++ctfo_count;
  } else {
    ctfo_count = data.ctfo_count;
  }

  if (tfu_count && data.tfu_count <= tfu_count) {
    std::cerr << "tfu_count: " << tfu_count << " => " << data.tfu_count << std::endl;
    data.tfu_count = ++tfu_count;
  } else {
    tfu_count = data.tfu_count;
  }

  if (skip_count && data.skip_count <= skip_count) {
    std::cerr << "skip_count: " << skip_count << " => " << data.skip_count << std::endl;
    data.skip_count = ++skip_count;
  } else {
    skip_count = data.skip_count;
  }

  return JSON(result);
}

// Special case to convert `UIDAndCID` into `std::pair<UID, CID>`.
template <typename T_RECORD, typename T_PERSISTED_RECORD>
std::string StarredNotificationsSentDictionaryUpdate(const std::chrono::microseconds timestamp,
                                                     const std::vector<std::string>& tsv) {
  assert(tsv.size() == 3u);

  current::storage::Transaction<T_PERSISTED_VARIANT> transaction;

  transaction.meta.timestamp = timestamp;
  std::regex key_regex(".*\"key\":(\\{[^\\}]+\\})[\\}]+");
  std::smatch key_match;
  OldDictionaryUIDAndCIDEntry old_key;
  if (std::regex_match(tsv[2], key_match, key_regex) && key_match.size() == 2) {
    old_key = ParseJSON<OldDictionaryUIDAndCIDEntry>(key_match[1].str());
  } else {
    std::cerr << "Key field search failed in `StarredNotificationsSentDictionaryUpdate()`." << std::endl;
    std::cerr << tsv[2] << std::endl;
    std::exit(-1);
  }
  T_RECORD record;
  record.key = std::make_pair(old_key.uid, old_key.cid);
  transaction.mutations.emplace_back(T_PERSISTED_RECORD(record));

  return JSON(transaction);
}

template <typename T_RECORD, typename T_PERSISTED_RECORD>
std::string DictionaryErase(const std::chrono::microseconds timestamp, const std::vector<std::string>& tsv) {
  assert(tsv.size() == 3u);

  current::storage::Transaction<T_PERSISTED_VARIANT> transaction;

  transaction.meta.timestamp = timestamp;
  const auto key = ParseJSON<OldDictionarySimpleKeyEntry>(tsv[2]);
  auto delete_event = T_PERSISTED_RECORD();
  delete_event.key = static_cast<decltype(T_PERSISTED_RECORD::key)>(key.data);
  transaction.mutations.emplace_back(delete_event);

  return JSON(transaction);
}

template <typename T_RECORD, typename T_PERSISTED_RECORD>
std::string GenericMatrixErase(const std::chrono::microseconds timestamp,
                               const std::vector<std::string>& tsv,
                               bool swap) {
  assert(tsv.size() == 4u);

  current::storage::Transaction<T_PERSISTED_VARIANT> transaction;

  transaction.meta.timestamp = timestamp;
  using T_ROW = current::storage::sfinae::entry_row_t<T_RECORD>;
  using T_COL = current::storage::sfinae::entry_col_t<T_RECORD>;
  const T_ROW row = static_cast<T_ROW>(ParseJSON<OldDictionarySimpleKeyEntry>(tsv[swap ? 3 : 2]).data);
  const T_COL col = static_cast<T_COL>(ParseJSON<OldDictionarySimpleKeyEntry>(tsv[swap ? 2 : 3]).data);
  auto delete_event = T_PERSISTED_RECORD();
  delete_event.key = std::make_pair(row, col);
  transaction.mutations.emplace_back(delete_event);

  return JSON(transaction);
}

template <typename T_RECORD, typename T_PERSISTED_RECORD>
std::string MatrixErase(const std::chrono::microseconds timestamp, const std::vector<std::string>& tsv) {
  return GenericMatrixErase<T_RECORD, T_PERSISTED_RECORD>(timestamp, tsv, false /*swap*/);
}

template <typename T_RECORD, typename T_PERSISTED_RECORD>
std::string MatrixEraseSwapped(const std::chrono::microseconds timestamp, const std::vector<std::string>& tsv) {
  return GenericMatrixErase<T_RECORD, T_PERSISTED_RECORD>(timestamp, tsv, true /*swap*/);
}

// Special case to convert notifications.
std::string NotificationsUpdate(const std::chrono::microseconds timestamp,
                                const std::vector<std::string>& tsv) {
  assert(tsv.size() == 3u);

  using T_TRANSACTION = current::storage::Transaction<T_PERSISTED_VARIANT>;
  T_TRANSACTION transaction;
  transaction.meta.timestamp = timestamp;

  new_ctfo::Notification notification;
  notification.timestamp = timestamp;

  std::regex mega_regex(
      ".*\"uid\":([0-9]+).*\"polymorphic_name\":\"([a-zA-Z]+)\".*\"data\":(\\{[^\\}]+\\})[\\}]+");
  std::smatch mega_match;
  std::string notification_type;
  std::string notification_data;
  if (std::regex_match(tsv[2], mega_match, mega_regex) && mega_match.size() == 4) {
    notification.uid = new_ctfo::StringToUID("u0" + mega_match[1].str());
    assert(notification.uid != UID::INVALID_USER);
    // TODO(dk+mz): ignore notifications for admin user?
    notification_type = mega_match[2].str();
    notification_data = mega_match[3].str();
  } else {
    std::cerr << "Notification mega search failed in `NotificationsUpdate()`." << std::endl;
    std::cerr << tsv[2] << std::endl;
    std::exit(-1);
  }

  if (notification_type == "NotificationMyCardNewComment") {
    notification.notification = new_ctfo::NotificationMyCardNewComment();
  } else if (notification_type == "NotificationNewReplyToMyComment") {
    notification.notification = new_ctfo::NotificationNewReplyToMyComment();
  } else if (notification_type == "NotificationMyCommentLiked") {
    notification.notification = new_ctfo::NotificationMyCommentLiked();
  } else if (notification_type == "NotificationNewCommentOnCardIStarred") {
    notification.notification = new_ctfo::NotificationNewCommentOnCardIStarred();
  } else if (notification_type == "NotificationMyCardStarred") {
    notification.notification = new_ctfo::NotificationMyCardStarred();
  } else if (notification_type == "NotificationNewVotesOnMyCard") {
    notification.notification = new_ctfo::NotificationNewVotesOnMyCard();
  } else {
    std::cerr << "Unknown notification type: " << notification_type << std::endl;
    std::exit(-1);
  }
  transaction.mutations.push_back(Persisted_NotificationUpdated(notification));

  std::string json = JSON(transaction);
  const size_t type_offset = json.find(notification_type);
  assert(type_offset != std::string::npos);
  const size_t value_begin = json.find("{", type_offset);
  assert(value_begin != std::string::npos);
  // Assume there are no complex objects inside.
  const size_t value_end = json.find("}", value_begin);
  assert(value_end != std::string::npos);

  const std::string result = json.substr(0u, value_begin) + notification_data + json.substr(value_end + 1);
  // Check if result is parsable JSON.
  ParseJSON<T_TRANSACTION>(result);
  return result;
}

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  std::ifstream fi(FLAGS_input);
  assert(fi.good());

  std::ofstream fo(FLAGS_output);
  assert(fo.good());

  std::map<std::string, std::function<std::string(std::chrono::microseconds, const std::vector<std::string>&)>>
      handlers;

  // Dictionaries.
  handlers["users.insert"] = GenericUpdate<new_ctfo::User, Persisted_UserUpdated>;
  handlers["users.erase"] = DictionaryErase<new_ctfo::User, Persisted_UserDeleted>;
  handlers["cards.insert"] = GenericUpdate<new_ctfo::Card, Persisted_CardUpdated>;
  handlers["cards.erase"] = DictionaryErase<new_ctfo::User, Persisted_CardDeleted>;
  // We never erase smth in the dictionaries below.
  handlers["starred_notification_already_sent.insert"] =
      StarredNotificationsSentDictionaryUpdate<new_ctfo::StarNotificationAlreadySent,
                                               Persisted_StarNotificationAlreadySentUpdated>;
  handlers["banned_users.insert"] = GenericUpdate<new_ctfo::BannedUser, Persisted_BannedUserUpdated>;

  // Matrix updates.
  handlers["auth_token.add"] = GenericUpdate<new_ctfo::AuthKeyTokenPair, Persisted_AuthKeyTokenPairUpdated>;
  handlers["auth_uid.add"] = GenericUpdate<new_ctfo::UIDAuthKeyPair, Persisted_UIDAuthKeyPairUpdated>;
  handlers["card_authors.add"] = GenericUpdate<new_ctfo::AuthorCard, Persisted_AuthorCardUpdated>;
  handlers["answers.add"] = GenericUpdate<new_ctfo::Answer, Persisted_AnswerUpdated>;
  handlers["favorites.add"] = GenericUpdate<new_ctfo::Favorite, Persisted_FavoriteUpdated>;
  handlers["comments.add"] = GenericUpdate<new_ctfo::Comment, Persisted_CommentUpdated>;
  handlers["comment_likes.add"] = GenericUpdate<new_ctfo::CommentLike, Persisted_CommentLikeUpdated>;
  handlers["flagged_cards.add"] =
      GenericUpdate<new_ctfo::CardFlagAsInappropriate, Persisted_CardFlagAsInappropriateUpdated>;
  handlers["flagged_comments.add"] =
      GenericUpdate<new_ctfo::CommentFlagAsInappropriate, Persisted_CommentFlagAsInappropriateUpdated>;
  handlers["user_reported_user.add"] =
      GenericUpdate<new_ctfo::UserReportedUser, Persisted_UserReportedUserUpdated>;
  handlers["user_blocked_user.add"] =
      GenericUpdate<new_ctfo::UserBlockedUser, Persisted_UserBlockedUserUpdated>;

  // Rare matrix deletes.
  handlers["card_authors.delete"] = MatrixEraseSwapped<new_ctfo::AuthorCard, Persisted_AuthorCardDeleted>;
  handlers["comments.delete"] = MatrixErase<new_ctfo::Comment, Persisted_CommentDeleted>;
  handlers["comment_likes.delete"] = MatrixErase<new_ctfo::CommentLike, Persisted_CommentLikeDeleted>;

  // Notifications.
  handlers["notifications.add"] = NotificationsUpdate;

  // TODO(dkorolev): Matrix REST support.

  size_t total_lines = 0u;
  size_t processed_lines = 0u;
  uint64_t output_mutation_index = 0u;
  std::chrono::microseconds last_timestamp(0);
  std::string line;

  while (std::getline(fi, line)) {
    const auto tsv = current::strings::Split(line, '\t');
    if (tsv.size() < 2u) {
      std::cerr << "ERROR " << total_lines << " : " << line << std::endl;
    }
    // milliseconds -> microseconds.
    auto timestamp = std::chrono::microseconds(current::FromString<int64_t>(tsv[0]) * 1000);
    assert(timestamp.count() > 1448163159000000 && timestamp < current::time::Now());
    if (timestamp <= last_timestamp) {
      timestamp = ++last_timestamp;
    }

    std::string output;

    const auto cit = handlers.find(tsv[1]);
    if (cit != handlers.end()) {
      output = cit->second(timestamp, tsv);
    }

    if (!output.empty()) {
      fo << JSON(idxts_t(output_mutation_index++, timestamp)) << '\t' << output << '\n';
      ++processed_lines;
    }

    ++total_lines;
    last_timestamp = timestamp;
  }

  std::cerr << "Processed " << processed_lines << " of " << total_lines << " lines." << std::endl;
}
