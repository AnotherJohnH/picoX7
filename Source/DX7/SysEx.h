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

#include <cstdint>
#include <cstring>

namespace SysEx {

static const unsigned NUM_OP   = 6;
static const unsigned NAME_LEN = 10;

enum ScaleCurve : uint8_t { NEG_LIN = 0, NEG_EXP = 1, POS_EXP = 2, POS_LIN = 3 };
enum OscMode    : uint8_t { RATIO = 0, FIXED = 1 };

enum LfoWave : uint8_t
{
   TRIANGLE        = 0,
   SAW_DOWN        = 1,
   SAW_UP          = 2,
   SQUARE          = 3,
   SINE            = 4,
   SAMPLE_AND_HOLD = 5
};

//! Envelope generator parameters
struct EnvGen
{
   uint8_t rate[4]  = {99, 99, 99, 99};  // 0..99
   uint8_t level[4] = {99, 99, 99,  0};  // 0..99
};

//! Packed voice parameters
struct Packed
{
   struct Op
   {
      EnvGen   eg_amp{};
      uint8_t  kbd_lvl_scl_bpt;                 // 0..99  C3 = 39
      uint8_t  kbd_lvl_scl_lft_depth;           // 0..99
      uint8_t  kbd_lvl_scl_rgt_depth;           // 0..99

      union
      {
         struct
         {
            ScaleCurve kbd_lvl_scl_lft_curve : 2; // 0..3   -LIN, -EXP, +EXP, +LIN
            ScaleCurve kbd_lvl_scl_rgt_curve : 2; // 0..3   -LIN, -EXP, +EXP, +LIN
            uint8_t                          : 4;
         };

         uint8_t kbd_bits;
      };

      union
      {
         struct
         {
            uint8_t    kbd_rate_scale        : 3; //
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

   Op      op[NUM_OP];
   EnvGen  eg_pitch;
   uint8_t alg;                                  // 0..31

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
         uint8_t lfo_sync        : 1;        // 0..1
         LfoWave lfo_waveform    : 3;        // 0..5   TR, SD, SU, SQ, SI, SH
         uint8_t pitch_mod_sense : 3;        // 0..7
         uint8_t                 : 1;
      };

      uint8_t lfo_bits;
   };

   uint8_t transpose;                            // 0..48  12 = C2
   char    name[NAME_LEN];                       // ASCII
};

//! Operator parameters
struct Op
{
   Op() = default;

   void print(unsigned n) const;

   void operator=(const Packed::Op& packed)
   {
      eg_amp                = packed.eg_amp;
      kbd_lvl_scl_bpt       = packed.kbd_lvl_scl_bpt;
      kbd_lvl_scl_lft_depth = packed.kbd_lvl_scl_lft_depth;
      kbd_lvl_scl_rgt_depth = packed.kbd_lvl_scl_rgt_depth;
      kbd_lvl_scl_lft_curve = packed.kbd_lvl_scl_lft_curve;
      kbd_lvl_scl_rgt_curve = packed.kbd_lvl_scl_rgt_curve;
      kbd_rate_scale        = packed.kbd_rate_scale;
      amp_mod_sense         = packed.amp_mod_sense;
      key_vel_sense         = packed.key_vel_sense;
      out_level             = packed.out_level;
      osc_mode              = packed.osc_mode;
      osc_freq_coarse       = packed.osc_freq_coarse;
      osc_freq_fine         = packed.osc_freq_fine;
      osc_detune            = packed.osc_detune;
   }

   EnvGen     eg_amp {};
   uint8_t    kbd_lvl_scl_bpt {};        // 0-99  C3 = 39
   uint8_t    kbd_lvl_scl_lft_depth {};  // 0-99
   uint8_t    kbd_lvl_scl_rgt_depth {};  // 0-99
   ScaleCurve kbd_lvl_scl_lft_curve {};  // 0-3
   ScaleCurve kbd_lvl_scl_rgt_curve {};  // 0-3
   uint8_t    kbd_rate_scale {0};        // 0-7
   uint8_t    amp_mod_sense {0};         // 0-3
   uint8_t    key_vel_sense {0};         // 0-7
   uint8_t    out_level {99};            // 0-99
   OscMode    osc_mode {RATIO};          // 0-1
   uint8_t    osc_freq_coarse {1};       // 0-31
   uint8_t    osc_freq_fine {0};         // 0-99
   uint8_t    osc_detune {7};            // 0-14   0 is -7
};

//! LFO parameters
struct Lfo
{
   Lfo() = default;

   uint8_t speed {0};           // 0-99
   uint8_t delay {0};           // 0-99
   uint8_t pitch_mod_depth {0}; // 0-99
   uint8_t amp_mod_depth {0};   // 0-99
   uint8_t sync {1};            // 0-1
   LfoWave waveform {TRIANGLE}; // 0-5
};

//! Voice parameters
struct Voice
{
   Voice() = default;

   //! Construct from a patch in a packed table
   Voice(const uint8_t* packed_patch_table, unsigned index = 0)
   {
      *this = (const SysEx::Packed&)packed_patch_table[index * sizeof(SysEx::Packed)];
   }

   void print(unsigned ) const;

   //! Copy in a packed SysEx voice
   void operator=(const Packed& packed)
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         op[i] = packed.op[i];
      }

      eg_pitch            = packed.eg_pitch;
      alg                 = packed.alg;
      feedback            = packed.feedback;
      osc_sync            = packed.osc_sync;
      lfo.speed           = packed.lfo_speed;
      lfo.delay           = packed.lfo_delay;
      lfo.pitch_mod_depth = packed.lfo_pitch_mod_depth;
      lfo.amp_mod_depth   = packed.lfo_amp_mod_depth;
      lfo.sync            = packed.lfo_sync;
      lfo.waveform        = packed.lfo_waveform;
      pitch_mod_sense     = packed.pitch_mod_sense;
      transpose           = packed.transpose;

      memcpy(name, packed.name, NAME_LEN);
   }

   Op      op[NUM_OP];
   EnvGen  eg_pitch {};
   uint8_t alg {0};             // 0-31
   uint8_t feedback {0};        // 0-7
   uint8_t osc_sync {1};        // 0-1
   Lfo     lfo {};
   uint8_t pitch_mod_sense {0}; // 0-7
   uint8_t transpose {0};       // 0-48     12 = C2
   uint8_t name[NAME_LEN];
};

} // namespace SysEx
