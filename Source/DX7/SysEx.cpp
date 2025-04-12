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

#include <cstdio>
#include <cstdarg>

#include "SysEx.h"

using namespace SysEx;

// Some helpers for pretty printing
static const char* FG_GREEN   = "\e[32m";
static const char* FG_DEFAULT = "\e[39m";
static void fgGreen()   { printf("%s", FG_GREEN); }
static void fgDefault() { printf("%s", FG_DEFAULT); }

static void fmtf(const char* fix, const char* format = "", ...)
{
   printf("%s%s%s ", FG_GREEN, fix, FG_DEFAULT);

   va_list ap;
   va_start(ap, format);
   vprintf(format, ap);
   va_end(ap);
}

void Op::print(unsigned n) const
{
   fgGreen();
   printf("%u  ", n);
   fgDefault();

   printf("%2d  ", amp_mod_sense);

   if (osc_mode == RATIO)
   {
      if (osc_freq_coarse == 0)
      {
         printf("R  0.%02u", 50 + (osc_freq_fine / 2));
      }
      else
      {
         printf("R %2u.%02u", osc_freq_coarse, osc_freq_fine);
      }
   }
   else
   {
      printf("F %2u.%02u", osc_freq_coarse, osc_freq_fine);
   }

   printf(" %2d  ", osc_detune - 7);

   for(unsigned i = 0; i < 4; i++)
   {
      printf("%02u ", eg_amp.rate[i]);
   }

   printf(" ");

   for(unsigned i = 0; i < 4; i++)
   {
      printf("%02u ", eg_amp.level[i]);
   }

   static const char* curve_lbl[4] = {"-lin", "-exp", "+exp", "+lin"};

   printf(" %2u  %s %s  %02u %02u  %u ",
          kbd_lvl_scl_bpt,
          curve_lbl[kbd_lvl_scl_lft_curve], curve_lbl[kbd_lvl_scl_rgt_curve],
          kbd_lvl_scl_lft_depth, kbd_lvl_scl_rgt_depth,
          kbd_rate_scale);

   printf("  %02u", out_level);
   printf(" %2d", key_vel_sense);

   printf("\n");
}

void Voice::print(unsigned n) const
{
   static const char* bool_lbl[2] = {"off", "on "};

   fmtf("#", "%03u", n);
   fmtf(" NAME");
   for(unsigned i = 0; i < NAME_LEN; i++)
      putchar(name[i]);
   fmtf(" ALG", "%u", alg + 1);
   fmtf(" FBK", "%u", feedback);
   fmtf(" SYNC", "%s", bool_lbl[osc_sync]);
   fmtf(" PMS", "%u", pitch_mod_sense);
   fmtf(" TRN", "%d\n", transpose - 24);

   static const char* wave_lbl[6] = {"tri", "swd", "swu", "sqr", "sin" ,"s&h"};

   fmtf("LFO");
   fmtf("WAV", "%s", wave_lbl[lfo.waveform]);
   fmtf(" SPD", "%2u", lfo.speed);
   fmtf(" DLY", "%02u", lfo.delay);
   fmtf(" PMD", "%02u", lfo.pitch_mod_depth);
   fmtf(" AMD", "%02u", lfo.amp_mod_depth);
   fmtf(" SYNC", "%s", bool_lbl[lfo.sync]);
   printf("\n");

   fmtf("OP AMS FREQ    DET R1 R2 R3 R4  L1 L2 L3 L4  BPT   CURVE    DEPTH RTE OUT KVS", "\n");

   for(unsigned i = 0; i < NUM_OP; ++i)
   {
      op[i].print(NUM_OP - i);
   }

   fmtf("PITCH EG          ");
   for(unsigned i = 0; i < 4; i++)
   {
       printf("%02u ", eg_pitch.rate[i]);
   }
   printf(" ");
   for(unsigned i = 0; i < 4; i++)
   {
       printf("%02u ", eg_pitch.level[i]);
   }
   printf("\n");
   printf("\n");
}

void Param::print() const
{
   fmtf(" MOD", "%2u", mod_wheel_range);
   fmtf(" P", "%u", mod_wheel_assign & 1);
   fmtf(" A", "%u", (mod_wheel_assign >> 1) & 1);
   fmtf(" E", "%u", (mod_wheel_assign >> 2) & 1);

   fmtf("  FOOT", "%2u", foot_control_range);
   fmtf(" P", "%u", foot_control_assign & 1);
   fmtf(" A", "%u", (foot_control_assign >> 1) & 1);
   fmtf(" E", "%u", (foot_control_assign >> 2) & 1);

   fmtf("  BREATH", "%2u", breath_control_range);
   fmtf(" P", "%u", breath_control_assign & 1);
   fmtf(" A", "%u", (breath_control_assign >> 1) & 1);
   fmtf(" E", "%u", (breath_control_assign >> 2) & 1);

   fmtf("  AFTER", "%2u", after_touch_range);
   fmtf(" P", "%u", after_touch_assign & 1);
   fmtf(" A", "%u", (after_touch_assign >> 1) & 1);
   fmtf(" E", "%u", (after_touch_assign >> 2) & 1);
   printf("\n");
}
