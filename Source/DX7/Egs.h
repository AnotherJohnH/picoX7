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

#include "OpsAlg.h"
#include "EnvGen.h"

#if defined(HW_NATIVE)
#define REG_SIM
#endif

//! Model of Yamaha EGS (like the YM21290)
class Egs : public OpsAlg
{
public:
   Egs()
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         egs[i] = getEgsPointer(i);
      }
   }

   // EGS inputs

   //! Set voice pitch [0x3000..301F]
   void setEgsVoicePitch(uint16_t pitch_)
   {
      voice_pitch = pitch_;

#if defined(REG_SIM)
      pitch_ <<= 2;
      reg[0] = pitch_ >> 8;
      reg[1] = pitch_ & 0xFF;
#endif
   }

   //! Set operator pitch value [0x3020..302F]
   void setEgsOpPitch(unsigned op_index_, uint16_t pitch_)
   {
      op_pitch[op_index_] = pitch_;

#if defined(REG_SIM)
      reg[0x20 + op_index_ * 2] = pitch_ >> 8;
      reg[0x21 + op_index_ * 2] = (reg[0x21 + op_index_ * 2] & 0b11) | (pitch_ & 0xFC);
#endif
   }

   //! Set operator pitch mode, fixed or ratio
   void setEgsOpPitchFixed(unsigned op_index_, bool fixed_)
   {
      op_pitch_fixed[op_index_] = fixed_;

#if defined(REG_SIM)
      reg[0x21 + op_index_ * 2] = (reg[0x21 + op_index_ * 2] & 0b11111100) | (fixed_ & 0b1);
#endif
   }

   //! Set operator detune [0x3030..303F]
   void setEgsOpDetune(unsigned op_index_, signed detune_)
   {
      op_detune[op_index_] = detune_;

#if defined(REG_SIM)
      reg[0x30 + op_index_] = detune_ < 0 ? 0b1000 | -detune_
                                          : detune_;
#endif
   }

   //! Set an operator envelope generator rate [0x3040..305F]
   void setEgsOpEgRate(unsigned op_index_, unsigned index_, uint8_t rate6_)
   {
      egs[op_index_]->setRate(index_, rate6_);

#if defined(REG_SIM)
      reg[0x40 + op_index_ * 4 + index_] = rate6_;
#endif
   }

   //! Set operator envelope generator target levels [0x3060..307F]
   void setEgsOpEgLevel(unsigned op_index_, unsigned index_, const uint8_t level6_)
   {
      egs[op_index_]->setLevel(index_, level6_);

#if defined(REG_SIM)
      reg[0x60 + op_index_ * 4 + index_] = level6_;
#endif
   }

   //! Set operator level [0x3080..]
   void setEgsOpLevel(unsigned op_index_, uint8_t level_)
   {
      egs[op_index_]->setOpLevel(level_);

#if defined(REG_SIM)
      reg[0x80 + op_index_] = level_;
#endif
   }

   //! Set operator rate scaling
   void setEgsOpRateScaling(unsigned op_index_, uint8_t scaling_)
   {
      op_rate_scaling[op_index_] = scaling_;

#if defined(REG_SIM)
      reg[0xE0 + op_index_] = (reg[0xE0 + op_index_] & 0b11111000) | (scaling_ & 0b111);
#endif
   }

   //! Set operator amplitude modulation sensitivity
   void setEgsOpAmpModSens(unsigned op_index_, uint8_t sens_)
   {
      op_amp_mod_sens[op_index_] = sens_;

#if defined(REG_SIM)
      reg[0xE0 + op_index_] = (reg[0xE0 + op_index_] & 0b111) | (sens_ << 3);
#endif
   }

   //! Set amplitude modulation (0..FF)
   void setEgsAmpMod(uint8_t amp_mod_)
   {
      amp_mod = amp_mod_;

#if defined(REG_SIM)
      reg[0xF0] = amp_mod_;
#endif
   }

   //! Start of note
   void keyOn()
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         egs[op_index]->keyOn();
      }

      Ops<NUM_OP>::keyOn();

#if defined(REG_SIM)
      reg[0xF1] = 1;
#endif
   }

   //! Release of note
   void keyOff()
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         egs[op_index]->keyOff();
      }

#if defined(REG_SIM)
      reg[0xF1] = 0;
#endif
   }

   //! Set pitch modulation (0..FFFF)
   void setEgsPitchMod(uint16_t pitch_mod_)
   {
      pitch_mod = pitch_mod_;

#if defined(REG_SIM)
      reg[0xF2] = pitch_mod_ >> 8;
      reg[0xF3] = pitch_mod_ & 0xFF;
#endif
   }

   //! Check if all amplitude envelopes are at L4
   //! NOTE: This is not functionality performed by a real DX
   bool isComplete() const
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         if (not egs[op_index]->isComplete())
            return false;
      }

      return true;
   }

   int32_t getEgsAmp(unsigned op_index_)
   {
      return egs[op_index_]->operator()();
   }

   //! get frequency value from the EGS for the OPS
   uint32_t getEgsFreq(unsigned op_index_)
   {
      uint16_t pitch = op_pitch_fixed[op_index_] ? 0 : voice_pitch;

      return pitch + (op_pitch[op_index_] >> 2) + op_detune[op_index_] + pitch_mod;
   }

   void debug()
   {
#if defined(REG_SIM)
      printf("\n");
      for(unsigned i = 0; i < 256; ++i)
      {
         if ((i % 8) == 0)
         {
            printf("\e[32m");
            printf("30%02X ", i);
            printf("\e[37m");
         }

         printf(" %02X", reg[i]);

         if ((i % 8) == 7)
         {
            printf("\n");
         }
      }
      printf("\n");
#endif
   }

private:
   static const unsigned NUM_OP = 6;

   // EGS registers
   uint16_t voice_pitch{0};
   uint16_t op_pitch[NUM_OP] = {0};
   bool     op_pitch_fixed[NUM_OP] = {false};
   int8_t   op_detune[NUM_OP] = {0};
   EnvGen*  egs[NUM_OP];
   uint8_t  op_rate_scaling[NUM_OP] = {0};
   uint8_t  op_amp_mod_sens[NUM_OP] = {0};
   uint8_t  amp_mod{0};                          // DX7 @ 0x30F0
   uint16_t pitch_mod{0x0};                      // DX7 @ 0x30F2

#if defined(REG_SIM)
   uint8_t  reg[256];
#endif
};
