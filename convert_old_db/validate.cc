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

  db.Transaction([](MutableFields<DB> fields) {
    std::cerr << fields.user.Size() << " users.\n";
    std::cerr << fields.card.Size() << " cards.\n";
    std::cerr << fields.auth_token.Size() << " `auth_token` entries.\n";
    std::cerr << fields.auth_uid.Size() << " `auth_uid` entries.\n";
    std::cerr << fields.card_author.Size() << " card authors.\n";
    std::cerr << fields.answer.Size() << " answers.\n";
    std::cerr << fields.comment.Size() << " comments.\n";
    std::cerr << fields.comment_like.Size() << " comment likes.\n";
    std::cerr << fields.flagged_card.Size() << " flagged cards.\n";
    std::cerr << fields.flagged_comment.Size() << " flagged comments.\n";
    std::cerr << fields.starred_notification_already_sent.Size()
              << " `starred_notification_already_sent` entries.\n";
    std::cerr << fields.user_reported_user.Size() << " `user_reported_user` entries.\n";
    std::cerr << fields.user_blocked_user.Size() << " `user_blocked_user` entries.\n";
    std::cerr << fields.banned_user.Size() << " banned users.\n";
  }).Go();
}
