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

#ifndef CTFO_SCHEMA_H
#define CTFO_SCHEMA_H

#include <vector>
#include <string>

#include "../Current/Bricks/cerealize/cerealize.h"
#include "../Current/Yoda/yoda.h"

// Common data structures.
struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  Color() : red(0u), green(0u), blue(0u) {}
  Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
  Color(const Color&) = default;

  template <typename A>
  void serialize(A& ar) {
    ar(CEREAL_NVP(red), CEREAL_NVP(green), CEREAL_NVP(blue));
  }
};

// Constants.
const std::vector<unsigned int> LEVEL_SCORES{
    0,        // "Fish"
    15000,    // "Turkey"
    30000,    // "Rat"
    60000,    // "Pig"
    120000,   // "Octopus"
    240000,   // "Raven"
    480000,   // "Dolphin"
    960000,   // "Elephant"
    1920000,  // "Chimp"
    3840000   // "Skrik"
};

const std::vector<Color> CARD_COLORS{{0x0A, 0xB2, 0xCB},
                                     {0x0A, 0xCB, 0xCB},
                                     {0x0A, 0xCB, 0xAB},
                                     {0x0B, 0xE6, 0x9D},
                                     {0x0B, 0xBA, 0xE6},
                                     {0x0B, 0x95, 0xE6},
                                     {0x0F, 0xCE, 0xB9},
                                     {0x10, 0xDE, 0x81},
                                     {0x10, 0xDE, 0x96},
                                     {0x11, 0xE8, 0x79},
                                     {0x31, 0xDE, 0x83},
                                     {0x4C, 0xED, 0x61},
                                     {0x1F, 0xCD, 0x85},
                                     {0x8F, 0xD6, 0x24},
                                     {0x48, 0xD6, 0x24},
                                     {0x42, 0xDE, 0x1B},
                                     {0x1B, 0xDE, 0x42},
                                     {0x27, 0xE3, 0x7B},
                                     {0x6E, 0xD9, 0x3A},
                                     {0x78, 0xD6, 0x2F},
                                     {0x84, 0xD1, 0x2C},
                                     {0xA9, 0xDE, 0x37},
                                     {0xB0, 0xDB, 0x39},
                                     {0xC1, 0xE3, 0x39},
                                     {0xEC, 0xDE, 0x00},
                                     {0xED, 0xE9, 0x09},
                                     {0xED, 0xDE, 0x11},
                                     {0xF2, 0xF2, 0x00},
                                     {0xF5, 0xE9, 0x00},
                                     {0xFC, 0xE7, 0x00},
                                     {0xEC, 0x60, 0x3F},
                                     {0xF0, 0x6E, 0x43},
                                     {0xFA, 0x6E, 0x4B},
                                     {0xFF, 0x81, 0x69},
                                     {0xFF, 0xA6, 0x91},
                                     {0xFF, 0x99, 0x85},
                                     {0xD2, 0x37, 0x67},
                                     {0xE6, 0x30, 0x68},
                                     {0xF5, 0x42, 0x79},
                                     {0xE3, 0x5B, 0x85},
                                     {0xF2, 0x8F, 0xAE},
                                     {0xFF, 0x3B, 0x77},
                                     {0xB8, 0x38, 0x7D},
                                     {0xEB, 0x38, 0x9A},
                                     {0xFA, 0x57, 0xB1},
                                     {0xFF, 0x40, 0xA9},
                                     {0xED, 0x1F, 0x90},
                                     {0xFF, 0x08, 0x90},
                                     {0xA0, 0x4F, 0xDE},
                                     {0xB9, 0x64, 0xFA},
                                     {0xB6, 0x69, 0xFF},
                                     {0xCA, 0x91, 0xFF},
                                     {0xC4, 0x9E, 0xE8},
                                     {0xB3, 0x97, 0xDE},
                                     {0x74, 0x4A, 0xCA},
                                     {0x8B, 0x5D, 0xD9},
                                     {0x9D, 0x73, 0xE6},
                                     {0xA6, 0x79, 0xF2},
                                     {0xB3, 0x88, 0xFC},
                                     {0x98, 0x88, 0xFC},
                                     {0x4C, 0x5C, 0xC0},
                                     {0x55, 0x66, 0xD4},
                                     {0x63, 0x82, 0xEB},
                                     {0x67, 0x79, 0xEB},
                                     {0x72, 0x84, 0xF2},
                                     {0x83, 0x93, 0xF7},
                                     {0x2B, 0x73, 0xDB},
                                     {0x2B, 0x7A, 0xED},
                                     {0x38, 0x85, 0xF5},
                                     {0x4B, 0x93, 0xFA},
                                     {0x60, 0x99, 0xEB},
                                     {0x7D, 0xA9, 0xE8},
                                     {0xC9, 0x49, 0xC7},
                                     {0xF0, 0x46, 0xED},
                                     {0xF0, 0x5D, 0xED},
                                     {0xDE, 0x6F, 0xDC},
                                     {0xE8, 0x87, 0xE7},
                                     {0xCC, 0x81, 0xCB},
                                     {0x3E, 0xEE, 0x3E},
                                     {0x43, 0xDE, 0x43},
                                     {0x37, 0xD4, 0x37},
                                     {0x6A, 0xF7, 0x6A},
                                     {0x6A, 0xE6, 0x6A},
                                     {0x79, 0xED, 0x8E},
                                     {0xEC, 0x3F, 0x3F},
                                     {0xFC, 0x44, 0x44},
                                     {0xFC, 0x35, 0x35},
                                     {0xEB, 0x10, 0x10},
                                     {0xE3, 0x09, 0x09},
                                     {0xF2, 0x00, 0x00},
                                     {0x02, 0x98, 0xE4},
                                     {0x00, 0xA6, 0xFA},
                                     {0x21, 0xB4, 0xFF},
                                     {0x3D, 0xB8, 0xFF},
                                     {0x49, 0xA2, 0xD6},
                                     {0x54, 0xA0, 0xCC},
                                     {0xFF, 0x5E, 0x9A},
                                     {0xFF, 0x7A, 0x9C},
                                     {0xFF, 0x8C, 0xA9},
                                     {0xED, 0x98, 0xAD},
                                     {0xE3, 0xA8, 0xB7},
                                     {0xF0, 0xBD, 0xCA},
                                     {0xC2, 0xEC, 0x00},
                                     {0xB8, 0xDB, 0x16},
                                     {0xC8, 0xED, 0x1F},
                                     {0xAC, 0xED, 0x1F},
                                     {0xB5, 0xF2, 0x30},
                                     {0xAB, 0xE3, 0x32},
                                     {0xF2, 0x81, 0x01},
                                     {0xFF, 0x87, 0x00},
                                     {0xFF, 0x9D, 0x00},
                                     {0xFF, 0xAC, 0x26},
                                     {0xF0, 0xB2, 0x4F},
                                     {0xF5, 0xC6, 0x7A}};

