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

#include "VoiceBase.h"

#include "Firmware.h"
#include "SysEx.h"
#include "Egs.h"

namespace DX7 {

class Voice : public VoiceBase
{
public:
   Voice() = default;

   void loadProgram(uint8_t number, const SysEx::Voice* voice)
   {
      if (debug)
      {
         voice->print(number + 1);
      }

      fw.loadData(voice);
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
