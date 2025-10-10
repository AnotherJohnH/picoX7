//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief DX7 firmware simulation - LFO
//
// Credit to AJXS for his excellent disassembly of the DX7 firmware ROM

#pragma once

#include "SysEx.h"

//! DX7 LFO simulation
class Lfo
{
public:
   Lfo() = default;

   //! Get amplitude modulatiion factor (0..FF)
   uint8_t getAmpMod() const { return amp_mod; }

   //! Get pitch modulatiion factor (0..FF)
   uint8_t getPitchMod() const { return pitch_mod; }

   //! Get current amplitude output value (2's comp 8-bit)
   int8_t getAmpOutput() const { return output; }

   //! Get current pitch output value (2's comp 8-bit)
   int8_t getPitchOutput() const { return (output * pitch_mod_sense) >> 8; }

   //! Configure from SysEx program
   void load(const SysEx::Voice& patch)
   {
      // Compute LFO phase increment from LFO speed
      // Scale 0..99 to full scale 8-bit e.g. 0..255
      uint8_t  speed = (patch.lfo.speed * 660) >> 8;
      unsigned scale = 11;

      if (speed == 0)
      {
         speed = 1;                     // LFO speed 0 => 11
      }
      else if (speed >= 160)
      {
         scale += (speed - 160) >> 2;   //      LFO speed 63..99 => 1782..8670
      }                                 // else LFO speed  1..62 =>   11..1749

      phase_inc = scale * speed;


      // Compute LFO delay increment from LFO delay
      // Invert 0..99 => 99..0
      // and treat as an 7-bit floating point value _EEEMMMM
      uint8_t  delay   = 99 - patch.lfo.delay;
      uint8_t  exp     = 7 - (delay >> 4);
      uint16_t mantisa = (0b10000 | (delay & 0b1111)) << 9;
      delay_inc        = mantisa >> exp;


      // Scale 0..99 to full scale 8-bit e.g. 0..255
      pitch_mod_depth = (patch.lfo.pitch_mod_depth * 660) >> 8;

      // Scale 0..99 to full scale 8-bit e.g. 0..255
      amp_mod_depth = (patch.lfo.amp_mod_depth * 660) >> 8;

      waveform = patch.lfo.waveform;

      // Scale 0..7 to full scale 8-bit e.g. 0..255
      const uint8_t pitch_mod_sense_table[8] = {0, 10, 20, 33, 55, 92, 153, 255};
      pitch_mod_sense = pitch_mod_sense_table[patch.pitch_mod_sense];

      sync = patch.lfo.sync;
   }

   //! Start of note
   void keyOn()
   {
      delay_accum = 0;
      fade_in     = 0;

      if (sync)
      {
         phase_accum = MAX_PHASE;
      }
   }

   //! Step the LFO and return current output (should be called at 375 Hz)
   void tick()
   {
      // Evaluate delay and fade-in
      unsigned next_delay_value = delay_accum + delay_inc;
      if (next_delay_value < 0xFFFF)
      {
         // Still delayed
         delay_accum = next_delay_value;
      }
      else
      {
         // Delay complete
         delay_accum = 0xFFFF;

         // Compute fade-in => Use MSB of delay increment for fade-in increment
         unsigned fade_in_inc  = delay_inc > 0xFF ? delay_inc >> 8 : 1;
         unsigned next_fade_in = fade_in + fade_in_inc;
         fade_in               = next_fade_in < 0xFF ? next_fade_in : 0xFF;
      }

      // Increment LFO phase
      phase_accum += phase_inc;

      switch(waveform)
      {
      case SysEx::TRIANGLE:
         {
            output = phase_accum >> 7;
            if (phase_accum < 0)
            {
               output = ~output;
            }
            output += 0x80;
         }
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

            unsigned index = (phase_accum >> 8) & 0x3F;
            if ((phase_accum & 0x4000) != 0)
            {
               index ^= 0x3F;
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
            sample_hold_accum = sample_hold_accum * 179 + 11;
         }
         output = sample_hold_accum;
         break;
      }

      amp_mod   = (fade_in * amp_mod_depth) >> 8;
      pitch_mod = (fade_in * pitch_mod_depth) >> 8;
   }

private:
   using Phase = int16_t;

   static const Phase MAX_PHASE = 0x7FFF;
   static const Phase MIN_PHASE = -0x8000;

   // Configuration from patch
   uint16_t        phase_inc{0};              //!< DX7 var @ 0x2320
   uint16_t        delay_inc{0};              //!< DX7 var @ 0x2322
   SysEx::LfoWave  waveform{SysEx::TRIANGLE}; //!< DX7 var @ 0x2324
   uint8_t         amp_mod_depth{0};          //!< DX7 var @ 0x2326
   uint8_t         pitch_mod_depth{0};        //!< DX7 var @ 0x2325
   uint8_t         pitch_mod_sense{0};        //!< DX7 var @ 0x2327
   bool            sync {false};

   // State
   Phase    phase_accum{MAX_PHASE};   //!< DX7 var @ 0xD7:D8
   uint8_t  sample_hold_accum{0};     //!< DX7 var @ 0xDA
   uint8_t  fade_in{0};               //!< DX7 var @ 0xDB
   uint16_t delay_accum{0};           //!< DX7 var @ 0xDD:DE

   // Outputs
   int8_t   output{0};                //!< DX7 var @ 0xD9
   uint8_t  amp_mod{};
   uint8_t  pitch_mod{};
};
