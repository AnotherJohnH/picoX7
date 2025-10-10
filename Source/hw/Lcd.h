//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief 16x2 Alphanumeric LCD interface

#pragma once

#include "hw/Config.h"

#if defined(HW_LCD_I2C)

#if defined(HW_LCD_I2C_BRIDGE)
#include "MTL/AlphaNumLcd_I2CBridge.h"
#else
#include "MTL/AlphaNumLcd_I2C.h"
#endif

#else
#define HW_LCD_NONE
#endif

namespace hw {

#if defined(HW_LCD_I2C)

class Lcd : public MTL::AlphaNumLcd<HW_LCD_I2C,
                                    /* COLS */ 16,
                                    /* ROWS */ 2>
{
public:
   Lcd()
      : MTL::AlphaNumLcd<HW_LCD_I2C,
                         /* COLS */ 16,
                         /* ROWS */ 2>{ HW_LCD_I2C_ADDR }
   {
      MTL::config.gpio(HW_LCD_I2C::SDA, "I2C SDA (16x2 LCD)");
      MTL::config.gpio(HW_LCD_I2C::SCL, "I2C SCL (16x2 LCD)");
   }
};

#endif

} // namespace hw
