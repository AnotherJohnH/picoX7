//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief DX7 firmware simulation - Pitch EG
//
// Credit to AJXS for his excellent disassembly of the DX7 firmware ROM

#pragma once

#include "SysEx.h"

//! DX7 Pitch envelope generator
template <unsigned NUM_VOICES>
class PitchEg
{
   enum Phase : uint8_t
   {
      ATTACK = 0, DECAY1 = 1, DECAY2 = 2, SUSTAIN = 3, RELEASE = 4, END = 5, NUM_PHASE
   };

public:
   PitchEg() = default;

   //! Get current output value
   int16_t getOutput(unsigned voice_index_) const { return output[voice_index_] - 0x4000; }

   //! Configure from SysEx program
   void load(const SysEx::Voice& patch)
   {
      static uint8_t table_rate[100] = 
      {
         0x01, 0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06,
         0x07, 0x07, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B,
         0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0E, 0x0F, 0x10, 0x10, 0x11,
         0x12, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
         0x1B, 0x1C, 0x1E, 0x1F, 0x21, 0x22, 0x24, 0x25, 0x26, 0x27,
         0x29, 0x2A, 0x2C, 0x2E, 0x2F, 0x31, 0x33, 0x35, 0x36, 0x38,
         0x3A, 0x3C, 0x3E, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C,
         0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x62, 0x66, 0x6A, 0x6E,
         0x73, 0x78, 0x7D, 0x82, 0x87, 0x8D, 0x93, 0x99, 0x9F, 0xA5,
         0xAB, 0xB2, 0xB9, 0xC1, 0xCA, 0xD3, 0xE8, 0xF3, 0xFE, 0xFF
      };

      static uint8_t table_level[100] = 
      {
         0x00, 0x0C, 0x18, 0x21, 0x2B, 0x34, 0x3C, 0x43, 0x48, 0x4C,
         0x4F, 0x52, 0x55, 0x57, 0x59, 0x5B, 0x5D, 0x5F, 0x60, 0x61,
         0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
         0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
         0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
         0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
         0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93,
         0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D,
         0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA6, 0xA8, 0xAB, 0xAE,
         0xB1, 0xB5, 0xBA, 0xC1, 0xC9, 0xD2, 0xDC, 0xE7, 0xF3, 0xFF
      };

      for(unsigned i = 0; i < 4; ++i)
      {
         rate[i]  = table_rate[patch.eg_pitch.rate[i]];
         level[i] = table_level[patch.eg_pitch.level[i]];
      }
   }

   //! Start of note
   void keyOn(unsigned voice_index_)
   {
      output[voice_index_] = level[3] << 7;
      phase[voice_index_]  = ATTACK;
   }

   //! Start of end of note
   void keyOff(unsigned voice_index_)
   {
      phase[voice_index_] = RELEASE;
   }

   //! Step the EG (should be called at 375 Hz)
   bool tick()
   {
      toggle = not toggle;
      if (toggle)
         return false;

      for(unsigned v = 0; v < NUM_VOICES; ++v)
      {
         if ((phase[v] != SUSTAIN) && (phase[v] != END))
         {
            unsigned s       = phase[v] > SUSTAIN ? 3 : phase[v];
            unsigned delta   = rate[s];
            signed   target  = level[s] << 7;
            signed   current = output[v];

            if (target == current)
            {
               // Target already reached skip to next phase
               ++phase[v];
            }
            else if (target < current)
            {
               // Target lower than current
               signed next = current - delta;
               if ((next < 0) || (next <= target))
               {
                  next = target;
                  ++phase[v];
               }
               output[v] = next;
            }
            else
            {
               // Target higher than current
               uint16_t next = current + delta;
               if (next >= target)
               {
                  next = target;
                  ++phase[v];
               }
               output[v] = next;
            }
         }
      }

      return true;
   }

private:
   // Configuration
   uint8_t  rate[4];
   uint8_t  level[4];

   // State
   bool     toggle{false};
   uint8_t  phase[NUM_VOICES];

   // Output
   uint16_t output[NUM_VOICES];
};
