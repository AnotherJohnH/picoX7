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

class Voice : public VoiceBase
{
public:
   Voice() = default;

   void setDebug(bool debug_) { debug = debug_; }

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

   //! Start a new note
   void gateOn() override
   {
      fw.voiceAdd(getNote(), getVelocity());
   }

   //! Release a new note
   void gateOff() override
   {
      fw.voiceRemove();
   }

   void setPressure(uint8_t level_) override
   {
      fw.setModAfterTouch(level_);
   }

   void setControl(uint8_t control, uint8_t value) override
   {
      switch (control)
      {
      case  1: fw.setModWheel(value);         break;
      case  2: fw.setModBreathControl(value); break;
      case  4: fw.setModFootControl(value);   break;

      case 64: /* sustain */ break;
      case 65: /* portamento */ break;
      }
   }

   void setPitchBend(int16_t value) override
   {
      fw.setPitchBend(value);
   }

   //! Return next sample for this voice
   int32_t operator()()
   {
      return hw();
   } 

private:
   bool     debug{false};
   OpsAlg   hw;
   Firmware fw{hw};
};
