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

#include "EnvGen.h"

//! Model of Yamaha YM21290 EGS
template <unsigned NUM_OP>
class Egs
{
public:
   Egs() = default;

   //! Check if all amplitude envelopes are at L4
   //! NOTE: This is not functionality performed by a real DX
   bool isComplete() const
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         if (not egs[i].isComplete())
            return false;
      }

      return true;
   }

   //! Re-program voice from SysEx
   void prog(const SysEx::Voice* patch)
   {
      for(unsigned i = 0; i < NUM_OP; i++)
      {
         const SysEx::Op& op = patch->op[5 - i];

         egs[i].prog(op.eg_amp, op.out_level);

         loadOperatorPitch(i, patch);
         loadOperatorDetune(i, patch);
      }
   }

   //! Start of note
   void keyOn()
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         egs[i].gateOn();
      }
   }

   //! Release of note
   void keyOff()
   {
      for(unsigned i = 0; i < NUM_OP; ++i)
      {
         egs[i].gateOff();
      }
   }

protected:
   int32_t getEgsAmp(unsigned index)
   {
      return egs[index]();
   }

private:
   //! Implement PATCH_LOAD_OPERATOR_PITCH
   void loadOperatorPitch(unsigned index, const SysEx::Voice* patch)
   {
      const SysEx::Op& op = patch->op[index];

      if (op.osc_mode == SysEx::RATIO)
      {
         const uint16_t table_op_freq_coarse[32] =
         {
            0xF000, 0x0000, 0x1000, 0x195C, 0x2000, 0x2528, 0x295C, 0x2CEC,
            0x3000, 0x32B8, 0x3528, 0x375A, 0x395C, 0x3B34, 0x3CEC, 0x3E84,
            0x4000, 0x4168, 0x42B8, 0x43F8, 0x4528, 0x4648, 0x475A, 0x4860,
            0x495C, 0x4A4C, 0x4B34, 0x4C14, 0x4CEC, 0x4DBA, 0x4E84, 0x4F44
         };

         const uint16_t table_op_freq_fine[100] =
         {
            0x000, 0x03A, 0x075, 0x0AE, 0x0E7, 0x120, 0x158, 0x18F, 0x1C6, 0x1FD,
            0x233, 0x268, 0x29D, 0x2D2, 0x306, 0x339, 0x36D, 0x39F, 0x3D2, 0x403,
            0x435, 0x466, 0x497, 0x4C7, 0x4F7, 0x526, 0x555, 0x584, 0x5B2, 0x5E0,
            0x60E, 0x63B, 0x668, 0x695, 0x6C1, 0x6ED, 0x719, 0x744, 0x76F, 0x799,
            0x7C4, 0x7EE, 0x818, 0x841, 0x86A, 0x893, 0x8BC, 0x8E4, 0x90C, 0x934,
            0x95C, 0x983, 0x9AA, 0x9D1, 0x9F7, 0xA1D, 0xA43, 0xA69, 0xA8F, 0xAB4,
            0xAD9, 0xAFE, 0xB22, 0xB47, 0xB6B, 0xB8F, 0xBB2, 0xBD6, 0xBF9, 0xC1C,
            0xC3F, 0xC62, 0xC84, 0xCA7, 0xCC9, 0xCEA, 0xD0C, 0xD2E, 0xD4F, 0xD70,
            0xD91, 0xDB2, 0xDD2, 0xDF3, 0xE13, 0xE33, 0xE53, 0xE72, 0xE92, 0xEB1,
            0xED0, 0xEEF, 0xF0E, 0xF2D, 0xF4C, 0xF6A, 0xF88, 0xFA6, 0xFC4, 0xFE2
         };

         op_pitch_fixed[index] = false;

         op_pitch[index] = table_op_freq_coarse[op.osc_freq_coarse] +
                           table_op_freq_fine[op.osc_freq_fine] +
                           0x232C;
      }
      else
      {
         const uint16_t table_op_freq_fixed[4] =
         {
            0x0000, 0x3526, 0x6A4C, 0x9F74
         };

         op_pitch_fixed[index] = true;

         op_pitch[index] = table_op_freq_fixed[op.osc_freq_coarse & 0b11] +
                           op.osc_freq_fine * 136 +
                           0x16AC;
      }
   }

   //! Implement PATCH_LOAD_OPERATOR_DETUNE
   void loadOperatorDetune(unsigned index, const SysEx::Voice* patch)
   {
      const SysEx::Op& op = patch->op[index];

      op_detune[index] = op.osc_detune - 7;
   }

   // EGS program state
   EnvGen   egs[NUM_OP];
   uint8_t  op_rate_scaling[NUM_OP] = {0};
   uint8_t  op_amp_mod_sens[NUM_OP] = {0};
   bool     op_pitch_fixed[NUM_OP] = {false};
   uint16_t op_pitch[NUM_OP] = {0};
   int8_t   op_detune[NUM_OP] = {0};

   // EGS voice state
   uint8_t  op_levels[NUM_OP] {0};
   uint16_t amp_mod {0};
   uint16_t voice_pitch {0};
   uint16_t pitch_mod {0};
};
