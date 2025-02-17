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

#include <cstring>
#include <unistd.h>

#include "MTL/MTL.h"

#include "SynthBase.h"

#include "SysEx.h"
#include "Voice.h"

#include "Table_dx7_rom_1.h"
#include "Table_dx7_rom_2.h"
#include "Table_dx7_rom_3.h"
#include "Table_dx7_rom_4.h"

namespace DX7 {

template <unsigned N, unsigned AMP_N = N>
class Synth : public SynthBase<N,Voice,AMP_N>
{
public:
   Synth()
   {
      // Load ROM 1 into the internal patch memory
      memcpy(internal_patches, table_dx7_rom_1, sizeof(internal_patches));
   }

   void start()
   {
      this->displayLED(188);
      this->displayLCD(0, "*    picoX7    *");
      this->displayLCD(1, "*  SYNTHESIZER *");

      // 2.5s
      usleep(2500000);
   }

private:
   enum State : uint8_t
   {
      STATE_IGNORE,

      STATE_START,
      STATE_START_SUB,

      STATE_PATCH,
      STATE_PATCH_SIZE_MS,
      STATE_PATCH_SIZE_LS,

      STATE_PATCH_EDIT_DATA,
      STATE_PATCH_EDIT_CSUM,

      STATE_PATCH_INT_DATA,
      STATE_PATCH_INT_CSUM,

      STATE_PARAM,

      STATE_PARAM_VOICE,
      STATE_PARAM_VOICE_VALUE,

      STATE_PARAM_FUNC,
      STATE_PARAM_FUNC_VALUE
   };

   //! Handle a SYSEX byte
   void sysEx(uint8_t byte) override
   {
      if (byte == 0xF0)
      {
         state = STATE_START;
         return;
      }
      else if (byte == 0xF7)
      {
         state = STATE_IGNORE;
         return;
      }

      switch(state)
      {
      case STATE_IGNORE:
         break;

      case STATE_START:
         state = byte == ID_YAMAHA ? STATE_START_SUB
                                   : STATE_IGNORE;
         break;

      case STATE_START_SUB:
         {
            unsigned sub_status = (byte >> 4) & 0b111;
            unsigned channel    = byte & 0b1111;
            if (channel == 0)
            {
               if (sub_status == SUB_STATUS_PATCH)
                  state = STATE_PATCH;
               else if (sub_status == SUB_STATUS_PARAM)
                  state = STATE_PARAM;
            }
            else
               state = STATE_IGNORE;
         }
         break;


      case STATE_PATCH:
         if (byte == PATCH_FORMAT_1_VOICE)
         {
            index = 1;
            state = STATE_PATCH_SIZE_MS;
         }
         else if (byte == PATCH_FORMAT_32_VOICES)
         {
            index = 32;
            state = STATE_PATCH_SIZE_MS;
         }
         else
            state = STATE_IGNORE;
         break;

      case STATE_PATCH_SIZE_MS:
         size  = byte << 7;
         state = STATE_PATCH_SIZE_LS;
         break;

      case STATE_PATCH_SIZE_LS:
         size |= byte;
         if ((index == 1) && (size == SYSEX_EDIT_PATCH_SIZE))
         {
            index = 0;
            state = STATE_PATCH_EDIT_DATA;
         }
         else if ((index == 32) && (size == SYSEX_32_PATCH_SIZE))
         {
            index = 0;
            state = STATE_PATCH_INT_DATA;
         }
         else
            state = STATE_IGNORE;
         break;


      case STATE_PATCH_EDIT_DATA:
         {
            auto buffer = (uint8_t*) &edit_patch;
            buffer[index++] = byte;
            if (index == size)
               state = STATE_PATCH_EDIT_CSUM;

            // operator on/off byte is not in the patch se default to all on
            edit_patch.operator_on = 0b111111;
         }
         break;

      case STATE_PATCH_EDIT_CSUM:
         // TODO check the checksum
         for(unsigned i = 0; i < N; ++i)
            this->voice[i].loadProgram(0, &edit_patch);
         state = STATE_IGNORE;
         break;


      case STATE_PATCH_INT_DATA:
         {
            auto buffer = (uint8_t*) &internal_patches;
            buffer[index++] = byte;
            if (index == size)
               state = STATE_PATCH_INT_CSUM;
         }
         break;

      case STATE_PATCH_INT_CSUM:
         // TODO check the checksum
         state = STATE_IGNORE;
         break;


      case STATE_PARAM:
         {
            unsigned group = byte >> 2;
            if (group == PARAM_GROUP_VOICE)
            {
               index = (byte & 0b11) << 7;
               state = STATE_PARAM_VOICE;
            }
            else if (group == PARAM_GROUP_FUNC)
            {
               state = STATE_PARAM_FUNC;
            }
         }
         break;


      case STATE_PARAM_VOICE:
         index |= byte;
         state = STATE_PARAM_VOICE_VALUE;
         break;

      case STATE_PARAM_VOICE_VALUE:
         {
            auto buffer = (uint8_t*) &edit_patch;
            buffer[index] = byte;

            for(unsigned i = 0; i < N; ++i)
               this->voice[i].loadProgram(0, &edit_patch, /* update */ true);
         }
         state = STATE_IGNORE;
         break;


      case STATE_PARAM_FUNC:
         index = byte;
         state = STATE_PARAM_FUNC_VALUE;
         break;

      case STATE_PARAM_FUNC_VALUE:
         // TODO function parameters
         state = STATE_IGNORE;
         break;
      }
   }