// Data structures for internal storage.
enum class UID : uint64_t { INVALID = 0u };
enum class CID : uint64_t { INVALID = 0u };
enum class ANSWER : int { UNSEEN = 0, CTFO = 1, TFU = 2, SKIP = -1 };
enum class AUTH_TYPE : int { UNDEFINED = 0, IOS };

struct User : yoda::Padawan {
  UID uid = UID::INVALID;
  uint8_t level = 0u;   // User level [0, 9].
  uint64_t score = 0u;  // User score.

  UID key() const { return uid; }
  void set_key(UID value) { uid = value; }

  template <typename A>
  void serialize(A& ar) {
    Padawan::serialize(ar);
    ar(CEREAL_NVP(uid), CEREAL_NVP(level), CEREAL_NVP(score));
  }
};

// AuthKey structure defines generic authentication key.
// `key` is supposed to be a long string, combined from several parameters (depending on the authentication
// type) with "::" as a delimiter.
// For `type = AUTH_TYPE::IOS`:
//   key = "iOS::" + <Device ID> + "::" + <Application Key>
//   Application key is a random number, generated once in a lifetime in iOS application on its first launch.
struct AuthKey {
  std::string key = "";
  AUTH_TYPE type = AUTH_TYPE::UNDEFINED;

  AuthKey() = default;
  AuthKey(const std::string& key, AUTH_TYPE type) : key(key), type(type) {}
  size_t Hash() const { return std::hash<std::string>()(key); }
  bool operator==(const AuthKey& rhs) const { return key == rhs.key && type == rhs.type; }

  template <typename A>
  void serialize(A& ar) {
    ar(CEREAL_NVP(key), CEREAL_NVP(type));
  }
};

struct AuthKeyTokenPair : yoda::Padawan {
  AuthKey auth_key;
  std::string token = "";
  bool valid = false;

  AuthKeyTokenPair() = default;
  AuthKeyTokenPair(const AuthKeyTokenPair&) = default;
  AuthKeyTokenPair(const AuthKey& auth_key, const std::string& token, bool valid = false)
      : auth_key(auth_key), token(token), valid(valid) {}

  const AuthKey& row() const { return auth_key; }
  void set_row(const AuthKey& value) { auth_key = value; }
  const std::string& col() const { return token; }
  void set_col(const std::string& value) { token = value; }

  template <typename A>
  void serialize(A& ar) {
    Padawan::serialize(ar);
    ar(CEREAL_NVP(auth_key), CEREAL_NVP(token), CEREAL_NVP(valid));
  }
};

struct AuthKeyUIDPair : yoda::Padawan {
  AuthKey auth_key;
  UID uid = UID::INVALID;

  AuthKeyUIDPair() = default;
  AuthKeyUIDPair(const AuthKey& auth_key, const UID uid) : auth_key(auth_key), uid(uid) {}

  const AuthKey& row() const { return auth_key; }
  void set_row(const AuthKey& value) { auth_key = value; }
  UID col() const { return uid; }
  void set_col(UID value) { uid = value; }

  template <typename A>
  void serialize(A& ar) {
    Padawan::serialize(ar);
    ar(CEREAL_NVP(auth_key), CEREAL_NVP(uid));
  }
};

