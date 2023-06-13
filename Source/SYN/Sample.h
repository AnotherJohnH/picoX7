//------------------------------------------------------------------------------
// Copyright (c) 2022 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "STB/FixP.h"

namespace SYN
{
   using Sample                      = int16_t; //!< 16 bit signed
   static const Sample   SAMPLE_MAX  = INT16_MAX;
   static const Sample   SAMPLE_MIN  = INT16_MIN;
   static const unsigned SAMPLE_BITS = sizeof(Sample) * 8;

   static const unsigned SAMPLE_FREQ = 48000;   //!< 48 KHz

   //! Interpolate two samples using integer arithmatic
   //! frac is a fixed point value with 32 binary places
   inline Sample interpolate(Sample sample_a, Sample sample_b, uint32_t frac)
   {
      using Scale = STB::FixP<16>;

      Scale b = Scale::fromRaw(frac >> (32 - Scale::FRAC_BITS));
      Scale a = Scale(1) - b;

      return Sample(a * sample_a + b * sample_b);
   }
}
