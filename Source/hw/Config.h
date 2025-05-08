//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

// \brief Target hardware configurations

#pragma once

#if defined(HW_WAVESHARE_REV2_1)

#define HW_DESCR "WaveShare Rev2.1 I2S DAC"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_UART1
#define HW_DAC_I2S_WAVESHARE_REV2_1
#define HW_LED
#define HW_LED_7_SEG
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E

#elif defined(HW_PIMORONI_PICO_AUDIO)

#define HW_DESCR "Pimoroni pico audio I2S DAC"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_UART1
#define HW_DAC_I2S_PIMORONI_PICO_AUDIO
#define HW_LED
#define HW_LED_7_SEG
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E

#elif defined(HW_PIMORONI_VGA_DEMO)

#define HW_DESCR "Pimoroni VGA Demo I2S DAC"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_FAKE
#define HW_DAC_I2S_PIMORONI_VGA_DEMO
#define HW_LED
#define HW_LED_7_SEG

#elif defined(HW_I2S_DAC)

#define HW_DESCR "I2S DAC"

#define HW_DAC_I2S_ANY
#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_UART1
#define HW_LED
#define HW_LED_7_SEG
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E

#elif defined(HW_PWM_DAC)

#define HW_DESCR "PWM DAC"

#define HW_DAC_PWM
#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_UART1
#define HW_LED
#define HW_LED_7_SEG
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E

#elif defined(HW_NATIVE)

#define HW_DESCR "native"

#define HW_DAC_NATIVE
#define HW_MIDI_IN_NATIVE

#elif defined(HW_DEDICATED_GPIO_LCD)

#define HW_DESCR "Dedicated PCB + GPIO LCD"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_UART1
#define HW_DAC_PIMORONI_PICO_AUDIO
#define HW_LED
#define HW_LED_7_SEG
#define HW_LCD_GPIO_ALTERNATE

#elif defined(HW_DEDICATED_I2C_LCD)

#define HW_DESCR "Dedicated PCB + I2C LCD"

#define HW_MIDI_USB_DEVICE
#define HW_MIDI_IN_UART1
#define HW_DAC_PIMORONI_PICO_AUDIO
#define HW_LED
#define HW_LED_7_SEG
#define HW_LCD_I2C
#define HW_LCD_I2C_ADDR 0x3E

#else

#error "Target hardware not specified"

#endif
