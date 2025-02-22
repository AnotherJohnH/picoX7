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

#include "Table_dx7_exp_14.h"
#include "Table_dx7_exp_22.h"
#include "Table_dx7_log_sine_14.h"

#include "EnvGen.h"

//! Model of Yamaha OPS (like the YM21280)
template <unsigned NUM_OP>
class Ops
{
public:
   Ops() = default;

   EnvGen* getEgsPointer(unsigned op_index_) { return &state[op_index_].egs; }

   //! Set the oscillator sync mode
   void setOpsSync(bool sync_)
   {
      sync = sync_;
   }

   //! Set the algorithm feedback level
   void setOpsFdbk(uint8_t feedback_)
   {
      fdbk = (7 - feedback_) + 4;
   }

   //! Set operator frequency
   void setOpsFreq(unsigned op_index, uint32_t f14)
   {
      state[op_index].phase_inc_32 = table_dx7_exp_22[f14] << 13;
   }

   //! Start of note
   void keyOn()
   {
      for(unsigned op_index = 0; op_index < NUM_OP; ++op_index)
      {
         if (sync)
         {
            state[op_index].phase_acc_32 = 0;
         }
      }
   }

protected:
   //! Simulate a single operator (and associated envelope generator)
   template <unsigned OP_NUMBER, unsigned SEL, bool A, bool C, bool D, unsigned COM, unsigned LOG2_COM>
   int32_t ops()
   {
      // Documented operator number 1-6 map to internal operator index 5-0
      // the internal index follows the order of operator computation
      // and the order of operator parameter encoding in the SysEx patch
      // encoding
      const unsigned op_index = NUM_OP - OP_NUMBER;

      uint32_t phase_12  = (state[op_index].stepPhase() + (modulation_12 << 23)) >> 20;
      int32_t  amp_13    = state[op_index].egs();
      uint32_t log_15    = table_dx7_log_sine_14[phase_12] + amp_13;

      log_15 += LOG2_COM << 7;

      if (log_15 > 0x3FFF)
         log_15 = 0x3FFF;

      signed output_14 = table_dx7_exp_14[0x3FFF - log_15];
      if (phase_12 > 0x800)
         output_14 = -output_14;

      signed sum_15 = 0;
      if (C) sum_15 = memory_15;
      if (D) sum_15 += output_14;

      switch(SEL)
      {
      case 0: modulation_12 = 0;                                           break;
      case 1: modulation_12 = output_14 >> 3;                              break;
      case 2: modulation_12 = sum_15 >> 3;                                 break;
      case 3: modulation_12 = memory_15 >> 3;                              break;
      case 4: modulation_12 = feedback1_15 >> 3;                           break;
      case 5: modulation_12 = (feedback1_15 + feedback2_15) >> (fdbk + 1); break;
      }

      if (A)
      {
         // Feeedback path enabled
         feedback2_15 = feedback1_15;
         feedback1_15 = output_14;
      }

      if (C or D)
      {
         // Memory write enable
         memory_15 = sum_15;
      }

      return sum_15 << 1;
   }

private:
   // Externaly configured state
   bool     sync {true};
   uint8_t  fdbk {0};

   // Internal voice computation state
   int32_t  modulation_12 {0};
   int32_t  feedback1_15 {0};
   int32_t  feedback2_15 {0};
   int32_t  memory_15 {0};

   // Internal operator state
   struct State
   {
      EnvGen   egs{};
      uint32_t phase_acc_32{0};
      uint32_t phase_inc_32{0};

      uint32_t stepPhase()
      {
          phase_acc_32 += phase_inc_32;
          return phase_acc_32;
      }
   };

   State state[NUM_OP];
};
