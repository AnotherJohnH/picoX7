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

// \brief Pico Synth

#include <cstdio>

#include "PLT/Audio.h"
#include "PLT/Event.h"

#include "Simple/Synth.h"


class Monitor : public PLT::Audio::Out
{
public:
   Monitor()
      : PLT::Audio::Out(SYN::SAMPLE_FREQ, PLT::Audio::Format::SINT16, /* channels */ 2)
   {}

private:
   void getSamples(int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; i += 2)
      {
         buffer[i + 1] = buffer[i] = synth();
      }
   }

   Synth synth {};
};


int main()
{
   Monitor monitor;

   monitor.setEnable(true);

   return PLT::Event::mainLoop();
}
