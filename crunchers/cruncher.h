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

#ifndef CRUNCHER_H
#define CRUNCHER_H

#include "../../Current/Blocks/SS/ss.h"

namespace current {

namespace sherlock {

template <typename IMPL>
struct EntryCruncherImpl : public IMPL {
  using EntryResponse = current::ss::EntryResponse;
  using TerminationResponse = current::ss::TerminationResponse;
  using entry_t = typename IMPL::entry_t;
	
  EntryCruncherImpl() {}
  virtual ~EntryCruncherImpl() {}

  EntryResponse operator()(const entry_t& entry, idxts_t current, idxts_t) {
    IMPL::OnEvent(entry, current);
    return EntryResponse::More;
  }
	
  EntryResponse EntryResponseIfNoMorePassTypeFilter() const { return EntryResponse::More; }
  TerminationResponse Terminate() const { return TerminationResponse::Terminate; }
};

template <typename IMPL>
using StreamCruncher = current::ss::StreamSubscriber<EntryCruncherImpl<IMPL>, typename IMPL::entry_t>;

}  // namespace sherlock

}  // namespace current

#endif  // CRUNCHER_H
