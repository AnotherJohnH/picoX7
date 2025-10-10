//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

//! I/O interfaces for the Synth
class SynthIO
{
public:
   SynthIO() = default;

   //! Display number on 2x 7 segment LED
   void displayLED(unsigned number_);

   //! Display on text on 16x2 LCD
   void displayLCD(unsigned row_, const char* text);

   //! Read data entry control
   unsigned readControl();

   //! Read button matrix
   uint8_t readButtons(uint8_t row);
};
