#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(HW INTERFACE)
target_include_directories(HW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(${PLT_TARGET} STREQUAL rpipico)

   set(hw_config_list
       I2S_DAC
       PWM_DAC
       WAVESHARE_REV2_1
       PIMORONI_VGA_DEMO
       PIMORONI_PICO_AUDIO
       CACHE INTERNAL "Host List")

   target_link_libraries(HW INTERFACE TNY MTL)

   add_subdirectory(Test)

elseif(${PLT_TARGET} STREQUAL rpipico2)

   set(hw_config_list
       I2S_DAC
       PWM_DAC
       WAVESHARE_REV2_1
       PIMORONI_PICO_AUDIO
       CACHE INTERNAL "Host List")

   target_link_libraries(HW INTERFACE TNY MTL)

   add_subdirectory(Test)

elseif(${PLT_TARGET} STREQUAL native)

   set(hw_config_list
       NATIVE
       CACHE INTERNAL "Host List")

   target_link_libraries(HW INTERFACE PLT STB)

endif()
