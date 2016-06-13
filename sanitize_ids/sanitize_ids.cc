/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Dmitry "Dima" Korolev <dmitry.korolev@gmail.com>

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

// Run `./generate_autogen_schema.sh` to regenerate this file.

#include "autogen_schema.h"
#include "../util.h"

#include "../../Current/Bricks/dflags/dflags.h"

DEFINE_string(input, "db.json", "The name of the input data file to fix the IDs in.");
DEFINE_string(output, "output_db.json", "The name of the file to write the output to.");

// NOTE: This `current_userspace_*` would have to be updated manually if the `old` schema changes.
// Unlikely though. :-) -- D.K.
using namespace current_userspace_3b3bd18430c5a642;  // For `UID`, `CID`, and `UID` to be the right ones.

static std::unordered_map<uint64_t, uint64_t> uids;
static std::unordered_map<uint64_t, uint64_t> cids;
static std::unordered_map<uint64_t, uint64_t> oids;

template <typename T>
void AssertUnique(std::unordered_map<uint64_t, uint64_t>& map_into_to_from, T& from_id, T& into_id) {
  const auto into = static_cast<uint64_t>(into_id);
  const auto from = static_cast<uint64_t>(from_id);
  if (!map_into_to_from.count(into)) {
    map_into_to_from[into] = from;
  } else {
    assert(map_into_to_from[into] == from);
  }
}

void NormalizeID(UID from, UID& into) {
  assert(static_cast<uint64_t>(from) / CTFO::OLD_ID_RANGE == 1);
  if (from == static_cast<UID>(CTFO::old_admin_uid)) {
    into = static_cast<UID>(CTFO::admin_uid);
  } else {
    into = static_cast<UID>(static_cast<uint64_t>(from) / 1000ull);
    AssertUnique(uids, from, into);
  }
}
void NormalizeID(CID from, CID& into) {
  assert(static_cast<uint64_t>(from) / CTFO::OLD_ID_RANGE == 2);
  into = static_cast<CID>(static_cast<uint64_t>(from) / 1000ull);
  AssertUnique(cids, from, into);
}

void NormalizeID(OID from, OID& into) {
  if (from == static_cast<OID>(CTFO::OID::INVALID_COMMENT)) {
    // Top-level's comment `parent_oid` can and will be `0`, which is `INVALID_COMMENT`.
    into = static_cast<OID>(CTFO::OID::INVALID_COMMENT);
  } else {
    assert(static_cast<uint64_t>(from) / CTFO::OLD_ID_RANGE == 4);
    into = static_cast<OID>(static_cast<uint64_t>(from) / 1000ull);
    AssertUnique(oids, from, into);
  }
}

CURRENT_TYPE_EVOLUTOR(CTFOIdsSanitizer, OldCTFOStorage, UID, NormalizeID(from, into));
CURRENT_TYPE_EVOLUTOR(CTFOIdsSanitizer, OldCTFOStorage, CID, NormalizeID(from, into));
CURRENT_TYPE_EVOLUTOR(CTFOIdsSanitizer, OldCTFOStorage, OID, NormalizeID(from, into));

int main(int argc, char** argv) {
  ParseDFlags(&argc, &argv);

  current::FileSystem::RmFile(FLAGS_output, current::FileSystem::RmFileParameters::Silent);

  current::sherlock::Stream<typename OldCTFOStorage::TopLevelTransaction, current::persistence::File> input_db(
      FLAGS_input);
  current::sherlock::Stream<typename OldCTFOStorage::TopLevelTransaction, current::persistence::File> output_db(
      FLAGS_output);

  for (const auto& e : input_db.InternalExposePersister().Iterate()) {
    const typename OldCTFOStorage::TopLevelTransaction& from = e.entry;
    typename OldCTFOStorage::TopLevelTransaction into;
    current::type_evolution::Evolve<
        OldCTFOStorage,
        typename OldCTFOStorage::TopLevelTransaction,
        current::type_evolution::CTFOIdsSanitizer>::template Go<OldCTFOStorage>(from, into);
    output_db.Publish(into, e.idx_ts.us);
  }

  std::cout << "Done, " << uids.size() << " UIDs, " << cids.size() << " CIDs, " << oids.size() << " OIDs.\n";
}
