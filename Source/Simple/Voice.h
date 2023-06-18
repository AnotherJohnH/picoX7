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

#include "SYN/Sine.h"
#include "SYN/VoiceBase.h"

#include "Table_ampl.h"

class Voice : public VoiceBase
{
public:
   Voice() = default;

   SYN::Sample operator()()
   {
      return osc();
   }

   void tick()
   {
   }

   void gateOn() override
   {
      osc.sync();
      osc.setNote(note);
   }

   void gateOff() override
   {
      setMute();
   }

   void setLevel(uint8_t value) override
   {
      osc.gain = SYN::Gain::fromRaw(table_ampl[value]);
   }

   void setControl(uint8_t control, uint8_t value) override
   {
   }

   void setPitch(int16_t value) override
   {
      osc.setTune(value);
   }

private:
   SYN::Sine osc{};
};
