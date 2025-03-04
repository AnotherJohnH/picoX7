//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
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

#include "DX7/PitchEg.h"
#include "DX7/SysEx.h"

#include "STB/Test.h"

TEST(PitchEg, triangle)
{
   PitchEg<1>   pitch_eg;
   SysEx::Voice patch;

   patch.eg_pitch.rate[0] = 75;
   patch.eg_pitch.rate[1] = 75;
   patch.eg_pitch.rate[2] = 75;
   patch.eg_pitch.rate[3] = 75;

   patch.eg_pitch.level[0] = 0;
   patch.eg_pitch.level[1] = 99;
   patch.eg_pitch.level[2] = 0;
   patch.eg_pitch.level[3] = 99;

   pitch_eg.load(patch);

   pitch_eg.keyOn(0);

   for(unsigned t = 0; t < 3000; ++t)
   {
      if (t >= 2200)
         pitch_eg.keyOff(0);

      if (pitch_eg.tick())
      {
         printf("%5d, 0x%04x\n", t, pitch_eg.getOutput(0));
      }
   }
}

TEST(PitchEg, flat)
{
   PitchEg<1>   pitch_eg;
   SysEx::Voice patch;

   patch.eg_pitch.rate[0] = 99;
   patch.eg_pitch.rate[1] = 99;
   patch.eg_pitch.rate[2] = 99;
   patch.eg_pitch.rate[3] = 99;

   patch.eg_pitch.level[0] = 50;
   patch.eg_pitch.level[1] = 50;
   patch.eg_pitch.level[2] = 50;
   patch.eg_pitch.level[3] = 50;

   pitch_eg.load(patch);

   pitch_eg.keyOn(0);

   for(unsigned t = 0; t < 10; ++t)
   {
      if (pitch_eg.tick())
      {
         printf("%5d, 0x%04x\n", t, pitch_eg.getOutput(0));
      }
   }
}
