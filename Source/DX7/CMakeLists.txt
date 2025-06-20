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

autogen_py(Table_dx_exp_14)
autogen_py(Table_dx_exp_19)
autogen_py(Table_dx_exp_32)
autogen_py(Table_dx_log_sine_14)
autogen_py(Table_dx7_rom_1 ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom1a.syx)
autogen_py(Table_dx7_rom_2 ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom2a.syx)
autogen_py(Table_dx7_rom_3 ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom3a.syx)
autogen_py(Table_dx7_rom_4 ${CMAKE_CURRENT_SOURCE_DIR}/cart/rom4a.syx)

add_library(DX7 STATIC
   SysEx.cpp
   Table_dx_exp_14.cpp
   Table_dx_exp_19.cpp
   Table_dx_exp_32.cpp
   Table_dx_log_sine_14.cpp
   Table_dx7_rom_1.cpp
   Table_dx7_rom_2.cpp
   Table_dx7_rom_3.cpp
   Table_dx7_rom_4.cpp
   )

target_include_directories(DX7
   PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
   )

add_subdirectory(test)
