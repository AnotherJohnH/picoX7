//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "OpsAlg6.h"
#include "EnvGen.h"
#include "EgsOpState.h"

//! Model of Yamaha EGS (like the YM21290)
class Egs : public DX::OpsAlg6<EnvGen>
{
public:
   Egs()
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         op[i].init(getEgPointer(i));
      }
   }

   // EGS inputs

   //! Set voice pitch
   void setEgsVoicePitch(uint16_t pitch16_)
   {
      // LS 2 bits are zero
      voice_pitch14 = pitch16_ >> 2;

      for(unsigned op_index = 0; op_index < 6; ++op_index)
      {
         op[op_index].updateRates(voice_pitch14);
      }
   }

   //! Start of note
   void keyOn()
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         op[op_index].env_gen->keyOn();
      }

      Ops<NUM_OP,EnvGen>::keyOn();
   }

   //! Release of note
   void keyOff()
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         op[op_index].env_gen->keyOff();
      }
   }

   //! Set pitch modulation
   void setEgsPitchMod(int16_t pitch_mod_)
   {
      pitch_mod12 = pitch_mod_ >> 4;
   }

   //! Set amplitude modulation (0..FF)
   void setEgsAmpMod(uint8_t amp_mod8_)
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         op[op_index].setAmpMod(amp_mod8_);
      }
   }

   //! Check if all amplitude envelopes are at L4
   //! NOTE: This is not functionality performed by a real DX
   bool isComplete() const
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         if (not op[op_index].env_gen->isComplete())
            return false;
      }

      return true;
   }

   //! Used by unit test
   int32_t getEgsAmp(unsigned op_index_)
   {
      return op[op_index_].env_gen->getAtten12();
   }

   //! Send frequency value from the EGS to the OPS
   void sendEgsFreq()
   {
      for(unsigned op_index = 0; op_index < 6; op_index++)
      {
         setOpsFreq(op_index,
                    op[op_index].computeOpsFreq14(voice_pitch14, pitch_mod12));
      }
   }

   static const unsigned NUM_OP = 6;

   EgsOpState<EnvGen> op[NUM_OP];

private:
   // State representing EGS registers
   uint16_t voice_pitch14{0};
   int16_t  pitch_mod12{0x0};     // DX7 @ 0x30F2
};
