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

#include "new_storage.h"

#include "../../Current/Bricks/dflags/dflags.h"

DEFINE_string(db, "new_db.json", "The name of the input DB to print the stats for.");

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  std::cerr << "Replaying " << FLAGS_db << std::endl;
  using DB = NewCTFO<SherlockStreamPersister>;
  DB db(FLAGS_db);
  std::cerr << "Replay done." << std::endl;

  db.ReadOnlyTransaction([](ImmutableFields<DB> fields) {
#define REPORT_SIZE(x) std::cerr << #x ": " << fields.x.Size() << " records.\n";
    REPORT_SIZE(user);
    REPORT_SIZE(auth_token);
    REPORT_SIZE(auth_uid);
    REPORT_SIZE(card);
    REPORT_SIZE(card_author);
    REPORT_SIZE(answer);
    REPORT_SIZE(favorite);
    REPORT_SIZE(comment);
    REPORT_SIZE(comment_like);
    REPORT_SIZE(flagged_card);
    REPORT_SIZE(flagged_comment);
    REPORT_SIZE(notification);
    REPORT_SIZE(starred_notification_already_sent);
    REPORT_SIZE(user_reported_user);
    REPORT_SIZE(user_blocked_user);
    REPORT_SIZE(banned_user);
#undef REPORT_SIZE
  }).Go();
}
