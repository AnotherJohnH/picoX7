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

// \brief 16x2 Alphanumeric LCD interface

#pragma once

#include "hw/Config.h"

#if defined(HW_LCD_I2C)
#include "MTL/AlphaNumLcd_I2C.h"

#elif defined(HW_LCD_I2C_BRIDGE)
#include "MTL/AlphaNumLcd_I2CBridge.h"

#else
#define HW_LCD_NONE
#endif

namespace hw {

#if defined(HW_LCD_I2C)

// pico pin 24    : SDA
// pico pin 25    : SCL

class Lcd : public MTL::AlphaNumLcd<MTL::I2C1_P24_P25,
                                    /* COLS */ 16,
                                    /* ROWS */ 2>
{
public:
   Lcd()
      : MTL::AlphaNumLcd<MTL::I2C1_P24_P25,
                         /* COLS */ 16,
                         /* ROWS */ 2>{ HW_LCD_I2C_ADDR }
   {
   }
};

#elif defined(HW_LCD_I2C_BRIDGE)

// pico pin 24    : SDA
// pico pin 25    : SCL

class Lcd : public MTL::AlphaNumLcd<MTL::I2C1_P24_P25,
                                    /* COLS */ 16,
                                    /* ROWS */ 2>
{
public:
   Lcd()
      : MTL::AlphaNumLcd<MTL::I2C1_P24_P25,
                         /* COLS */ 16,
                         /* ROWS */ 2>{ HW_LCD_I2C_ADDR }
   {
   }
};

#endif

} // namespace hw
