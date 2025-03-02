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

add_custom_command(
   COMMENT
      "Auto-generate for DX7"
   COMMAND
      ${CMAKE_COMMAND}
         -E env PYTHONPATH=${python_path}
         ${CMAKE_CURRENT_SOURCE_DIR}/AUTO.py
             ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom1a.syx
             ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom1b.syx
             ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom2a.syx
             ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom2b.syx
   DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/AUTO.py
      ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom1a.syx
      ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom1b.syx
      ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom2a.syx
      ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom2b.syx
      ${python_path}/table.py
   OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_exp_14.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_exp_14.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_exp_22.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_exp_22.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_exp_19.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_exp_19.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_log_sine_14.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_log_sine_14.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_rom_1.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_rom_1.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_rom_2.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_rom_2.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_rom_3.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_rom_3.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_rom_4.h
      ${CMAKE_CURRENT_BINARY_DIR}/Table_dx7_rom_4.cpp
   )

add_library(DX7 STATIC
   SysEx.cpp
   Table_dx7_exp_14.cpp
   Table_dx7_exp_19.cpp
   Table_dx7_exp_22.cpp
   Table_dx7_log_sine_14.cpp
   Table_dx7_rom_1.cpp
   Table_dx7_rom_2.cpp
   Table_dx7_rom_3.cpp
   Table_dx7_rom_4.cpp
   )

target_include_directories(DX7
   PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
   )

add_subdirectory(test)

