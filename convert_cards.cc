/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Maxim Zhurovich <zhurovich@gmail.com>

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

#include <set>

#include "../Current/Bricks/dflags/dflags.h"
#include "../Current/Bricks/file/file.h"
#include "../Current/Bricks/strings/strings.h"
#include "../Current/TypeSystem/Serialization/json.h"

#include "util.h"
#include "schema.h"

using namespace CTFO;

DEFINE_string(in, "cards.txt", "Default input file in raw text format.");
DEFINE_string(out, "cards.json", "Default output file in JSON format.");
DEFINE_bool(append, false, "Append to existing output file if `true`, delete file otherwise.");

CID CIDByHash(const std::string& text) {
  CID cid = static_cast<CID>(std::hash<std::string>()(text) % ID_RANGE + 2 * ID_RANGE);
  return cid;
}

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);
  std::vector<std::string> raw_cards;

  if (!FLAGS_append) {
    current::FileSystem::RmFile(FLAGS_out, current::FileSystem::RmFileParameters::Silent);
  }

  try {
    raw_cards =
        current::strings::Split<current::strings::ByLines>(current::FileSystem::ReadFileAsString(FLAGS_in));
  } catch (const current::CannotReadFileException& e) {
    std::cerr << "Unable to read file '" << FLAGS_in << "': " << e.what() << std::endl;
    return -1;
  }

  std::ofstream out_json(FLAGS_out);
  assert(out_json.good());

  std::set<CID> cids;
  for (const auto& text : raw_cards) {
    if (text.length() >= 3u) {
      // Generate a unique CID.
      CID cid;
      std::string addition = "";
      do {
        cid = CIDByHash(text + addition);
        addition += " ";
      } while (cids.find(cid) != cids.end());
      cids.insert(cid);

      Card c(cid, text, CARD_COLORS[(static_cast<uint64_t>(cid) * 17) % CARD_COLORS.size()]);
      if (std::isdigit(text[0]) && text[1] == '\t') {
        const int index = static_cast<double>(text[0] - '0');
        c.text = text.substr(2);
        c.startup_index = index;
        c.us = std::chrono::microseconds((2000000000000 - index) * 1000);
      }
      out_json << JSON(c) << '\n';
    }
  }
}
