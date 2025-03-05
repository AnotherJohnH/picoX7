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

#include "DX7/Ops.h"

#include <cmath>
#include <cstdlib>
#include <vector>

#include "STB/Test.h"

class AlwaysOnEG
{
public:
   AlwaysOnEG() = default;

   uint32_t getAtten12() { return 0x000; }
};


class SingleOp : public Ops<1,AlwaysOnEG>
{
public:
   SingleOp()
   {
      setOpsSync(true);
      setOpsFdbk(0);
   }

   void noteOn(uint32_t note14_)
   {
      setOpsFreq(0, note14_ + RATIO_1_0);
      keyOn();
   }

   int32_t getSample()
   {
      return ops</* OP_NUMBER */ 1, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* LOG2_COM */ 0b00000>();
   }

   static const uint32_t RATIO_1_0 = 0x8CB;
};


TEST(Ops, amplitude)
{
   SingleOp ops;

   ops.noteOn(/* note14 (A4) */ 0x1000);

   int32_t first = 0;
   int32_t min   = 0;
   int32_t max   = 0;
   double  avg   = 0.0;

   static const unsigned SAMPLES = 10 * 49096; // Ten seconds

   for(unsigned i = 0; i < SAMPLES; ++i)
   {
      int32_t sample = ops.getSample();

      if (i == 0)
         first = sample;

      if (sample < min)
         min = sample;
      else if (sample > max)
         max = sample;

      avg += sample;
   }

   avg = avg / SAMPLES;

   printf("first = %d\n", first);
   printf("min   = -0x%04X\n", -min);
   printf("max   = +0x%04X\n", +max);
   printf("avg   = %.2f\n", avg);

   // Check first sample is not negative and small
   EXPECT_LT(-1, first);
   EXPECT_GT(0x800, first);

   // Check amplitude is close to full range 16 bits
   EXPECT_LT(-0x8000, min);
   EXPECT_GT(-0x7FE0, min);
   EXPECT_LT( 0x7FE0, max);
   EXPECT_GT( 0x8000, max);

   // Check average is close to zero
   EXPECT_GT(+2.0, avg);
   EXPECT_LT(-2.0, avg);
}


TEST(Ops, continuity)
{
   SingleOp ops;

   ops.noteOn(/* note14 (A4) */ 0x1000);

   static const unsigned SAMPLES = 49096; // One seconds

   int32_t last_sample{0};
   int32_t max_delta{0};

   for(unsigned i = 0; i < SAMPLES; ++i)
   {
      int32_t sample = ops.getSample();
 
      int32_t delta = abs(sample - last_sample);
      last_sample = sample;

      if (delta > max_delta)
         max_delta = delta;
   }

   printf("max_delta = 0x%04X\n", max_delta);

   // The following should catch clipping/overflow issues
   EXPECT_GT(0x800, max_delta);
}


TEST(Ops, freq)
{
   struct Test
   {
      uint32_t note14;
      double   exp_freq;
      double   err_lim;
   };

   SingleOp ops;

   std::vector<Test> test_vector =
   {
   //  note14, freq (Hz), err_lim (Hz)
      {0x0000,      27.5,           1},  // A0
      {0x0400,        55,           1},  // A1
      {0x0800,       110,           1},  // A2
      {0x0C00,       220,           1},  // A3
      {0x1000,       440,           1},  // A4
      {0x1400,       880,           1},  // A5
      {0x1800,      1760,           1},  // A6
      {0x1C00,      3520,           1},  // A7
      {0x2000,      7040,           2},  // A8
      {0x2356,     12544,           6},  // G9
   };

   static const unsigned SECONDS = 100;
   static const unsigned SAMPLES = SECONDS * 49096;

   for(const auto& test : test_vector)
   {
      ops.noteOn(test.note14);

      int32_t  last_sample = 0;
      unsigned cycles = 0;

      for(unsigned i = 0; i < SAMPLES; ++i)
      {
         int32_t sample = ops.getSample();

         if ((last_sample >= 0) && (sample < 0))
         {
            ++cycles;
         }

         last_sample = sample;
      }

      double freq       = double(cycles) / SECONDS;
      double freq_error = fabs(freq - test.exp_freq);

      printf("note14=0x%04x freq=%8.2f Hz error=%g Hz\n",
             test.note14, freq, freq_error);

      EXPECT_GT(test.err_lim, freq_error);
   }
}

