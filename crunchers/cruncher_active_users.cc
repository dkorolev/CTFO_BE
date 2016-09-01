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

#include "../../Current/Sherlock/replicator.h"
#include "../../Current/Bricks/dflags/dflags.h"

using namespace CTFO;

DEFINE_string(remote_url, "http://localhost:8383/raw_log", "Remote CTFOServer URL to subscribe to.");
DEFINE_string(cruncher_route, "/active_users", "The HTTP url path to expose the cruncher in.");
DEFINE_int32(cruncher_port, 8373, "Port to spawn the cruncher on.");

int main(int argc, char **argv) {
  ParseDFlags(&argc, &argv);

  current::sherlock::SubscribableRemoteStream<CTFO_2016_08_01::CTFOLogEntry> remote_stream(
      FLAGS_remote_url, "CTFOLogEntry", "CTFO_2016_08_01");
  using CTFOActiveUsersCruncher = CTFO::ActiveUsersMultiCruncher<CTFO_2016_08_01>;

  CTFOActiveUsersCruncher::duration_list_t intervals = {
      std::chrono::seconds(10), std::chrono::hours(1), std::chrono::hours(24)};
  CTFOActiveUsersCruncher cruncher(intervals, FLAGS_cruncher_port, FLAGS_cruncher_route);

  const auto scope = remote_stream.Subscribe(cruncher);
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}
