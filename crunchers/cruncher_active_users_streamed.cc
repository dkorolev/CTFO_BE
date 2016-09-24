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

CURRENT_STRUCT(ActiveUsersStreamedSubCruncherParams) {
  CURRENT_FIELD(interval, std::chrono::microseconds);
  CURRENT_FIELD_DESCRIPTION(interval, "Size of the cruncher sliding window.");
  CURRENT_FIELD(output_interval, std::chrono::microseconds);
  CURRENT_FIELD_DESCRIPTION(output_interval, "Time periods to write the cruncher data between.");
  CURRENT_FIELD(output_path, std::string);
  CURRENT_FIELD_DESCRIPTION(output_path, "A path to file to write the cruncher data to.");
  CURRENT_DEFAULT_CONSTRUCTOR(ActiveUsersStreamedSubCruncherParams) {}
  CURRENT_CONSTRUCTOR(ActiveUsersStreamedSubCruncherParams)(std::chrono::microseconds interval,
                                                            std::chrono::microseconds output_interval,
                                                            const std::string& log_path)
      : interval(interval), output_interval(output_interval), output_path(log_path) {}
};

CURRENT_STRUCT(ActiveUsersStreamedCruncherParams) {
  CURRENT_FIELD(remote_url, std::string, "http://localhost:8383/raw_log");
  CURRENT_FIELD_DESCRIPTION(remote_url, "Remote CTFOServer URL to subscribe to.");
  CURRENT_FIELD(route, std::string, "/active_users");
  CURRENT_FIELD_DESCRIPTION(route, "The HTTP url path to expose the cruncher in.");
  CURRENT_FIELD(port, uint16_t, 8373);
  CURRENT_FIELD_DESCRIPTION(port, "Port to spawn the cruncher on.");
  CURRENT_FIELD(sub_args, std::vector<ActiveUsersStreamedSubCruncherParams>);
  CURRENT_FIELD_DESCRIPTION(sub_args, "List of params for several individual cruncher instances.");
  CURRENT_DEFAULT_CONSTRUCTOR(ActiveUsersStreamedCruncherParams)
      : sub_args{ActiveUsersStreamedSubCruncherParams(
                     std::chrono::hours(24), std::chrono::hours(24), "active_users_1d.log"),
                 ActiveUsersStreamedSubCruncherParams(
                     std::chrono::hours(2 * 24), std::chrono::hours(24), "active_users_2d.log"),
                 ActiveUsersStreamedSubCruncherParams(
                     std::chrono::hours(3 * 24), std::chrono::hours(24), "active_users_3d.log")} {}
};

}  // namespace CTFO

using namespace CTFO;

DEFINE_string(config_file,
              "active_users_config.json",
              "The file to read ActiveUsersCruncherParams in JSON format from.");
DEFINE_bool(helpconfig, false, "Display the config format information.");
DEFINE_bool(defaultconfig, false, "Use default configuration and save it to config_file.");

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  if (FLAGS_helpconfig) {
    current::reflection::StructSchema schema;
    schema.AddType<ActiveUsersStreamedCruncherParams>();
    std::cout << schema.GetSchemaInfo().Describe<current::reflection::Language::FSharp>(false);
  } else if (FLAGS_defaultconfig) {
    std::cout << JSON(ActiveUsersStreamedCruncherParams()) << std::endl;
  } else {
    const auto params =
        ParseJSON<ActiveUsersStreamedCruncherParams>(current::FileSystem::ReadFileAsString(FLAGS_config_file));

    current::sherlock::SubscribableRemoteStream<CTFO_2016_08_01::CTFOLogEntry> remote_stream(
        params.remote_url, "CTFOLogEntry", "CTFO_2016_08_01");

    using response_t = CruncherResponse<std::vector<std::string>>;
    using stream_t = current::sherlock::Stream<response_t, current::persistence::File>;
    using cruncher_t = ActiveUsersStreamedCruncher<CTFO_2016_08_01, stream_t>;

    std::chrono::microseconds tick_interval = std::chrono::hours(24 * 365);
    std::vector<std::chrono::microseconds> intervals;
    std::vector<CruncherOutputStreamParams> output_streams;
    intervals.reserve(params.sub_args.size());
    output_streams.reserve(params.sub_args.size());
    for (const auto& args : params.sub_args) {
      intervals.push_back(args.interval);
      output_streams.emplace_back(args.output_interval, args.output_path);
      if (args.interval < tick_interval) tick_interval = args.interval;
    }

    cruncher_t cruncher(params.port, params.route, tick_interval, intervals, output_streams);
    const auto scope = remote_stream.Subscribe(cruncher);
    cruncher.Join();
  }
  return 0;
}
