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

//! DX7 LFO simulation
class Lfo
{
public:
   Lfo() = default;

   //! Configure from SysEx program
   void prog(const SysEx::Voice& patch)
   {
      // Compute LFO delay increment from LFO delay
      // Invert 0..99 => 99..0
      uint8_t  value   = 99 - patch.lfo.delay;

      // Treat as an 7-bit floating point value _EEEMMMM
      uint8_t  exp     = 7 - (value >> 4);
      uint16_t mantisa = (0b10000 | (value & 0b1111)) << 9;

      delay_inc = mantisa >> exp;

      // Compute LFO phase increment from LFO speed
      if (patch.lfo.speed == 0)
      {
         // LFO speed 0 => 11
         phase_inc = 11;
      }
      else
      {
         // Scale 0..99 to full scale 8-bit e.g. 0..255
         uint8_t scaled_speed = (patch.lfo.speed * 660) >> 8;

         if (scaled_speed < 160)
         {
            // LFO speed 1..62 => 22..1749
            phase_inc = 11 * scaled_speed;
         }
         else
         {
            // LFO speed 63..99 => 1782..8670
            phase_inc = (11 + (scaled_speed - 160) / 4) * scaled_speed;
         }
      }

      sync = patch.lfo.sync;

      wave = patch.lfo.waveform;

      // Scale 0..99 to full scale 8-bit e.g. 0..255
      amp_mod_depth = (patch.lfo.amp_mod_depth * 660) >> 8;
         
      // Scale 0..99 to full scale 8-bit e.g. 0..255
      pitch_mod_depth = (patch.lfo.pitch_mod_depth * 660) >> 8;
                                                          
      // Scale 0..7 to full scale 8-bit e.g. 0..255
      const uint8_t pitch_mod_sense_table[8] = {0, 10, 20, 33, 55, 92, 153, 255};
      pitch_mod_sense = pitch_mod_sense_table[patch.pitch_mod_sense];
   }

   //! Start of note
   void keyOn()
   {
      delay_counter        = 0;
      fade_in_scale_factor = 0;

      if (sync)
      {
         phase_accum = MAX_PHASE;
      }
   }

   //! Step the LFO and return current output (should be called at 375 Hz)
   void tick()
   {
      static const uint8_t sine_table[64] =
      {
         0x02, 0x05, 0x08, 0x0B, 0x0E, 0x11, 0x14, 0x17,
         0x1A, 0x1D, 0x20, 0x23, 0x26, 0x29, 0x2C, 0x2F,
         0x32, 0x35, 0x38, 0x3A, 0x3D, 0x40, 0x43, 0x45,
         0x48, 0x4A, 0x4D, 0x4F, 0x52, 0x54, 0x56, 0x59,
         0x5B, 0x5D, 0x5F, 0x61, 0x63, 0x65, 0x67, 0x69,
         0x6A, 0x6C, 0x6E, 0x6F, 0x71, 0x72, 0x73, 0x75,
         0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7C,
         0x7D, 0x7D, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F
      };

      if (delay_counter < 0xFFFF)
      {
         // Initial delay
         delay_counter += delay_inc;

         amp_mod   = 0;
         pitch_mod = 0;
      }
      else if (fade_in_scale_factor < 0xFF)
      {
         // Fading the LFO modulation in using LSB of delay increment

         uint8_t fade_in_scale_inc = delay_inc & 0xFF;

         fade_in_scale_factor += (fade_in_scale_inc == 0) ? 1
                                                          : fade_in_scale_inc;

         // Clamp scale factor at 0xFF
         if (fade_in_scale_factor > 0xFF)
         {
            fade_in_scale_factor = 0xFF;
         }

         amp_mod   = (fade_in_scale_factor * amp_mod_depth)   >> 8;
         pitch_mod = (fade_in_scale_factor * pitch_mod_depth) >> 8;
      }
      else
      {
         phase_accum += phase_inc;
      }

      switch(wave)
      {
      case SysEx::TRIANGLE:
         output = phase_accum >> 7;
         if (phase_accum < 0)
         {
            output = ~output;
         }
         output += 128; 
         break; 
   
      case SysEx::SAW_DOWN:
         output = ~(phase_accum >> 8);
         break; 
   
      case SysEx::SAW_UP:
         output = phase_accum >> 8;
         break;
   
      case SysEx::SQUARE:
         output = phase_accum < 0 ? 127 : -128;
         break;
   
      case SysEx::SINE:
         {
            uint8_t index = (phase_accum >> 8) % 64;
            if (phase_accum & 0x4000)
            {
               index ^= 63;
            }
            output = sine_table[index];
            if (phase_accum < 0)
            {
               output = ~output;
            }
         }
         break;

      case SysEx::SAMPLE_AND_HOLD:
         if ((phase_accum - MIN_PHASE) < phase_inc)
         {
            rand_state = rand_state * 179 + 11;
         }
         output = rand_state;
         break;
      }
   }

   uint8_t getAmpMod() const   { return amp_mod; }

   uint8_t getPitchMod() const { return pitch_mod; }

   int8_t  getOutput() const   { return output; }

private:
   using Phase = int16_t;

   static const Phase MAX_PHASE = 0x7FFF;
   static const Phase MIN_PHASE = -0x8000;

   // Configuration
   uint16_t        delay_inc {0};
   uint16_t        phase_inc {0};
   uint8_t         pitch_mod_depth {0};
   uint8_t         amp_mod_depth {0};
   bool            sync {false};
   SysEx::LfoWave  wave {SysEx::TRIANGLE};
   uint8_t         pitch_mod_sense {0};

   // State
   uint32_t delay_counter {0};
   uint16_t fade_in_scale_factor {0};
   Phase    phase_accum {MAX_PHASE};
   uint8_t  rand_state {0};
   uint8_t  amp_mod{0};
   uint8_t  pitch_mod{0};
   int8_t   output{0};
};
