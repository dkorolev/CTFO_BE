/*******************************************************************************
 The MIT License (MIT)

 Copyright (c) 2016 Grigory Nikolaenko <nikolaenko.grigory@gmail.com>

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

#include "cruncher_active_users.h"
#include "schema.h"

#include "../../Current/Bricks/dflags/dflags.h"
#include "../../Current/Sherlock/replicator.h"

namespace CTFO {

CURRENT_STRUCT(ActiveUsersCruncherParams) {
  CURRENT_FIELD(remote_url, std::string, "http://localhost:8383/raw_log");
  CURRENT_FIELD_DESCRIPTION(remote_url, "Remote CTFOServer URL to subscribe to.");
  CURRENT_FIELD(route, std::string, "/active_users");
  CURRENT_FIELD_DESCRIPTION(route, "The HTTP url path to expose the cruncher in.");
  CURRENT_FIELD(port, uint16_t, 8373);
  CURRENT_FIELD_DESCRIPTION(port, "Port to spawn the cruncher on.");
  CURRENT_FIELD(intervals, std::vector<std::chrono::microseconds>);
  CURRENT_FIELD_DESCRIPTION(intervals, "List of the cruncher sliding window sizes.");
  CURRENT_DEFAULT_CONSTRUCTOR(ActiveUsersCruncherParams) {}
};

}  // namespace CTFO

using namespace CTFO;

DEFINE_string(config_file,
              "active_users_config.json",
              "The file to read ActiveUsersCruncherParams in JSON format from.");
DEFINE_bool(helpconfig, false, "Display the config format information.");
DEFINE_bool(defaultconfig, false, "Use default configuration and save it to config_file.");

int main(int argc, char **argv) {
  ParseDFlags(&argc, &argv);

  if (FLAGS_helpconfig) {
    current::reflection::StructSchema schema;
    schema.AddType<ActiveUsersCruncherParams>();
    std::cout << schema.GetSchemaInfo().Describe<current::reflection::Language::FSharp>(false);
  } else {
    ActiveUsersCruncherParams params;
    if (FLAGS_defaultconfig) {
      params.intervals = {std::chrono::seconds(10), std::chrono::hours(1), std::chrono::hours(24)};
      current::FileSystem::WriteStringToFile(JSON(params), FLAGS_config_file.c_str());
    } else {
      params = ParseJSON<ActiveUsersCruncherParams>(current::FileSystem::ReadFileAsString(FLAGS_config_file));
    }

    using CTFOActiveUsersCruncher = CTFO::ActiveUsersMultiCruncher<CTFO_2016_08_01>;
    current::sherlock::SubscribableRemoteStream<CTFO_2016_08_01::CTFOLogEntry> remote_stream(
        params.remote_url, "CTFOLogEntry", "CTFO_2016_08_01");
    CTFOActiveUsersCruncher cruncher(params.intervals, params.port, params.route);
    const auto scope = remote_stream.Subscribe(cruncher);
    cruncher.Join();
  }
  return 0;
}
