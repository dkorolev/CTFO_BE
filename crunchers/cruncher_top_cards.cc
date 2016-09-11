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

#include "cruncher_top_cards.h"
#include "schema.h"

#include "../../Current/Bricks/dflags/dflags.h"
#include "../../Current/Sherlock/replicator.h"

namespace CTFO {

CURRENT_STRUCT(TopCardsSubCruncherParams) {
  CURRENT_FIELD(interval, std::chrono::microseconds);
  CURRENT_FIELD_DESCRIPTION(interval, "Size of the cruncher sliding window.");
  CURRENT_FIELD(top_size, uint64_t);
  CURRENT_FIELD_DESCRIPTION(top_size, "Maximum number of items in the top cards list.");
  CURRENT_DEFAULT_CONSTRUCTOR(TopCardsSubCruncherParams) {}
  CURRENT_CONSTRUCTOR(TopCardsSubCruncherParams)(std::chrono::microseconds interval, uint64_t top_size)
      : interval(interval), top_size(top_size) {}
};

CURRENT_STRUCT(TopCardsCruncherParams) {
  CURRENT_FIELD(remote_url, std::string, "http://localhost:8383/raw_log");
  CURRENT_FIELD_DESCRIPTION(remote_url, "Remote CTFOServer URL to subscribe to.");
  CURRENT_FIELD(route, std::string, "/top_cards");
  CURRENT_FIELD_DESCRIPTION(route, "The HTTP url path to expose the cruncher in.");
  CURRENT_FIELD(port, uint16_t, 8374);
  CURRENT_FIELD_DESCRIPTION(port, "Port to spawn the cruncher on.");
  CURRENT_FIELD(args, std::vector<TopCardsSubCruncherParams>);
  CURRENT_FIELD_DESCRIPTION(args, "List of params for several individual cruncher instances.");
  CURRENT_DEFAULT_CONSTRUCTOR(TopCardsCruncherParams)
      : args{TopCardsSubCruncherParams(std::chrono::seconds(10), 3),
             TopCardsSubCruncherParams(std::chrono::hours(1), 5),
             TopCardsSubCruncherParams(std::chrono::hours(24), 10)} {}
};

}  // namespace CTFO

using namespace CTFO;

DEFINE_string(config_file,
              "top_cards_config.json",
              "The file to read TopCardsCruncherParams in JSON format from.");
DEFINE_bool(helpconfig, false, "Display the config format information.");
DEFINE_bool(defaultconfig, false, "Use default configuration and save it to config_file.");

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  if (FLAGS_helpconfig) {
    current::reflection::StructSchema schema;
    schema.AddType<TopCardsCruncherParams>();
    std::cout << schema.GetSchemaInfo().Describe<current::reflection::Language::FSharp>(false);
  } else if (FLAGS_defaultconfig) {
    std::cout << JSON(TopCardsCruncherParams()) << std::endl;
  } else {
    const auto params =
        ParseJSON<TopCardsCruncherParams>(current::FileSystem::ReadFileAsString(FLAGS_config_file));
    using CTFOTopCardsCruncher = TopCardsCruncher<CTFO_2016_08_01>;
    current::sherlock::SubscribableRemoteStream<CTFO_2016_08_01::CTFOLogEntry> remote_stream(
        params.remote_url, "CTFOLogEntry", "CTFO_2016_08_01");
    std::vector<TopCardsCruncherArgs> cruncher_params;
    cruncher_params.reserve(params.args.size());
    const auto calculator = [](const CardCounters& counters) -> int64_t {
      return counters.ctfo + counters.skip + counters.tfu + counters.fav - counters.unfav;
    };
    for (const auto& arg : params.args) {
      cruncher_params.emplace_back(arg.interval, arg.top_size, calculator);
    }
    CTFOTopCardsCruncher cruncher(params.port, params.route, cruncher_params);
    const auto scope = remote_stream.Subscribe(cruncher);
    cruncher.Join();
  }
  return 0;
}
