#-------------------------------------------------------------------------------
#  Copyright (c) 2024 John D. Haughton
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL rpipico)

   set(hw_config_list
       WAVESHARE_GPIO_LCD
       WAVESHARE_I2C_LCD
       PIMORONI_VGA_DEMO
       PIMORONI_PICO_AUDIO
       PWM_DAC
       PARENT_SCOPE)

   set(hw_target_libs TNY MTL TNY
       PARENT_SCOPE)

elseif(${PLT_TARGET} STREQUAL rpipico2)

   set(hw_config_list
       WAVESHARE_GPIO_LCD
       WAVESHARE_I2C_LCD
       PIMORONI_PICO_AUDIO
       PWM_DAC
       PARENT_SCOPE)

   set(hw_target_libs TNY MTL TNY
       PARENT_SCOPE)

elseif(${PLT_TARGET} STREQUAL native)

   set(hw_config_list
       NATIVE
       PARENT_SCOPE)

   set(hw_target_libs PLT STB
       PARENT_SCOPE)

endif()
