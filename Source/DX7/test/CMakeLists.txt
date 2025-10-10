#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   add_executable(test_DX7
                  testMain.cpp
                  testOps.cpp
                  testEgs.cpp
                  testEgsOpState.cpp
                  testEnvGen.cpp
                  testPitchEg.cpp)

   target_link_libraries(test_DX7
      PRIVATE DX7 STB)

   add_test(NAME test_DX7 COMMAND test_DX7)

endif()
