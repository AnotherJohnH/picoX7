#-------------------------------------------------------------------------------
#  Copyright (c) 2023 John D. Haughton
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

   set(config_list
       WAVESHARE_GPIO_LCD
       WAVESHARE_I2C_LCD
       PIMORONI_VGA_DEMO
       PIMORONI_PICO_AUDIO
       PWM_DAC)

   set(target_libs TNY MTL TNY)

elseif(${PLT_TARGET} STREQUAL rpipico2)

   set(config_list
       WAVESHARE_GPIO_LCD
       WAVESHARE_I2C_LCD
       PIMORONI_PICO_AUDIO
       PWM_DAC)

   set(target_libs TNY MTL TNY)

elseif(${PLT_TARGET} STREQUAL native)

   set(config_list
       NATIVE)

   set(target_libs PLT STB)

endif()

set(python_path ${CMAKE_CURRENT_SOURCE_DIR}/../Platform/python)

add_subdirectory(DX7)

foreach(config ${config_list})

   add_executable(            picoX7_${config} picoX7.cpp)
   target_compile_definitions(picoX7_${config} PRIVATE HW_${config})
   target_include_directories(picoX7_${config} PRIVATE .)
   target_link_libraries(     picoX7_${config} PRIVATE DX7 ${target_libs})

endforeach()
