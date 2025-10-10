//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Hardware LED

#pragma once

#include "hw/Config.h"

#if defined(HW_LED)
#include "MTL/Pins.h"
#include "MTL/Digital.h"
#endif

namespace hw {

#if defined(HW_LED)

using Led = MTL::Digital::Out<MTL::PIN_LED1>;

#else

using Led = bool;

#endif

} // namespace hw
