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

#if defined(HW_LCD_GPIO)
#include "MTL/AlphaNumLcd_Gpio.h"

#elif defined(HW_LCD_I2C)
#include "MTL/AlphaNumLcd_I2C.h"

#elif defined(HW_LCD_I2C_BRIDGE)
#include "MTL/AlphaNumLcd_I2CBridge.h"

#else
#define HW_LCD_NONE
#endif

namespace hw {

#if defined(HW_LCD_GPIO)

// pico pin 9-12  : DATA[0:3]
// pico pin 14-17 : DATA[4:7]
// pico pin 19    : RS
// pico pin 20    : E

using Lcd = MTL::AlphaNumLcd</* PIN_DATA */   MTL::PIN_9,
                             /* PIN_R_S */    MTL::PIN_19,
                             /* PIN_ENABLE */ MTL::PIN_20,
                             /* COLS */       16,
                             /* ROWS */       2,
                             /* DL_8BIT */    true>;

#elif defined(HW_LCD_I2C)

// pico pin 19    : SDA
// pico pin 20    : SCL

class Lcd : public MTL::AlphaNumLcd<MTL::I2C1_P19_P20,
                                    /* COLS */ 16,
                                    /* ROWS */ 2>
{
public:
   Lcd()
      : MTL::AlphaNumLcd<MTL::I2C1_P19_P20,
                         /* COLS */ 16,
                         /* ROWS */ 2>{ HW_LCD_I2C_ADDR }
   {
   }
};

#elif defined(HW_LCD_I2C_BRIDGE)

// pico pin 19    : SDA
// pico pin 20    : SCL

class Lcd : public MTL::AlphaNumLcd<MTL::I2C1_P19_P20,
                                    /* COLS */ 16,
                                    /* ROWS */ 2>
{
public:
   Lcd()
      : MTL::AlphaNumLcd<MTL::I2C1_P19_P20,
                         /* COLS */ 16,
                         /* ROWS */ 2>{ HW_LCD_I2C_ADDR }
   {
   }
};

#endif

} // namespace hw
