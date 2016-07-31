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

#include "schema_old.h"
#include "schema_new.h"

#include "../../Current/Bricks/dflags/dflags.h"

DEFINE_string(old_db, "db.json", "");
DEFINE_string(new_db, "new_db.json", "");

static int64_t pseudo_now = 0ull;

CURRENT_TYPE_EVOLVER(CustomEvolver,
                     OldCTFOStorage,
                     TopLevelTransaction,
                     {
                       pseudo_now = std::max(pseudo_now, from.meta.timestamp.count());
                       into.meta.begin_us = std::chrono::microseconds(pseudo_now);
                       CURRENT_NATURAL_EVOLVE(
                           OldCTFOStorage, NewCTFOStorage, from.meta.fields, into.meta.fields);
                       CURRENT_NATURAL_EVOLVE(OldCTFOStorage, NewCTFOStorage, from.mutations, into.mutations);
                       into.meta.end_us = std::chrono::microseconds(++pseudo_now);
                     });

#define EXISTING_TABLE(X)                                                         \
  CURRENT_TYPE_EVOLVER(CustomEvolver,                                             \
                       OldCTFOStorage,                                            \
                       Persisted_##X##Updated,                                    \
                       {                                                          \
    into.us = std::chrono::microseconds(++pseudo_now);                            \
    CURRENT_NATURAL_EVOLVE(OldCTFOStorage, NewCTFOStorage, from.data, into.data); \
                       });                                                        \
  CURRENT_TYPE_EVOLVER(CustomEvolver,                                             \
                       OldCTFOStorage,                                            \
                       Persisted_##X##Deleted,                                    \
                       {                                                          \
    into.us = std::chrono::microseconds(++pseudo_now);                            \
    CURRENT_NATURAL_EVOLVE(OldCTFOStorage, NewCTFOStorage, from.key, into.key);   \
                       })

EXISTING_TABLE(User);
EXISTING_TABLE(AuthKeyTokenPair);
EXISTING_TABLE(UIDAuthKeyPair);
EXISTING_TABLE(UserNotificationPlayerID);
EXISTING_TABLE(PushNotificationsMarker);
EXISTING_TABLE(Card);
EXISTING_TABLE(AuthorCard);
EXISTING_TABLE(Answer);
EXISTING_TABLE(Favorite);
EXISTING_TABLE(Comment);
EXISTING_TABLE(CommentLike);
EXISTING_TABLE(CardFlagAsInappropriate);
EXISTING_TABLE(CommentFlagAsInappropriate);
EXISTING_TABLE(Notification);
EXISTING_TABLE(StarNotificationAlreadySent);
EXISTING_TABLE(UserReportedUser);
EXISTING_TABLE(UserBlockedUser);
EXISTING_TABLE(BannedUser);
// EXISTING_TABLE(IOSAdWordsInstallTracked); <-- Doesn't exist in previous data. -- D.K.

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  std::vector<std::string> output;

  for (const std::string& line : current::strings::Split<current::strings::ByLines>(
           current::FileSystem::ReadFileAsString(FLAGS_old_db))) {
    std::vector<std::string> parts = current::strings::Split(line, '\t');
    assert(parts.size() == 2u);

    typename OldCTFOStorage::TopLevelTransaction from;
    typename NewCTFOStorage::TopLevelTransaction into;

    ParseJSON(parts[1], from);
    current::type_evolution::Evolve<OldCTFOStorage,
                                    typename OldCTFOStorage::TopLevelTransaction,
                                    current::type_evolution::CustomEvolver>::template Go<NewCTFOStorage>(from,
                                                                                                         into);

    output.push_back(parts[0] + '\t' + JSON(into));
  }

  current::FileSystem::WriteStringToFile(current::strings::Join(output, '\n'), FLAGS_new_db.c_str());
}
