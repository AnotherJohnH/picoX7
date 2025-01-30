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
// \brief DX7 firmware simulation - modulation sources
//
// Credit to AJXS for his excellent disassembly of the DX7 firmware ROM

#pragma once

#include <cstdint>

#include "SysEx.h"

//! Model of Yamaha DX7 modulation source firmware
class Modulation
{
public:
   enum Source
   {
      MOD_WHEEL = 0,
      FOOT_CONTROL,
      BREATH_CONTROL,
      AFTER_TOUCH,
      NUM_SOURCES
   };

   Modulation() = default;

   uint8_t getPitchFactor() const { return pitch_factor; }
   uint8_t getAmpFactor() const   { return amp_factor; }
   uint8_t getAmpEGBias() const   { return amp_eg_bias; }

   //! Program ranges and assigns from SysEx parameter patch
   void load(const SysEx::Param& param_)
   {
      source[MOD_WHEEL]     .load(param_.mod_wheel_range,      param_.mod_wheel_assign);
      source[FOOT_CONTROL]  .load(param_.foot_control_range,   param_.foot_control_assign);
      source[BREATH_CONTROL].load(param_.breath_control_range, param_.breath_control_assign);
      source[AFTER_TOUCH].   load(param_.after_touch_range,    param_.after_touch_assign);

      calcTotals();
   }

   //! Supply modulation source raw input
   void rawInput(Source index_, uint8_t value_)
   {
      source[index_].input(value_);

      calcTotals();
   }

private:
   void calcTotals()
   {
      pitch_factor = 0;
      amp_factor   = 0;
      amp_eg_bias  = 0;

      for(unsigned i = 0; i < NUM_SOURCES; ++i)
      {
         source[i].accumTotal(source[i].getPitchAssign(),  pitch_factor);
         source[i].accumTotal(source[i].getAmpAssign(),    amp_factor);
         source[i].accumTotal(source[i].getEGBiasAssign(), amp_eg_bias);
      }
   }

   //! State for one modulation source
   struct SourceState
   { 
   public:
      SourceState() = default;

      //! Get the assign flag states
      bool getPitchAssign()  const { return pitch; }
      bool getAmpAssign()    const { return amp; }
      bool getEGBiasAssign() const { return eg_bias; }

      //! Program range and assign flags
      void load(uint8_t range_, uint8_t assign_)
      {
         quant_range  = (range_ * 660) >> 8;
         pitch        = (assign_ & 0b001) != 0;
         amp          = (assign_ & 0b010) != 0;
         eg_bias      = (assign_ & 0b100) != 0;

         computeScaledInput();
      }

      void input(uint8_t raw_)
      {
         raw_input = raw_;

         computeScaledInput();
      }

      void accumTotal(bool assign_, uint8_t& total_)
      {
         if (not assign_)
            return;

         unsigned accum = total_ + scaled_input;
         total_ = accum > 0xFF ? 0xFF : accum;
      }

   private:
      void computeScaledInput()
      {
         scaled_input = (raw_input * quant_range) >> 8;
      }

      uint8_t quant_range{0};
      bool    pitch{false};
      bool    amp{false};
      bool    eg_bias{false};
      uint8_t raw_input{0};
      uint8_t scaled_input{0};

   } source[NUM_SOURCES];

   uint8_t pitch_factor{0};
   uint8_t amp_factor{0};
   uint8_t amp_eg_bias{0};
};
