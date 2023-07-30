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

#include "SynthBase.h"

#include "SysEx.h"
#include "Voice.h"

#include "Table_dx7_rom_1.h"
#include "Table_dx7_rom_2.h"
#include "Table_dx7_rom_3.h"
#include "Table_dx7_rom_4.h"

template <unsigned N, unsigned AMP_N = N>
class Synth : public SynthBase<N,Voice,AMP_N>
{
public:
   Synth()
   {
      // Load ROM 1 into the internal patch memory
      memcpy(internal_patches, table_dx7_rom_1, sizeof(internal_patches));
   }

private:
   //! Handle a SYSEX byte
   void sysEx(uint8_t byte) override
   {
      if (byte == 0xF0)
      {
         state = 0;
      }
      else if (byte == 0xF7)
      {
      }
      else
      {
         switch(state)
         {
         case 0: // Identification
            state = byte == ID_YAMAHA ? 1 : 10;
            break;

         case 1: // Sub-state and channel
            {
               unsigned sub_status = (byte >> 4) & 0b111;
               unsigned channel    = byte & 0b1111;
               if (sub_status == 0)
                  state = 2;
               else if (sub_status == 1)
                  state = 7;
               (void) channel;
            }
            break;

         case 2: // voice patch or patches
            state = 3;
            index = 0;
            if (byte == TYPE_1_VOICE)
            {
               buffer      = (uint8_t*) &edit_patch;
               buffer_size = sizeof(edit_patch);
            }
            else if (byte == TYPE_32_VOICES)
            {
               buffer      = (uint8_t*) &internal_patches;
               buffer_size = sizeof(internal_patches);
            }
            else
            {
               state = 10;
            }
            break;

         case 3: // byte count MS byte
            size = byte << 7;
            state = 4;
            break;

         case 4: // byte count LS byte
            size |= byte;
            if (size == buffer_size)
            {
               state = 5;
            }
            break;

         case 5: // data
            buffer[index++] = byte;
            if (index == buffer_size)
            {
               state = 6;
            }
            break;

         case 6: // checksum
            // TODO check the checksum
            if (buffer == (const uint8_t*)&edit_patch)
            {
               for(unsigned i = 0; i < N; ++i)
                  this->voice[i].loadProgram(0, &edit_patch);
            }
            break;

         case 7:
            group = byte >> 2;
            param = (byte & 0b11) << 7;
            state = 8;
            break;

         case 8:
            param |= byte;
            state = 9;
            break;

         case 9:
            if (group == 0)
            {
               buffer = (uint8_t*) &edit_patch;
               buffer[param] = byte;
               for(unsigned i = 0; i < N; ++i)
                  this->voice[i].loadProgram(0, &edit_patch);
            }
            else if (group == 2)
            {
               // TODO function parameters
            }
            break;

         case 10: // Ignore
            break;
         }
      }
   }

   void voiceProgram(unsigned index_, uint8_t number_) override
   {
      const SysEx::Packed* memory;

      switch(number_ >> 5)
      {
      case 0:  memory = internal_patches;                       break;
      case 1:  memory = (const SysEx::Packed*) table_dx7_rom_2; break;

      // DX7 did not support selecting programs above 63
      case 2:  memory = (const SysEx::Packed*) table_dx7_rom_3; break;
      case 3:  memory = (const SysEx::Packed*) table_dx7_rom_4; break;

      default:
         return;
      }

      edit_patch = memory[number_ & 0x1F];

      this->voice[index_].loadProgram(number_, &edit_patch);
   }

   const uint8_t ID_YAMAHA      = 67;
   const uint8_t TYPE_1_VOICE   = 0;
   const uint8_t TYPE_32_VOICES = 9;

   SysEx::Voice  edit_patch;
   SysEx::Packed internal_patches[32];

   // SYSEX state machine state
   uint8_t  state{0};
   uint8_t* buffer {};
   size_t   buffer_size {};
   size_t   size{};
   unsigned index{};
   uint8_t  group{};
   unsigned param{};
};
