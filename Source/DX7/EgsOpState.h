//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

template <typename EG_TYPE>
class EgsOpState
{
public:
   EgsOpState() = default;

   //! Initialise the state
   void init(EG_TYPE* env_gen_)
   {
      env_gen = env_gen_;
   }

   //! Set pitch ratio from patch
   void setPitchRatio(uint16_t pitch_ratio_16_)
   {
      pitch_ratio_14 = pitch_ratio_16_ >> 2;
   }

   //! Set pitch mode, fixed or ratio from patch
   void setPitchFixed(bool is_pitch_fixed_)
   {
      is_pitch_fixed = is_pitch_fixed_;
   }

   //! Set pitch detune from patch
   void setDetune(signed detune_4_)
   {
      detune_4 = detune_4_;
   }

   //! Set EG attenuation targete level from patch
   void setEgAtten(unsigned index_, uint8_t atten_6_)
   {
      // Convert to 8-bit
      // 0x00 - No attenuation
      // 0xFF - Full attenuation
      // XXX "interpolating" LS 2 bits from top 2 bits (may not be accurate?)
      eg[index_].atten_8 = (atten_6_ << 2) | (atten_6_ >> 4);
   }

   //! Set EG rate from patch
   void setEgRate(unsigned index_, uint8_t rate_6_)
   {
      eg[index_].rate_6 = rate_6_;
   }

   //! Set rate scaling from patch
   void setEgRateScale(uint8_t scale_3_)
   {
      rate_scale_6 = (scale_3_ << 3) | scale_3_;
   }

   //! Set amplitude modulation senitivity from patch
   void setAmpModSens(uint8_t amp_mod_sens_2_)
   {
      switch(amp_mod_sens_2_)
      {
      case 0b00: amp_mod_sens_12 = 0xFFF; break;
      case 0b01: amp_mod_sens_12 = 0xAAA; break;
      case 0b10: amp_mod_sens_12 = 0x555; break;
      case 0b11: amp_mod_sens_12 = 0x000; break;
      }
   }


   //! Update operator attenuation level for new note
   void setOpAtten(uint8_t op_atten_8_)
   {
      for(unsigned i = 0; i < EG_TYPE::NUM_CONTROL; ++i)
      {
         unsigned atten_8 = eg[i].atten_8 + op_atten_8_;
         if (atten_8 > 0xFF)
            atten_8 = 0xFF;

         env_gen->setAtten8(i, atten_8);
      }
   }

   //! Update rates for new note
   void updateRates(uint16_t voice_pitch14_)
   {
      for(unsigned i = 0; i < EG_TYPE::NUM_CONTROL; ++i)
      {
         uint8_t rate_6 = eg[i].rate_6;

         // TODO add rate_scale

         env_gen->setRate6(i, rate_6);
      }
   }

   //! Set amplitude modulation
   void setAmpMod(uint8_t amp_mod_8_)
   {
      signed amp_mod_12 = (amp_mod_8_ << 4) | (amp_mod_8_ >> 4);

      amp_mod_12 -= amp_mod_sens_12;
      if (amp_mod_12 < 0)
         amp_mod_12 = 0;

      env_gen->setAmpMod(amp_mod_12);
   }

   //! Compute the 14-bit frequency value to send to the OPS
   uint32_t computeOpsFreq14(uint16_t voice_pitch_14_, int16_t pitch_mod_12_) const
   {
      int32_t pitch_14 = pitch_ratio_14;

      if (is_pitch_fixed)
         pitch_14 -= 0x1000;
      else
         pitch_14 += voice_pitch_14_;

      pitch_14 += detune_4 + pitch_mod_12_;

      return pitch_14 & 0x3FFF;
   }

   EG_TYPE* env_gen{};

private:
   struct EgState
   {
      uint8_t rate_6{0};
      uint8_t atten_8{0};
   };

   uint16_t pitch_ratio_14{0};
   bool     is_pitch_fixed{false};
   int8_t   detune_4{0};
   uint8_t  rate_scale_6{0};
   EgState  eg[EG_TYPE::NUM_CONTROL];
   uint16_t amp_mod_sens_12{0};
};
