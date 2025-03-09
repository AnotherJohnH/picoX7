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

#include "DX7/EnvGen.h"

#include "STB/Test.h"


TEST(EnvGen, basic)
{
   EnvGen env_gen{};

   env_gen.setAtten8(0, 0x00);
   env_gen.setAtten8(1, 0x80);
   env_gen.setAtten8(2, 0x40);
   env_gen.setAtten8(3, 0xFF);

   env_gen.setRate6(0, 0x80);
   env_gen.setRate6(1, 0x80);
   env_gen.setRate6(2, 0x80);
   env_gen.setRate6(3, 0x80);

   env_gen.setAmpMod(0);

   uint32_t last_atten_12 = env_gen.getAtten12();
   EXPECT_EQ(0xFFF, last_atten_12);

   unsigned last_phase = env_gen.dbgPhase();
   EXPECT_EQ(5, last_phase);

   env_gen.keyOn();

   EXPECT_LT(0xFFF, env_gen.dbgInternal());
   EXPECT_EQ(0,     env_gen.dbgTarget());

   const unsigned SAMPLES = 49096; // One second

   for(unsigned i = 0; i < SAMPLES; ++i)
   {
      if (i == (SAMPLES / 2))
         env_gen.keyOff();

      uint32_t atten_12 = env_gen.getAtten12();

      EXPECT_GT(1 << 12, atten_12);

      //printf("%03X\n", atten_12);

      unsigned phase = env_gen.dbgPhase();

      bool new_phase = last_phase != phase;

      switch(phase)
      {
      case 0:
         if (new_phase)
         {
            EXPECT_EQ(5,     last_phase);
            EXPECT_EQ(0xFFF, last_atten_12);
         }
         // EXPECT_GE(last_atten_12, atten_12);
         break;

      case 1:
         if (new_phase)
         {
            EXPECT_EQ(0,     last_phase);
            EXPECT_EQ(0x000, atten_12);
         }
         // EXPECT_LE(last_atten_12, atten_12);
         break;

      case 2:
         if (new_phase)
         {
            EXPECT_EQ(1,     last_phase);
         }
         // EXPECT_LE(last_atten_12, atten_12);
         break;

      case 3:
         if (new_phase)
         {
            EXPECT_EQ(2,     last_phase);
         }
         // EXPECT_LE(last_atten_12, atten_12);
         break;

      case 4:
         if (new_phase)
         {
            EXPECT_EQ(3,     last_phase);
         }
         // EXPECT_LE(last_atten_12, atten_12);
         break;

      case 5:
         if (new_phase)
         {
            EXPECT_EQ(4, last_phase);
         }
         EXPECT_EQ(0xFFF, atten_12);
         break;
      }

      last_atten_12 = atten_12;
      last_phase    = phase;
   }

   EXPECT_EQ(5,     last_phase);
   EXPECT_EQ(0xFFF, last_atten_12);
}


// XXX the tests in this file have bit rotted and need improving
#include "DX7/SysEx.h"


const uint8_t table_log[100] =
{
   0x7F, 0x7A, 0x76, 0x72, 0x6E, 0x6B, 0x68, 0x66, 0x64, 0x62,
   0x60, 0x5E, 0x5C, 0x5A, 0x58, 0x56, 0x55, 0x54, 0x52, 0x51,
   0x4F, 0x4E, 0x4D, 0x4C, 0x4B, 0x4A, 0x49, 0x48, 0x47, 0x46,
   0x45, 0x44, 0x43, 0x42, 0x41, 0x40, 0x3F, 0x3E, 0x3D, 0x3C,
   0x3B, 0x3A, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32,
   0x31, 0x30, 0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
   0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20, 0x1F, 0x1E,
   0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14,
   0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A,
   0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
};

TEST(EnvGen, level)
{
   EnvGen eg;

   FILE* fp = fopen("level.csv", "w");

   for(unsigned l = 0; l <= 99; ++l)
   {
      fprintf(fp, "%2u, 0x%08x\n", l, 0x3f000000 - ((table_log[l] >> 1) << 24));
   }

   fclose(fp);
}

TEST(EnvGen, brass_1)
{
   // "BRASS   1 "
   SysEx::EnvGen eg_patch[6] =
   {
      { {72, 76, 99, 71}, {99, 88, 96, 00} },  // OP 1
      { {62, 51, 29, 71}, {82, 95, 96, 00} },  // OP 2
      { {77, 76, 82, 71}, {99, 98, 98, 00} },  // OP 3
      { {77, 36, 41, 71}, {99, 98, 98, 00} },  // OP 4
      { {77, 36, 41, 71}, {99, 98, 98, 00} },  // OP 5
      { {49, 99, 28, 68}, {99, 98, 91, 00} }   // OP 6
   };
   uint8_t level[6] = {98, 86, 99, 99, 98, 82};

   EnvGen env_gen[6];

   for(unsigned i = 0; i < 6; ++i)
   {
      for(unsigned j = 0; j < 4; ++j)
      {
         env_gen[i].setRate6( j, (eg_patch[i].rate[j] * 164) >> 8);
         env_gen[i].setAtten8(j, table_log[eg_patch[i].level[j]] >> 1);
      }

      env_gen[i].keyOn();
   }

   FILE* fp = fopen("brass_1.csv", "w");

   fprintf(fp, "tick,OP-1,OP-2,OP-3,OP-4,OP-5,OP-6\n");

   for(unsigned i = 0; i < 28000; ++i)
   {
      if (i == 24548)
      {
         for(unsigned i = 0; i < 6; ++i)
            env_gen[i].keyOff();
      }

      fprintf(fp, "%6u,", i);
      fprintf(fp, "0x%08x,", env_gen[0].getAtten12());
      fprintf(fp, "0x%08x,", env_gen[1].getAtten12());
      fprintf(fp, "0x%08x,", env_gen[2].getAtten12());
      fprintf(fp, "0x%08x,", env_gen[3].getAtten12());
      fprintf(fp, "0x%08x,", env_gen[4].getAtten12());
      fprintf(fp, "0x%08x\n", env_gen[5].getAtten12());
   }

   fclose(fp);
}

TEST(EnvGen, decay)
{
   static const SysEx::EnvGen eg_patch =
   {
      {99, 40, 99, 99}, // Rate
      {99,  0, 99,  0}  // Level
   };

   EnvGen env_gen;

   for(unsigned i = 0; i < 4; ++i)
   {
      env_gen.setRate6( i, (eg_patch.rate[i] * 164) >> 8);
      env_gen.setAtten8(i, table_log[eg_patch.level[i]] >> 1);
   }

   FILE* fp = fopen("decay.csv", "w");

   fprintf(fp, "sample,atten12,internal,target  ,rate    ,phase\n");

   static const unsigned SAMPLE_RATE = 49096;

   for(unsigned i = 0; i < SAMPLE_RATE * 6; ++i)
   {
      if (i == 4)
      {
         env_gen.keyOn();
      }
      else if (i == SAMPLE_RATE * 5)
      {
         env_gen.keyOff();
      }

      fprintf(fp, "%6u,", i);
      fprintf(fp, "0x%03x,", env_gen.getAtten12());
      fprintf(fp, "0x%06x,", env_gen.dbgInternal());
      fprintf(fp, "0x%06x,", env_gen.dbgTarget());
      fprintf(fp, "0x%06x,", env_gen.dbgRate());
      fprintf(fp, "%u,", env_gen.dbgPhase());
      fprintf(fp, "\n");
   }

   fclose(fp);
}
