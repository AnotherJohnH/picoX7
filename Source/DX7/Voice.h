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

#include "SYN/VoiceBase.h"

#include "OpsAlg.h"

class Voice : public VoiceBase
{
public:
   Voice() = default;

   SYN::Sample operator()()
   {
      return ops.alg32();
   }

   void tick()
   {
   }

   void gateOn() override
   {
      for(unsigned i = 0; i < 6; i++)
      {
         ops.setFrq(i, 37 * 4);
         ops.setAmp(i, 0xFFF);
      }
      ops.setFbk(7);
      //ops.setFrq(4, 36 * 4);
   }

   void gateOff() override
   {
      for(unsigned i = 0; i < 6; i++)
      {
         ops.setAmp(i, 0);
      }
      setMute();
   }

   void setLevel(uint8_t value) override
   {
      for(unsigned i = 0; i < 6; i++)
      {
         ops.setAmp(i, value);
      }
   }

   void setControl(uint8_t control, uint8_t value) override
   {
   }

   void setPitch(int16_t value) override
   {
   }

private:
   OpsAlg ops;
};
