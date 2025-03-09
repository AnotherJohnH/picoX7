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

#include "DX7/EgsOpState.h"

#include "STB/Test.h"

#define DEBUG if (1) printf


struct TestEG
{
public:
   TestEG() = default;

   void setAtten8(unsigned i, unsigned atten_8_)
   {
      EXPECT_GT(NUM_CONTROL, i);
      EXPECT_LT(i, NUM_CONTROL);
      EXPECT_GT(0x100, atten_8_);

      atten_8[i] = atten_8_;
   }

   void setRate6(unsigned i, unsigned rate_6_)
   {
      EXPECT_GT(NUM_CONTROL, i);
      EXPECT_GT(0x40, rate_6_);

      rate_6[i] = rate_6_;
   }

   void setAmpMod(unsigned amp_mod_12_)
   {
      EXPECT_GT(0x1000, amp_mod_12_);

      amp_mod_12 = amp_mod_12_;
   }

   static const unsigned NUM_CONTROL = 2;

   uint16_t amp_mod_12{0};
   uint8_t  atten_8[NUM_CONTROL] = {};
   uint8_t  rate_6[NUM_CONTROL] = {};
};


TEST(EgsOpState, basic)
{
   TestEG             eg{};
   EgsOpState<TestEG> op;

   op.init(&eg);

   // Configure frequency
   op.setPitchRatio(0x1000);
   op.setPitchFixed(false);
   op.setDetune(0);

   // Configure amplitude
   op.setEgAtten(0, 0x00);
   op.setEgAtten(1, 0x3F);
   op.setEgRate( 0, 0x00);
   op.setEgRate( 1, 0x3F);
   op.setEgRateScale(0);
   op.setAmpModSens(0);

   // Note-on setup
   op.setOpAtten(0x00);

   // Modulation
   op.setAmpMod(0x00);
   op.updateRates(0x1000);
   op.computeOpsFreq14(0x1000, 0x0000);
}
