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

#include "new_schema.h"
#include "new_storage.h"

#include "../../Current/Bricks/dflags/dflags.h"
#include "../../Current/Bricks/time/chrono.h"

#include "../../Current/TypeSystem/struct.h"
#include "../../Current/TypeSystem/Serialization/json.h"

DEFINE_string(input, "db.json", "The name of the input data to convert.");
DEFINE_string(output, "new_db.json", "The name of the input data to convert.");

using T_PERSISTED_VARIANT = typename NewCTFO<SherlockStreamPersister>::T_TRANSACTION::T_VARIANT;

template<typename T_RECORD, typename T_PERSISTED_RECORD>
std::string DictionaryUpdate(const std::chrono::microseconds timestamp, const std::vector<std::string>& tsv) {
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

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  std::ifstream fi(FLAGS_input);
  assert(fi.good());

  std::ofstream fo(FLAGS_output);
  assert(fo.good());

  std::map<std::string, std::function<std::string(std::chrono::microseconds, const std::vector<std::string>&)>>
      handlers;

  handlers["users.insert"] = DictionaryUpdate<User, Persisted_UserUpdated>;
  handlers["cards.insert"] = DictionaryUpdate<Card, Persisted_CardUpdated>;
  // TODO(dkorolev): Dictionary deletions.
  // TODO(dkorolev): Matrix mutations.
  // TODO(dkorolev): Matrix deletions.
  // TODO(dkorolev): Matrix REST support.

  int lines = 0;
  int output_mutation_index = 0;
  std::string line;

  while (std::getline(fi, line)) {
    const auto timestamp = current::time::Now();
    const auto tsv = current::strings::Split(line, '\t');
    if (tsv.size() < 2u) {
      std::cerr << "ERROR " << lines << " : " << line << std::endl;
    }

    std::string output;

    const auto cit = handlers.find(tsv[1]);
    if (cit != handlers.end()) {
      output = cit->second(timestamp, tsv);
    }

    if (!output.empty()) {
      fo << JSON(idxts_t(output_mutation_index++, timestamp)) << '\t' << output << '\n';
    }

    ++lines;
  }

  std::cerr << "Processed " << lines << " lines." << std::endl;
}
