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

#include <cstdio>

struct SysEx
{
   void print() const
   {
      printf("\"");
      for(unsigned i = 0; i < NAME_LEN; i++)
         putchar(name[i]);
      printf("\" ALG %u FBK %u\n", alg + 1, feedback);

      printf("    L1:R1 L2:R2 L3:R3 L4:R4 OUT FREQ\n");
      printf("------------------------------------------------\n");

      for(unsigned i = 0; i < 6; ++i)
      {
         op[5 - i].print(i + 1);
      }
   }

   static const unsigned NUM_OP   = 6;
   static const unsigned NAME_LEN = 10;

   struct Envelope
   {
      uint8_t rate[4]  = {99, 99, 99, 99};        // 0..99
      uint8_t level[4] = {99, 99, 99,  0};        // 0..99
   };

   struct Op
   {
      enum ScaleCurve : uint8_t { NEG_LIN = 0, NEG_EXP = 1, POS_EXP = 2, POS_LIN = 3 };
      enum OscMode    : uint8_t { RATIO = 0, FIXED = 1 };

      void print(unsigned n) const
      {
         printf("OP%u ", n);

         for(unsigned i = 0; i < 4; i++)
         {
            printf("%02u:%02u ", eg_amp.level[i], eg_amp.rate[i]);
         }

         printf("%02u ", out_level);

         if (osc_mode == RATIO)
         {
            if (osc_freq_coarse == 0)
            {
               printf("R00.%02u", 50 + (osc_freq_fine / 2));
            }
            else
            {
               printf("R%02u.%02u", osc_freq_coarse, osc_freq_fine);
            }
         }
         else
         {
            printf("F%02u.%02u", osc_freq_coarse, osc_freq_fine);
         }

         printf(" %2d", osc_detune - 7);
         printf("\n");
      }

      Envelope eg_amp{};
      uint8_t  kbd_lev_scl_bpt;                 // 0..99  C3 = 39
      uint8_t  kbd_lev_scl_lft_depth;           // 0..99
      uint8_t  kbd_lev_scl_rgt_depth;           // 0..99

      union
      {
         struct
         {
            ScaleCurve kbd_scale_left_curve  : 2; // 0..3   -LIN, -EXP, +EXP, +LIN
            ScaleCurve kbd_scale_right_curve : 2; // 0..3   -LIN, -EXP, +EXP, +LIN
            uint8_t                          : 4;
         };

         uint8_t kbd_bits;
      };

      union
      {
         struct
         {
            uint8_t    rate_scale            : 3; //
            uint8_t    osc_detune            : 4; // 0..14 -7..+7
            uint8_t                          : 1;
         };

         uint8_t rate_bits;
      };

      union
      {
         struct
         {
            uint8_t    amp_mod_sense         : 2; // 0..3
            uint8_t    key_vel_sense         : 3; // 0..3
            uint8_t                          : 3;
         };

         uint8_t sense_bits;
      };

      uint8_t out_level;                          // 0..99

      union
      {
         struct
         {
            OscMode osc_mode              : 1;    // 0..1   Ratio, Fixed
            uint8_t osc_freq_coarse       : 5;    // 0..31
            uint8_t                       : 2;
         };
         uint8_t osc_bits;
      };

      uint8_t    osc_freq_fine;                   // 0..99
   };

   Op       op[NUM_OP];
   Envelope eg_pitch;
   uint8_t  alg;                                  // 0..31

   union
   {
      struct
      {
         uint8_t feedback : 3;
         uint8_t osc_sync : 1;
         uint8_t          : 4;
      };

      uint8_t alg_bits;
   };

   uint8_t lfo_speed;                            // 0..99
   uint8_t lfo_delay;                            // 0..99
   uint8_t lfo_pitch_mod_depth;                  // 0..99
   uint8_t lfo_amp_mod_depth;                    // 0..99

   union
   {
      struct
      {
         uint8_t lfo_sync            : 1;        // 0..1
         uint8_t lfo_wave            : 3;        // 0..5   TR, SD, SU, SQ, SI, SH
         uint8_t lfo_pitch_mod_sense : 3;        // 0..7
         uint8_t                     : 1;
      };

      uint8_t lfo_bits;
   };

   uint8_t transpose;                            // 0..48  12 = C2
   char    name[NAME_LEN];                       // ASCII
};
