//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

// \brief Hardware 2 digit 7-segment LED display

#pragma once

#include "hw/Config.h"

#if defined(HW_LED_7_SEG)
#include "MTL/Pins.h"
#include "MTL/Led7Seg.h"
#endif

namespace hw {

#if defined(HW_LED_7_SEG)

// pico pin 4 : DATA
// pico pin 5 : CLOCK

class Led7Seg : public MTL::Led7Seg<HW_LED_7_SEG_CLK,
                                    HW_LED_7_SEG_DAT,
                                    /* NUM_DIGITS */ 2>
{
public:
   Led7Seg()
   {
      MTL::config.gpio(HW_LED_7_SEG_CLK, "7-SEG LED clk");
      MTL::config.gpio(HW_LED_7_SEG_DAT, "7-SEG LED data");
   }
};

#endif

} // namespace hw
