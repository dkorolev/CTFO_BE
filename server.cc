/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Maxim Zhurovich <zhurovich@gmail.com>
          (c) 2015 Dmitry "Dima" Korolev <dmitry.korolev@gmail.com>

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

#include "server.h"

#include "../Current/Bricks/dflags/dflags.h"

using namespace CTFO;

DEFINE_string(cards_file, "cards.json", "Cards data file in JSON format.");
DEFINE_int32(port, 8383, "Port to spawn CTFO RESTful server on.");
DEFINE_string(storage_file, "./db.json", "The file to store the snapshot of the database in.");
DEFINE_int32(midichlorians_port, 0, "Port to spawn midichlorians server on.");  // 0 = the same as `port`.
DEFINE_string(midichlorians_file,
              "./ctfo_events.log",
              "Log file to store events received by midichlorians server");
DEFINE_int32(rand_seed, 42, "The answer to the question of life, universe and everything.");
DEFINE_int32(tick_interval_ms, 5 * 60 * 1000, "Maximum interval between event entries.");
DEFINE_bool(debug_print, true, "Print debug info to stderr.");

int main(int argc, char **argv) {
  ParseDFlags(&argc, &argv);
  current::random::SetRandomSeed(FLAGS_rand_seed);
  CTFOServer(FLAGS_cards_file,
             FLAGS_port,
             FLAGS_storage_file,
             FLAGS_midichlorians_port,
             FLAGS_midichlorians_file,
             std::chrono::milliseconds(FLAGS_tick_interval_ms),
             FLAGS_debug_print).Join();
}
