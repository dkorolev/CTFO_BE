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

namespace CTFO {

// Data structures for internal storage.
enum class UID : uint64_t { INVALID_USER = 0u };
enum class CID : uint64_t { INVALID_CARD = 0u };
enum class OID : uint64_t { INVALID_COMMENT = 0u };
enum class NID : uint64_t { INVALID_NOTIFICATION = 0u };
enum class ANSWER : int { UNSEEN = 0, CTFO = 1, TFU = 2, SKIP = -1 };
enum class AUTH_TYPE : int { UNDEFINED = 0, IOS };

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
  BLOCK_USER = 402
};

}  // namespace CTFO

#endif  // CTFO_SCHEMA_BASE_H
