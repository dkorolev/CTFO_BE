/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Maxim Zhurovich <zhurovich@gmail.com>

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

#ifndef CTFO_UTIL_H
#define CTFO_UTIL_H

#include "../Current/Bricks/strings/strings.h"
#include "../Current/Bricks/util/random.h"

#include "schema_base.h"

namespace CTFO {
static constexpr uint64_t OLD_ID_RANGE = static_cast<uint64_t>(1e18);
static constexpr uint64_t ID_RANGE = static_cast<uint64_t>(1e15);
static constexpr UID old_admin_uid = static_cast<UID>(1000000000000000ull);
static constexpr UID admin_uid = static_cast<UID>(1000000000000ull);

inline UID RandomUID() { return static_cast<UID>(RandomUInt64(1 * ID_RANGE + 1, 2 * ID_RANGE - 1)); }

inline CID RandomCID() { return static_cast<CID>(RandomUInt64(2 * ID_RANGE + 1, 3 * ID_RANGE - 1)); }

inline std::string RandomToken() {
  return current::strings::Printf("t%020llu", RandomUInt64(3 * ID_RANGE + 1, 4 * ID_RANGE - 1));
}

inline OID RandomOID() { return static_cast<OID>(RandomUInt64(4 * ID_RANGE + 1, 5 * ID_RANGE - 1)); }

// "05" followed by 18 digits <=> NID, notification ID.

// TODO(dkorolev): Change string representations into the ones three characters shorter. Some day soon. Maybe.

inline std::string UIDToString(const UID uid) {
  return current::strings::Printf("u%020llu", static_cast<uint64_t>(uid));
}

inline UID StringToUID(const std::string& s) {
  if (s.length() == 21 && s[0] == 'u') {  // 'u' + 20 digits of `uint64_t` decimal representation.
    return static_cast<UID>(current::FromString<uint64_t>(s.substr(1)));
  }
  return UID::INVALID_USER;
}

inline std::string CIDToString(const CID cid) {
  return current::strings::Printf("c%020llu", static_cast<uint64_t>(cid));
}

inline CID StringToCID(const std::string& s) {
  if (s.length() == 21 && s[0] == 'c') {  // 'c' + 20 digits of `uint64_t` decimal representation.
    return static_cast<CID>(current::FromString<uint64_t>(s.substr(1)));
  }
  return CID::INVALID_CARD;
}

inline std::string OIDToString(const OID oid) {
  return current::strings::Printf("o%020llu", static_cast<uint64_t>(oid));
}

inline OID StringToOID(const std::string& s) {
  if (s.length() == 21 && s[0] == 'o') {  // 'o' + 20 digits of `uint64_t` decimal representation.
    return static_cast<OID>(current::FromString<uint64_t>(s.substr(1)));
  }
  return OID::INVALID_COMMENT;
}

inline std::string NIDToString(const NID nid) {
  return current::strings::Printf("n%020llu", static_cast<uint64_t>(nid));
}

inline NID StringToNID(const std::string& s) {
  if (s.length() == 21 && s[0] == 'n') {  // 'n' + 20 digits of `uint64_t` decimal representation.
    return static_cast<NID>(current::FromString<uint64_t>(s.substr(1)));
  }
  return NID::INVALID_NOTIFICATION;
}
}  // namespace CTFO

#endif  // CTFO_UTIL_H
