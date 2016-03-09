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

using T_PERSISTED_VARIANT = typename NewCTFO<SherlockStreamPersister>::T_TRANSACTION::T_VARIANT;

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

  return json.substr(0u, offset) + tsv[2] + json.substr(offset + subjson.length());
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
    std::cerr << key_match.size() << std::endl;
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
std::string MatrixErase(const std::chrono::microseconds timestamp, const std::vector<std::string>& tsv) {
  assert(tsv.size() == 4u);

  current::storage::Transaction<T_PERSISTED_VARIANT> transaction;

  transaction.meta.timestamp = timestamp;
  using T_ROW = current::storage::sfinae::ENTRY_ROW_TYPE<T_RECORD>;
  using T_COL = current::storage::sfinae::ENTRY_COL_TYPE<T_RECORD>;
  const T_ROW row = static_cast<T_ROW>(ParseJSON<OldDictionarySimpleKeyEntry>(tsv[2]).data);
  const T_COL col = static_cast<T_COL>(ParseJSON<OldDictionarySimpleKeyEntry>(tsv[3]).data);
  auto delete_event = T_PERSISTED_RECORD();
  delete_event.key = std::make_pair(row, col);
  transaction.mutations.emplace_back(delete_event);

  return JSON(transaction);
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
  handlers["auth_uid.add"] = GenericUpdate<new_ctfo::AuthKeyUIDPair, Persisted_AuthKeyUIDPairUpdated>;
  handlers["card_authors.add"] = GenericUpdate<new_ctfo::CardAuthor, Persisted_CardAuthorUpdated>;
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
  handlers["card_authors.delete"] = MatrixErase<new_ctfo::CardAuthor, Persisted_CardAuthorDeleted>;
  handlers["comments.delete"] = MatrixErase<new_ctfo::Comment, Persisted_CommentDeleted>;
  handlers["comment_likes.delete"] = MatrixErase<new_ctfo::CommentLike, Persisted_CommentLikeDeleted>;

  // TODO(dkorolev): Notifications.
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
