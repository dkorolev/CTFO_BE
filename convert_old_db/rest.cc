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

#include "new_storage.h"

#include "../../Current/Bricks/dflags/dflags.h"

#include "../../Current/Storage/api.h"

#ifdef USE_BASIC_HYPERMEDIA
#include "../../Current/Storage/rest/hypermedia.h"
#else
#include "../../Current/Storage/rest/advanced_hypermedia.h"
#endif

DEFINE_string(db, "new_db.json", "The name of the input DB to print the stats for.");

DEFINE_int32(port, 3000, "The port to spawn CTFO REST API on.");
DEFINE_string(route, "", "The route to spawn CTFO REST API on.");
DEFINE_string(restful_route, "http://localhost:3000", "The hypermedia route to spawn CTFO REST API on.");

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  using DB = NewCTFO<SherlockStreamPersister>;
  DB db(FLAGS_db);

#ifdef USE_BASIC_HYPERMEDIA
  RESTfulStorage<DB, current::storage::rest::Hypermedia> rest(db, FLAGS_port, FLAGS_route, FLAGS_restful_route);
#else
  RESTfulStorage<DB, current::storage::rest::AdvancedHypermedia> rest(
      db, FLAGS_port, FLAGS_route, FLAGS_restful_route);
#endif

  HTTP(FLAGS_port).Join();
}
