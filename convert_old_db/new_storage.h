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

#ifndef NEW_STORAGE_H
#define NEW_STORAGE_H

#include "new_schema.h"

#include "../../Current/Storage/storage.h"
#include "../../Current/Storage/persister/sherlock.h"

CURRENT_STORAGE_FIELD_ENTRY(UnorderedDictionary, User, Persisted_User);
CURRENT_STORAGE_FIELD_ENTRY(UnorderedDictionary, Card, Persisted_Card);

CURRENT_STORAGE(NewCTFO) {
  CURRENT_STORAGE_FIELD(user, Persisted_User);
  CURRENT_STORAGE_FIELD(card, Persisted_Card);
};

#endif  // NEW_STORAGE_H