   void voiceProgram(unsigned index_, uint8_t number_) override
   {
      const SysEx::Packed* memory;

      switch(number_ >> 5)
      {
      case 0: memory = internal_patches;                       break;
      case 1: memory = (const SysEx::Packed*) table_dx7_rom_2; break;

      // DX7 did not support selecting programs above 63
      case 2: memory = (const SysEx::Packed*) table_dx7_rom_3; break;
      case 3: memory = (const SysEx::Packed*) table_dx7_rom_4; break;

      default:
         return;
      }

      edit_patch = memory[number_ & 0x1F];

      if (index_ == 0)
      {
         this->displayLED(number_ + 1);

         char line1[17];

         if ((number_ >> 5) == 0)
         {
            this->displayLCD(0, "INTERNAL VOICE  ");
            strcpy(line1, "INT");
         }
         else
         {
            this->displayLCD(0, "CARTRIDGE VOICE ");
            strcpy(line1, "CRT");
         }

#if defined(TARGET_NATIVE)
         sprintf(line1 + 3, "%2u ", (number_ % 32) + 1);
#else
         snprintf(line1 + 3, 13, "%2u ", (number_ % 32) + 1);
#endif
         strncpy(line1 + 6, (const char*)edit_patch.name, 10);
         line1[16] = '\0';

         this->displayLCD(1, line1);
      }

      this->voice[index_].loadProgram(number_, &edit_patch);
   }

   const uint8_t ID_YAMAHA              = 67;
   const uint8_t SUB_STATUS_PATCH       = 0;
   const uint8_t SUB_STATUS_PARAM       = 1;
   const uint8_t PATCH_FORMAT_1_VOICE   = 0;
   const uint8_t PATCH_FORMAT_32_VOICES = 9;
   const uint8_t PARAM_GROUP_VOICE      = 0;
   const uint8_t PARAM_GROUP_FUNC       = 2;

   // sizeof(SysEx::Voice) - 1 as the operator on/off byte is missing from the SYSEX message
   const size_t SYSEX_EDIT_PATCH_SIZE = sizeof(SysEx::Voice) - 1;
   const size_t SYSEX_32_PATCH_SIZE   = sizeof(SysEx::Packed) * 32;

   SysEx::Voice  edit_patch;
   SysEx::Packed internal_patches[32];

   // SYSEX state machine state
   State  state{STATE_IGNORE};
   size_t size{};
   size_t index{};
};

} // namespace DX7
