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

template <unsigned N>
class Synth : public SynthBase<N,Voice>
{
public:
   Synth() = default;

   void sysEx(uint8_t byte)
   {
      if (byte == 0xF0)
      {
         printf("SYSEX START %u state", state);
         state = 0;
      }
      else if (byte == 0xF7)
      {
         printf("\nSYSEX END %u state\n", state);

         if ((id      == ID_YAMAHA) &&
             (type    == TYPE_VOICE) &&
             (channel == 0) &&
             (size    == 0x80) &&
             (size    == index))
         {
            for(unsigned i = 0; i < N; ++i)
            {
               this->voice[i].setProgramRaw(buffer);
            }

            printf("OK\n");
         }
         else
         {
            printf("BAD\n");
         }
      }
      else
      {
         switch(state)
         {
         case 0: id      = byte;                   ++state; break;
         case 1: type    = byte;                   ++state; break;
         case 2: channel = byte;                   ++state; break;
         case 3: size    = byte << 7;              ++state; break;
         case 4: size    = size | byte; index = 0; ++state; break;

         case 5:
            if (index < sizeof(buffer))
            {
               if ((index % 8) == 0)
                  printf("\n%02X : ", index);

               printf(" %02X", byte);
               buffer[index++] = byte;
            }
            break;
         }
      }
   }

private:
   const uint8_t ID_YAMAHA  = 0x43;
   const uint8_t TYPE_VOICE = 0x00;

   uint8_t  state{0};
   uint8_t  id{};
   uint8_t  type{};
   uint8_t  channel{};
   uint16_t size{};
   uint8_t  index{0};
   uint8_t  buffer[128];
};
