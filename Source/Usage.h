//------------------------------------------------------------------------------
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <https://unlicense.org>
//------------------------------------------------------------------------------

#pragma once

#include "MTL/CortexM0/SysTick.h"
#include "MTL/Config.h"

//! Resource usgae measurement
class Usage
{
public:
   Usage() = default;

   void start()
   {
      start_t24 = tick;
      non_usage = (end_t24 - start_t24) & 0xFFFFFF;
   }

   void end()
   {
      end_t24 = tick;
      usage   = (start_t24 - end_t24) & 0xFFFFFF;
   }

   unsigned getCPUUsage() const
   {
      return usage * 100 / (non_usage + usage);
   }

   unsigned getFLASHUsage() const
   {
      extern uint8_t __text_start__;
      extern uint8_t __text_end__;

      return unsigned(&__text_end__ - &__text_start__) * 100 / FLASH_SIZE;
   }

   unsigned getRAMUsage() const
   {
      extern uint8_t __data_start__;
      extern uint8_t __bss_end__;

      return (&__bss_end__ - &__data_start__) * 100 / RAM_SIZE;
   }

   void wait(unsigned ticks)
   {
      uint32_t future = (tick + ticks) & 0xFFFFFF;

      while(tick < future);
   }

private:
   MTL::SysTick      tick {};
   uint32_t          start_t24 {0};
   uint32_t          end_t24 {0};
   volatile uint32_t usage{0};
   volatile uint32_t non_usage{0};
};
