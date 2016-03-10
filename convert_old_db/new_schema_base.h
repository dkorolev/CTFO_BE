/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Dmitry "Dima" Korolev <dmitry.korolev@gmail.com>
          (c) 2016 Maxim Zhurovich <zhurovich@gmail.com>

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

#ifndef NEW_SCHEMA_BASE_H
#define NEW_SCHEMA_BASE_H

#include <functional>

#include "../../Current/TypeSystem/struct.h"
#include "../../Current/TypeSystem/enum.h"
#include "../../Current/Storage/container/matrix.h"  // For `PairHash`.

CURRENT_ENUM(UID, uint64_t){INVALID_USER = 0u};
CURRENT_ENUM(CID, uint64_t){INVALID_CARD = 0u};

using UIDAndCID = std::pair<UID, CID>;
namespace std {
template <>
struct hash<UIDAndCID> {
  size_t operator()(const UIDAndCID& s) const { return current::storage::container::PairHash()(s); }
};
}

CURRENT_ENUM(OID, uint64_t){INVALID_COMMENT = 0u};
CURRENT_ENUM(NID, uint64_t){INVALID_NOTIFICATION = 0u};
CURRENT_ENUM(ANSWER, int8_t){UNSEEN = 0, CTFO = 1, TFU = 2, SKIP = -1};
CURRENT_ENUM(AUTH_TYPE, uint8_t){UNDEFINED = 0u, IOS = 1u};

CURRENT_STRUCT(Color) {
  CURRENT_FIELD(red, uint8_t);
  CURRENT_FIELD(green, uint8_t);
  CURRENT_FIELD(blue, uint8_t);

  CURRENT_DEFAULT_CONSTRUCTOR(Color) : red(0u), green(0u), blue(0u) {}
  CURRENT_CONSTRUCTOR(Color)(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
};

#endif  // NEW_SCHEMA_BASE_H
