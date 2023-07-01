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

#include <cstring>

#include "SYN/VoiceBase.h"

#include "Table_dx7_program.h"

#include "OpsAlg.h"
#include "SysEx.h"

class Voice : public VoiceBase
{
public:
   Voice() = default;

   SYN::Sample operator()() { return ops(); }

   void tick()
   {
   }

   void gateOn() override
   {
      ops.gateOn(note);
   }

   void gateOff() override
   {
      ops.gateOff();
   }

   void setLevel(uint8_t value) override
   {
   }

   void setControl(uint8_t control, uint8_t value) override
   {
   }

   void setPitch(int16_t value) override
   {
   }

   void setProgram(uint8_t prog) override
   {
      const SysEx* sysex = (const SysEx*) &table_dx7_program[prog * sizeof(SysEx)];

      if (debug)
         sysex->print();

      ops.prog(sysex);
   }

   void setDebug(bool debug_) { debug = debug_; }

private:
   OpsAlg ops;
   bool   debug{false};
};
