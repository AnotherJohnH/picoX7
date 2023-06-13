//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

#include "Oscillator.h"
#include "Gain.h"
#include "Table_sine.h"

namespace SYN
{
   //! Sine wave oscillator
   class Sine : public Oscillator
   {
   public:
      Sine() = default;

      Sample operator()()
      {
         uint32_t frac;
         unsigned index_a  = samplePhase<LOG2_TABLE_SINE_SIZE>(frac);
         unsigned index_b  = (index_a + 1) & TABLE_SINE_MASK;
         Sample   sample_a = table_sine[index_a];
         Sample   sample_b = table_sine[index_b];
         Sample   sample   = interpolate(sample_a, sample_b, frac);

         return Sample(gain * sample);
      }

      Gain gain{1.0};
      Freq freq{440.0};
   };
}
