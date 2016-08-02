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

struct CTFOEventsParser {
  std::chrono::microseconds us;
  std::vector<std::pair<std::chrono::microseconds, std::string>>& output;

  CTFOEventsParser(std::vector<std::pair<std::chrono::microseconds, std::string>>& output) : output(output) {}

  void operator()(const current::midichlorians::ios::iOSDeviceInfo& input_device_info) {
    typename CTFO_2016_08_01::CTFOEvent output_event;
    output_event.meta.begin_us = us + std::chrono::microseconds(1);
    output_event.meta.end_us = us + std::chrono::microseconds(3);

    typename CTFO_2016_08_01::iOSDeviceInfo output_device_info;

    typename CTFO_2016_08_01::Device output_device;
    output_device.did = input_device_info.device_id;
    static const std::string field_name = "advertisingIdentifier";
    output_device.ad_id = input_device_info.info.count(field_name) ? input_device_info.info.at(field_name) : "";

    current::type_evolution::Evolve<CTFO_2016_08_01,
                                    typename CTFO_2016_08_01::iOSDeviceInfo,
                                    current::type_evolution::Evolver_2016_08_01>::
        template Go<CTFO_2016_08_01>(
            reinterpret_cast<const typename CTFO_2016_08_01::iOSDeviceInfo&>(input_device_info),
            output_device.info);

    typename CTFO_2016_08_01::Persisted_DeviceUpdated output_device_info_mutation_event;
    output_device_info_mutation_event.us = us + std::chrono::microseconds(1);
    output_device_info_mutation_event.data = output_device;
    output_event.mutations.push_back(output_device_info_mutation_event);

    output.emplace_back(us, JSON(output_event));
  }

  // Ignore all events except `current::midichlorians::ios::iOSDeviceInfo` for now.
  template <typename T>
  void operator()(const T&) {}

};

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  std::vector<std::pair<std::chrono::microseconds, std::string>> output;

  CTFOEventsParser parser(output);
  for (const std::string& line : current::strings::Split<current::strings::ByLines>(
           current::FileSystem::ReadFileAsString(FLAGS_old_ctfo_events))) {
    const auto e = ParseJSON<current::midichlorians::server::EventLogEntry, JSONFormat::Minimalistic>(line);
    parser.us = e.server_us;
    e.event.Call(parser);
  }

  for (const std::string& line : current::strings::Split<current::strings::ByLines>(
           current::FileSystem::ReadFileAsString(FLAGS_old_db))) {
    std::vector<std::string> parts = current::strings::Split(line, '\t');
    assert(parts.size() == 2u);

    typename NewCTFOStorage::TopLevelTransaction from;
    typename CTFO_2016_08_01::CTFOEvent into;

    const auto idx_ts = ParseJSON<idxts_t>(parts[0]);
    ParseJSON(parts[1], from);

    current::type_evolution::Evolve<
        NewCTFOStorage,
        typename NewCTFOStorage::TopLevelTransaction,
        current::type_evolution::Evolver_2016_08_01>::template Go<CTFO_2016_08_01>(from, into);

    output.emplace_back(idx_ts.us, JSON(into));
  }

  std::sort(output.begin(), output.end());

  {
    std::ofstream fo(FLAGS_new_db);
    idxts_t key;
    key.index = 0;
    std::chrono::microseconds previous_timestamp;
    for (const auto& e : output) {
      assert(e.first > previous_timestamp);
      previous_timestamp = e.first;

      key.us = e.first;
      fo << JSON(key) << '\t' << e.second << '\n';

      ++key.index;
    }
  }
}
