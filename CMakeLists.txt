#------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(picoX7
        LANGUAGES CXX C ASM
        VERSION 0.21)

include(CTest)

add_subdirectory(Platform)

add_subdirectory(Source)
add_subdirectory(docs/Research)

if(${PLT_TARGET} STREQUAL rpipico OR ${PLT_TARGET} STREQUAL rpipico2)
   math(EXPR plt_bcd_version "${CMAKE_MAJOR_VERSION} * 0x100 + ${CMAKE_MINOR_VERSION}"
        OUTPUT_FORMAT HEXADECIMAL)
   target_compile_definitions(MTL PUBLIC PLT_BCD_VERSION=${plt_bcd_version})
endif()
