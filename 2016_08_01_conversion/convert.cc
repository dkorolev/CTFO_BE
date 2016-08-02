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

#include "schema_2016_07_17.h"

// Note: The following lines have been added into `schema_2016_08_01.h` manually:
/*
  // <MANUAL_LINE>
  CURRENT_NAMESPACE_TYPE(CTFOEvent, current_userspace::t9204521049515947345::Transaction_T9225800578910870409);
  // </MANUAL_LINE>
*/

#include "schema_2016_08_01.h"

#include "../../Current/Bricks/dflags/dflags.h"

#include "../../Current/Midichlorians/Server/schema.h"

// Note: `NewCTFOStorage` is the old one, which we're evolving from.
// From this commit on, I'm naming them `CTFO_YYYY_MM_DD`. -- D.K.

DEFINE_string(old_db, "db.json", "");
DEFINE_string(old_ctfo_events, "ctfo_events.log", "");
DEFINE_string(new_db, "new_db.json", "");

CURRENT_TYPE_EVOLVER(Evolver_2016_08_01,
                     NewCTFOStorage,
                     TopLevelTransaction,
                     {
                       CURRENT_NATURAL_EVOLVE(NewCTFOStorage, CTFO_2016_08_01, from.meta, into.meta);
                       CURRENT_NATURAL_EVOLVE(NewCTFOStorage, CTFO_2016_08_01, from.mutations, into.mutations);
                     });

struct TypesCounter {
  std::map<std::string, size_t> counters;
  template <typename T>
  void operator()(const T&) {
    ++counters[current::reflection::CurrentTypeName<T>()];
  }
};

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  if (false) {
    TypesCounter counter;
    for (const std::string& line : current::strings::Split<current::strings::ByLines>(
             current::FileSystem::ReadFileAsString(FLAGS_old_ctfo_events))) {
      const auto e = ParseJSON<current::midichlorians::server::EventLogEntry, JSONFormat::Minimalistic>(line);
      e.event.Call(counter);
    }
    std::cout << JSON(counter.counters);
    return 0;
    /*
      $ cat ctfo_events.log | sed 's/""/"NULL"/g' | jq -r .event.NULL | sort | uniq -c | sort -gr
        68165 T9201174724793298462
         8495 T9207776099720188653
         1171 T9201594862172611068
         1165 T9205653292032376105
          266 T9204032455253812491
            3 T9202274666560225185

      T9201174724793298462: iOSGenericEvent
      T9207776099720188653: iOSFocusEvent
      T9201594862172611068: iOSDeviceInfo
      T9205653292032376105: iOSAppLaunchEvent
      T9204032455253812491: iOSFirstLaunchEvent
      T9202274666560225185: (a dummy event with fake_did, ignore those three occurrences)

      $ ./.current/convert | jq .
      {
        "iOSGenericEvent": 68168,
        "iOSFocusEvent": 8495,
        "iOSFirstLaunchEvent": 266,
        "iOSDeviceInfo": 1171,
        "iOSAppLaunchEvent": 1165
      }

      => OK to leave this way, with `JSONFormat::Minimalistic`. -- D.K.
    */
  }

  std::vector<std::string> output;

  for (const std::string& line : current::strings::Split<current::strings::ByLines>(
           current::FileSystem::ReadFileAsString(FLAGS_old_db))) {
    std::vector<std::string> parts = current::strings::Split(line, '\t');
    assert(parts.size() == 2u);

    typename NewCTFOStorage::TopLevelTransaction from;
    typename CTFO_2016_08_01::CTFOEvent into;

    ParseJSON(parts[1], from);
    current::type_evolution::Evolve<
        NewCTFOStorage,
        typename NewCTFOStorage::TopLevelTransaction,
        current::type_evolution::Evolver_2016_08_01>::template Go<CTFO_2016_08_01>(from, into);

    output.push_back(parts[0] + '\t' + JSON(into));
  }

  current::FileSystem::WriteStringToFile(current::strings::Join(output, '\n'), FLAGS_new_db.c_str());
}
