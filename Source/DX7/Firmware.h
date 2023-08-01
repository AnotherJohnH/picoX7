//------------------------------------------------------------------------------
// The data in this file is a derivative of data reverse engineered from the
// DX7 firmware ROM published on AJXS github project. That publicly
// available project does not contain a copyright notice or any mention of
// permissions or restrictions that apply to the use of the data. However,
// according to law that does not necessarily mean that the data is free from
// restrictions on its use. At the time of publication, the copyright holder 
// is probably Yamaha. If the copyright holder wishes to retrospectively declare
// reasonable and legal restrictions on the data, then either those restrictions
// must be obeyed or this file shouled be deleted.
//
// The statements above shall be included in all copies or substantial portions
// of the data.
//
// THE DATA IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE DATA OR THE USE OR OTHER DEALINGS IN THE
// DATA.
//------------------------------------------------------------------------------
//
// \brief DX7 firmware simulation
//
// Credit to AJXS for his excellent disassembly of the DX7 firmware ROM

#pragma once

#include "OpsAlg.h"

//! Model of Yamaha DX7 firmware
class Firmware
{
public:
   Firmware(OpsAlg& hw_)
      : hw(hw_)
   {
   }

   //! Implement PATCH_LOAD_DATA
   void loadData(const SysEx::Voice* patch)
   {
      for(unsigned i = 0; i < SysEx::NUM_OP; i++)
      {
         const SysEx::Op& op = patch->op[i];

         loadOperatorEgRates(i, op);
         loadOperatorEgLevels(i, op);
         loadOperatorPitch(i, op);
         loadOperatorDetune(i, op);
      }

      loadAlgMode(patch);

      hw.prog(patch);
   }

   //! Implement PATCH_LOAD_OPERATOR_EG_RATES
   void loadOperatorEgRates(unsigned index, const SysEx::Op& op)
   {
      uint8_t rates[4];

      for(unsigned i = 0; i < 4; ++i)
      {
         rates[i] = (op.eg_amp.rate[i] * 164) >> 8;
      }

      hw.setEgsOpEgRates(index, rates);
   }

   //! Implement PATCH_LOAD_OPERATOR_EG_LEVELS
   void loadOperatorEgLevels(unsigned index, const SysEx::Op& op)
   {
      uint8_t levels[4];

      for(unsigned i = 0; i < 4; ++i)
      {
         levels[i] = table_log[op.eg_amp.level[i]];
      }

      hw.setEgsOpEgLevels(index, levels);
   }

   //! Implement PATCH_LOAD_OPERATOR_PITCH
   void loadOperatorPitch(unsigned index, const SysEx::Op& op)
   {
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

         hw.setEgsOpPitchFixed(index, false);

         hw.setEgsOpPitch(index, table_op_freq_coarse[op.osc_freq_coarse] +
                                 table_op_freq_fine[op.osc_freq_fine] +
                                 0x232C);
      }
      else
      {
         const uint16_t table_op_freq_fixed[4] =
         {
            0x0000, 0x3526, 0x6A4C, 0x9F74
         };

         hw.setEgsOpPitchFixed(index, true);

         hw.setEgsOpPitch(index, table_op_freq_fixed[op.osc_freq_coarse & 0b11] +
                                 op.osc_freq_fine * 136 +
                                 0x16AC);
      }
   }

   //! Implement PATCH_LOAD_OPERATOR_DETUNE
   void loadOperatorDetune(unsigned index, const SysEx::Op& op)
   {
      hw.setEgsOpDetune(index, op.osc_detune);
   }

   //! Implement PATCH_LOAD_ALG_MODE
   void loadAlgMode(const SysEx::Voice* patch)
   {
      hw.setOpsSync(patch->osc_sync);
      hw.setOpsAlg(patch->alg);
      hw.setOpsFdbk(patch->feedback);
   }

private:
   const uint8_t table_log[100] =
   {
      0x7F, 0x7A, 0x76, 0x72, 0x6E, 0x6B, 0x68, 0x66,
      0x64, 0x62, 0x60, 0x5E, 0x5C, 0x5A, 0x58, 0x56,
      0x55, 0x54, 0x52, 0x51, 0x4F, 0x4E, 0x4D, 0x4C,
      0x4B, 0x4A, 0x49, 0x48, 0x47, 0x46, 0x45, 0x44,
      0x43, 0x42, 0x41, 0x40, 0x3F, 0x3E, 0x3D, 0x3C,
      0x3B, 0x3A, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34,
      0x33, 0x32, 0x31, 0x30, 0x2F, 0x2E, 0x2D, 0x2C,
      0x2B, 0x2A, 0x29, 0x28, 0x27, 0x26, 0x25, 0x24,
      0x23, 0x22, 0x21, 0x20, 0x1F, 0x1E, 0x1D, 0x1C,
      0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14,
      0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C,
      0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04,
      0x03, 0x02, 0x01, 0x00
   };

   OpsAlg& hw; //! Access to simulated DX7 EGS and OPS hardware
};
