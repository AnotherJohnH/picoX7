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

#define DEBUG if (1) printf

class AlwaysOnEG
{
public:
   AlwaysOnEG() = default;

   void setAtten12(uint32_t atten12_)
   {
      atten12 = atten12_;
   }

   uint32_t getAtten12() { return atten12; }

private:
   uint32_t atten12{0x000};
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
      uint32_t f14 = (note14_ + RATIO_1_0) & 0x3FFF;
      setOpsFreq(0, f14);
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
   SingleOp    ops{};
   AlwaysOnEG* eg = ops.getEgsPointer(0);

   eg->setAtten12(0x000);

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

   DEBUG("first = %d\n", first);
   DEBUG("min   = -0x%04X\n", -min);
   DEBUG("max   = +0x%04X\n", +max);
   DEBUG("avg   = %.2f\n", avg);

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


TEST(Ops, attenuation)
{
   SingleOp    ops{};
   AlwaysOnEG* eg = ops.getEgsPointer(0);

   struct Test
   {
      uint32_t atten12;
      signed   exp_max;
   };

   std::vector<Test> test_vector =
   {
      {0x000, 0x8000},
      {0x200, 0x4000},
      {0x400, 0x2000},
      {0x600, 0x1000},
      {0x800, 0x0800},
      {0xA00, 0x0400},
      {0xC00, 0x0200},
      {0xE00, 0x0100},
      {0xFFE, 0x0080},
      {0xFFF, 0x0000},
   };

   for(const auto& test : test_vector)
   {
      eg->setAtten12(test.atten12);

      ops.noteOn(/* note14 (A4) */ 0x1000);

      signed max = 0;

      static const unsigned SAMPLES = 49096; // One seconds

      for(unsigned i = 0; i < SAMPLES; ++i)
      {
         int32_t sample = ops.getSample();

         if (sample > max)
            max = sample;
      }

      signed error = test.exp_max - max;

      DEBUG("atten12=0x%03X max = +0x%04X err = %u\n", test.atten12, max, error);

      EXPECT_GT(0x20, abs(error));
   }
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

   DEBUG("max_delta = 0x%04X\n", max_delta);

   // The following should catch clipping/overflow issues
   EXPECT_GT(0x800, max_delta);
}


TEST(Ops, freq)
{
   SingleOp ops;

   struct Test
   {
      uint32_t note14;
      double   exp_freq;
   };

   std::vector<Test> test_vector =
   {
   //  note14,  freq (Hz)
      {0xECDC,       1.0},  // Lowest frequency required in fixed mode
      {0xF000,   1.71875},  // A-4
      {0xF400,    3.4375},  // A-3
      {0xF800,     6.875},  // A-2
      {0xFC00,     13.75},  // A-1
      {0x0000,      27.5},  // A0
      {0x0400,        55},  // A1
      {0x0800,       110},  // A2
      {0x0C00,       220},  // A3
      {0x1000,       440},  // A4
      {0x1400,       880},  // A5
      {0x1800,      1760},  // A6
      {0x1C00,      3520},  // A7
      {0x2000,      7040},  // A8
      {0x2356,  12543.85},  // G9
      {0x2400,     14080},  // A9
      {0x2700,  23679.64},  // F#10
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
      double octave     = log2(freq);
      double exp_octave = log2(test.exp_freq);
      double cent_error = fabs(octave - exp_octave) * 12 * 100;

      DEBUG("note14=0x%04x freq=%8.2f Hz error=%.1f cents\n",
             test.note14, freq, cent_error);

      if (freq >= 50)
         EXPECT_GT(1.0, cent_error);
      else
         EXPECT_GT(20.0, cent_error);
   }
}

