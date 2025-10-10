//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "VoiceBase.h"

#include "DX7Firmware.h"
#include "SysEx.h"
#include "Egs.h"

namespace DX7 {

class Voice : public VoiceBase
{
public:
   Voice() = default;

   void loadProgram(const SysEx::Voice* voice)
   {
      fw.loadVoice(voice);
   }

   void tick()
   {
      if (hw.isComplete())
      {
         mute();
      }

      fw.tick();
   }

   //! Return next sample for this voice
   int32_t operator()()
   {
      return hw();
   }

private:
   //! Start a new note
   void gateOn() override
   {
      fw.voiceAdd(note, level);
   }

   //! Release a new note
   void gateOff() override
   {
      fw.voiceRemove();
   }

   void updatePitch() override
   {
      fw.setPitchBend(pitch);
   }

   void updateLevel() override
   {
      fw.setModAfterTouch(level);
   }

   void updateControl(uint8_t number_, uint8_t value_) override
   {
      switch (number_)
      {
      case  1: fw.setModWheel(value_);         break;
      case  2: fw.setModBreathControl(value_); break;
      case  4: fw.setModFootControl(value_);   break;

      case 64: /* sustain */ break;
      case 65: /* portamento */ break;
      }
   }

private:
   Egs      hw;
   Firmware fw{hw};
};

} // namespace DX7
