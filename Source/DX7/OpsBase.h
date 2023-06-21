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

#include "Table_dx7_exp_14.h"
#include "Table_dx7_exp_22.h"
#include "Table_dx7_sine_15.h"

class OpsBase
{
public:
   OpsBase() = default;

   //! Set algporithm feedback
   void setFbk(unsigned fbl_)
   {
      fbl = fbl_ + 1 + 3;
   }

   //! Set operatror frequency as a phase increment
   void setFrq(unsigned op, unsigned f14_)
   {
      phase_inc_32[op] = f14_ << 18;
   }

   //! Set operatror amplitude
   void setAmp(unsigned op, unsigned ec12_)
   {
      amp_14[op] = table_dx7_exp_14[ec12_ << 2];
   }

protected:
   template <unsigned OP, unsigned SEL, bool A, bool C, bool D, unsigned COM>
   int32_t op()
   {
      phase_accum_32[OP] += phase_inc_32[OP];

      uint32_t phase_12  = (phase_accum_32[OP] + (modulation_12 << 18)) >> 20;
      int32_t  output_15 = (table_dx7_sine_15[phase_12] * amp_14[OP] / signed(1 + COM)) >> 14;

      int32_t sum_15 = 0;
      if (C) sum_15 = memory_15;
      if (D) sum_15 += output_15;

      switch(SEL)
      {
      case 0:
         modulation_12 = 0;
         break;

      case 1:
         modulation_12 = output_15 >> 3;
         break;

      case 2:
         modulation_12 = sum_15 >> 3;
         break;

      case 3:
         modulation_12 = memory_15 >> 3;
         break;

      case 4:
         modulation_12 = feedback1_15 >> 3;
         feedback1_15  = output_15;
         break;

      case 5:
         modulation_12 = (feedback1_15 + feedback2_15) >> fbl;
         break;
      }

      if (A)
      {
         feedback2_15 = feedback1_15;
         feedback1_15 = output_15;
      }

      if (C or D)
      {
         memory_15 = sum_15;
      }

      return sum_15 << 1;
   }

private:
   uint8_t  fbl {0};

   uint32_t phase_accum_32[6] = {0};
   uint32_t phase_inc_32[6]   = {0};
   int32_t  amp_14[6] = {0};

   int32_t  modulation_12 {0};
   int32_t  feedback1_15 {0};
   int32_t  feedback2_15 {0};
   int32_t  memory_15 {0};
};
