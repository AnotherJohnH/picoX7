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

set(python_path ${CMAKE_CURRENT_SOURCE_DIR}/../Platform/python)

add_subdirectory(DX7)

if(${PLT_TARGET} STREQUAL rpipico)

   foreach(hardware
           WAVESHARE_BREAD_BOARD
           WAVESHARE_PIGGY_BACK)

      add_executable(            picoX7_${hardware} picoX7.cpp)
      target_compile_definitions(picoX7_${hardware} PRIVATE HW_${hardware})
      target_include_directories(picoX7_${hardware} PRIVATE .)
      target_link_libraries(     picoX7_${hardware} PRIVATE DX7 TNY MTL TNY)

   endforeach()

elseif(${PLT_TARGET} STREQUAL native)

   add_executable(            picoX7 picoX7_native.cpp)
   target_include_directories(picoX7 PRIVATE .)
   target_link_libraries(     picoX7 PRIVATE DX7 PLT STB)

endif()

