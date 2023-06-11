#pragma once

#include <cstdint>

static const unsigned LOG2_TABLE_SINE_SIZE = 9;
static const unsigned TABLE_SINE_SIZE      = 1 << LOG2_TABLE_SINE_SIZE;
static const unsigned TABLE_SINE_MASK      = TABLE_SINE_SIZE - 1;

extern const int16_t table_sine[TABLE_SINE_SIZE];
