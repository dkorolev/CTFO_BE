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

#ifndef CTFO_SCHEMA_BASE_H
#define CTFO_SCHEMA_BASE_H

#include <functional>

#include "../Current/Sherlock/sherlock.h"

#include "../Current/Bricks/util/comparators.h"  // For `CurrentHashFunction`.
#include "../Current/TypeSystem/struct.h"
#include "../Current/TypeSystem/enum.h"

// This ugliness is to be removed, right on the next round of type evolution. -- D.K.
namespace current {
namespace strings {

template <typename FST, typename SND>
struct ToStringImpl<std::pair<FST, SND>, false, false> {
  static std::string DoIt(const std::pair<FST, SND>& pair) {
    return ToString(pair.first) + '-' + ToString(pair.second);
  }
};

template <typename INPUT, typename OUTPUT_FST, typename OUTPUT_SND>
struct FromStringImpl<INPUT, std::pair<OUTPUT_FST, OUTPUT_SND>, false, false> {
  static const std::pair<OUTPUT_FST, OUTPUT_SND>& Go(const std::string& input,
                                                     std::pair<OUTPUT_FST, OUTPUT_SND>& output) {
    const size_t dash = input.find('-');
    FromString(input.substr(0, dash), output.first);
    if (dash != std::string::npos) {
      FromString(input.substr(dash + 1), output.second);
    } else {
      output.second = OUTPUT_SND();
    }
    return output;
  }
};

}  // namespace current::strings
}  // namespace current

namespace CTFO {

CURRENT_ENUM(UID, uint64_t){INVALID_USER = 0u};
CURRENT_ENUM(CID, uint64_t){INVALID_CARD = 0u};

using UIDAndCID = std::pair<UID, CID>;

}  // namespace CTFO

namespace std {
template <>
struct hash<CTFO::UIDAndCID> {
  size_t operator()(const CTFO::UIDAndCID& s) const {
    return current::CurrentHashFunction<CTFO::UIDAndCID>()(s);
  }
};
}

namespace CTFO {

CURRENT_ENUM(OID, uint64_t){INVALID_COMMENT = 0u};
CURRENT_ENUM(NID, uint64_t){INVALID_NOTIFICATION = 0u};
CURRENT_ENUM(ANSWER, int8_t){UNSEEN = 0, CTFO = 1, TFU = 2, SKIP = -1};
CURRENT_ENUM(AUTH_TYPE, uint8_t){UNDEFINED = 0u, IOS = 1u};
CURRENT_ENUM(SHARE_DESTINATION, uint8_t){UNDEFINED = 0u, FACEBOOK = 101};
CURRENT_ENUM(SHARE_STATUS, uint8_t){UNDEFINED = 0u,
                                    COMPLETE_SHARE_TO_FACEBOOK = 1u,
                                    START_SHARE_TO_FACEBOOK = 101u,
                                    CANCEL_SHARE_TO_FACEBOOK = 111u,
                                    FAIL_SHARE_TO_FACEBOOK = 112u};

// To parse incoming Midichlorians logs.
enum class LOG_EVENT : int {
  SKIP = static_cast<int>(ANSWER::SKIP),
  CTFO = static_cast<int>(ANSWER::CTFO),
  TFU = static_cast<int>(ANSWER::TFU),
  FAV_CARD = 101,
  UNFAV_CARD = 102,
  LIKE_COMMENT = 201,
  UNLIKE_COMMENT = 202,
  FLAG_COMMENT = 203,
  FLAG_CARD = 301,
  REPORT_USER = 401,
  BLOCK_USER = 402,
  ONE_SIGNAL_USER_ID = 501,
  COMPLETE_SHARE_TO_FACEBOOK = 601,
  START_SHARE_TO_FACEBOOK = 602,
  CANCEL_SHARE_TO_FACEBOOK = 603,
  FAIL_SHARE_TO_FACEBOOK = 604
};

CURRENT_STRUCT(Color) {
  CURRENT_FIELD(red, uint8_t);
  CURRENT_FIELD(green, uint8_t);
  CURRENT_FIELD(blue, uint8_t);

  CURRENT_DEFAULT_CONSTRUCTOR(Color) : red(0u), green(0u), blue(0u) {}
  CURRENT_CONSTRUCTOR(Color)(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
};

static current::sherlock::SherlockNamespaceName SchemaKey() {
  return current::sherlock::SherlockNamespaceName("CTFO_2016_08_01", "CTFOEvent");
}

}  // namespace CTFO

#endif  // CTFO_SCHEMA_BASE_H
