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

#if defined(MTL_rpipico) || defined(MTL_rpipico2)

#include "MTL/Config.h"
#include "MTL/core/SysTick.h"

#else

// allow building completely non function
namespace MTL { using SysTick = uint32_t; }

const unsigned FLASH_SIZE = 100;
const unsigned RAM_SIZE   = 100;

#endif

//! Resource usage measurement
class Usage
{
public:
   Usage() = default;

   //! Start CPU usage timing
   void start()
   {
      start_t24 = tick;
      non_usage = (end_t24 - start_t24) & 0xFFFFFF;
   }

   //! Stop CPU usage timing
   void end()
   {
      end_t24 = tick;
      usage   = (start_t24 - end_t24) & 0xFFFFFF;
   }

   //! Get CPU usage (%)
   unsigned getCPUUsage() const
   {
      return usage * 100 / (non_usage + usage);
   }

   //! Get FLASH memory usage (%)
   unsigned getFLASHUsage() const
   {
      extern uint8_t __text_start__;
      extern uint8_t __text_end__;

      return unsigned(&__text_end__ - &__text_start__) * 100 / FLASH_SIZE;
   }

   //! Get RAM memory usage (%)
   unsigned getRAMUsage() const
   {
      extern uint8_t __data_start__;
      extern uint8_t __bss_end__;

      return (&__bss_end__ - &__data_start__) * 100 / RAM_SIZE;
   }

private:
   MTL::SysTick      tick {};
   uint32_t          start_t24 {0};
   uint32_t          end_t24 {0};
   volatile uint32_t usage{0};
   volatile uint32_t non_usage{0};
};
