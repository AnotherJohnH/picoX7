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

#include "SysEx.h"

class EnvGen
{
public:
   EnvGen() = default;

   //! Program the level and rate
   void prog(const SysEx::Envelope& env, uint8_t out_level)
   {
#if 0 
      for(unsigned n = 0; n < 4; ++n)
      {
         if (n < 4) n -= 1;

         R[n] = rate_;
         L[n] = level_;

         if (n == 2)
         {
            R[3] = 0;
            L[3] = L[2];
         }
      }
#endif
   }

   //! Start a note
   void keyOn()
   {
      ampl = L[4];
      setIndex(0);
   }

   //! Release a note
   void keyOff()
   {
      setIndex(4);
   }

   //! Get sample
   int32_t operator()()
   {
      ampl += rate;

      if (rise == (ampl >= level))
      {
         ampl = level;
         setIndex(index + 1);
      }

      return ampl;
   }

private:
   void setIndex(uint8_t index_)
   {
      index = index_;
      rate  = R[index];
      level = L[index];
      rise  = ampl < level;
   }

   int32_t ampl{0};
   int32_t rate{0};
   int32_t level{0};
   uint8_t index{0};
   bool    rise{false};
   int32_t R[5] = {};
   int32_t L[5] = {};
};
