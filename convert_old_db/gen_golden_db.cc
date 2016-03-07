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

// This source file is just to generate golden data to fit the input converted entries to. -- D.K.

#include <cassert>
#include <fstream>

#include "new_schema.h"
#include "new_storage.h"

#include "../../Current/Bricks/dflags/dflags.h"

DEFINE_string(output, "new_db.json", "The name of the output DB file to generate.");

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  using DB = NewCTFO<SherlockStreamPersister>;
  DB db(FLAGS_output);

  current::storage::TransactionResult<bool> result = db.Transaction([](MutableFields<DB> fields) -> bool {
    if (fields.user.Size() || fields.card.Size()) {
      std::cerr << "Storage loaded from `" << FLAGS_output << "` is not empty. Not adding anything.\n";
      return false;
    } else {
      return true;
    }
  }).Go();

  if (Value(result)) {
    db.Transaction([](MutableFields<DB> fields) {
      User user;
      user.uid = static_cast<UID>(42);
      user.level = 1;
      user.score = 9001;
      fields.user.Add(user);
    }).Go();

    db.Transaction([](MutableFields<DB> fields) {
      Card card;
      card.cid = static_cast<CID>(42);
      card.text = "В лесу родилась ёлочка";
      card.color.green = 0xff;
      card.tfu_count = 987654321;
      fields.card.Add(card);
    }).Go();
  }
}