struct Card : yoda::Padawan {
  CID cid = CID::INVALID;
  std::string text = "";     // Plain text.
  Color color;               // Color.
  uint64_t ctfo_count = 0u;  // Number of users, who said "CTFO" on this card.
  uint64_t tfu_count = 0u;   // Number of users, who said "TFU" on this card.
  uint64_t skip_count = 0u;  // Number of users, who said "SKIP" on this card.

  Card() = default;
  Card(const Card&) = default;
  Card(CID cid, const std::string& text, const Color& color) : cid(cid), text(text), color(color) {}

  CID key() const { return cid; }
  void set_key(CID value) { cid = value; }

  template <typename A>
  void serialize(A& ar) {
    Padawan::serialize(ar);
    ar(CEREAL_NVP(cid),
       CEREAL_NVP(text),
       CEREAL_NVP(color),
       CEREAL_NVP(ctfo_count),
       CEREAL_NVP(tfu_count),
       CEREAL_NVP(skip_count));
  }
};

struct Answer : yoda::Padawan {
  UID uid = UID::INVALID;
  CID cid = CID::INVALID;
  ANSWER answer = ANSWER::UNSEEN;

  Answer() = default;
  Answer(const Answer&) = default;
  Answer(UID uid, CID cid, ANSWER answer) : uid(uid), cid(cid), answer(answer) {}

  UID row() const { return uid; }
  void set_row(UID value) { uid = value; }
  CID col() const { return cid; }
  void set_col(CID value) { cid = value; }

  template <typename A>
  void serialize(A& ar) {
    Padawan::serialize(ar);
    ar(CEREAL_NVP(uid), CEREAL_NVP(cid), CEREAL_NVP(answer));
  }
};

struct Favorite : yoda::Padawan {
  UID uid = UID::INVALID;
  CID cid = CID::INVALID;
  bool favorited = false;

  Favorite() = default;
  Favorite(const UID uid, const CID cid, bool favorited = false) : uid(uid), cid(cid), favorited(favorited) {}

  const UID& row() const { return uid; }
  void set_row(const UID& value) { uid = value; }
  CID col() const { return cid; }
  void set_col(CID value) { cid = value; }

  template <typename A>
  void serialize(A& ar) {
    Padawan::serialize(ar);
    ar(CEREAL_NVP(uid), CEREAL_NVP(cid), CEREAL_NVP(favorited));
  }
};

// Data structures for generating RESTful response.
struct ResponseUserEntry {
  std::string uid = "uINVALID";    // User id, format 'u01XXX...'.
  std::string token = "";          // User token.
  uint8_t level = 0u;              // User level, [0, 9].
  uint64_t score = 0u;             // User score.
  uint64_t next_level_score = 0u;  // Score value when user is promoted to the next level.

  template <typename A>
  void serialize(A& ar) {
    ar(CEREAL_NVP(uid), CEREAL_NVP(token), CEREAL_NVP(level), CEREAL_NVP(score), CEREAL_NVP(next_level_score));
  }
};

struct ResponseCardEntry {
  std::string cid = "cINVALID";  // Card id, format 'c02XXX...'.
  std::string text = "";         // Card text.
  Color color;                   // Card color.
  double relevance = 0.0;        // Card relevance for particular user, [0.0, 1.0].
  uint64_t ctfo_score = 0u;      // Number of points, which user gets for "CTFO" answer.
  uint64_t tfu_score = 0u;       // Number of points, which user gets for "TFU" answer.
  uint64_t ctfo_count = 0u;      // Number of users, who said "CTFO" on this card.
  uint64_t tfu_count = 0u;       // Number of users, who said "TFU" on this card.
  uint64_t skip_count = 0u;      // Number of users, who said "SKIP" on this card.
  bool favorited = false;        // True if the current user has favorited this card.

  template <typename A>
  void serialize(A& ar) {
    ar(CEREAL_NVP(cid),
       CEREAL_NVP(text),
       CEREAL_NVP(color),
       CEREAL_NVP(relevance),
       CEREAL_NVP(ctfo_score),
       CEREAL_NVP(tfu_score),
       CEREAL_NVP(ctfo_count),
       CEREAL_NVP(tfu_count),
       CEREAL_NVP(skip_count),
       CEREAL_NVP(favorited));
  }
};

// Universal response structure, combining user info & cards payload.
struct ResponseFeed {
  uint64_t ms;                                 // Server timestamp, milliseconds from epoch.
  ResponseUserEntry user;                      // User information.
  std::vector<ResponseCardEntry> feed_hot;     // "Hot" cards feeds.
  std::vector<ResponseCardEntry> feed_recent;  // "Recent" cards feeds.

  template <typename A>
  void serialize(A& ar) {
    ar(CEREAL_NVP(ms), CEREAL_NVP(user), CEREAL_NVP(feed_hot), CEREAL_NVP(feed_recent));
  }
};

// To parse incoming Midichlorians logs.
enum class RESPONSE : int {
  SKIP = static_cast<int>(ANSWER::SKIP),
  CTFO = static_cast<int>(ANSWER::CTFO),
  TFU = static_cast<int>(ANSWER::TFU),
  FAV = 101,
  UNFAV = 102
};

#endif  // CTFO_SCHEMA_H
