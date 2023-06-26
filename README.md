# picoX7

Retro FM synthesiser simulation using the Raspberry Pi Pico

## Description

A software simulation of the Yamaha DX7 audio hardware with a MIDI interface for
the Raspberry Pi Pico.

## Software dependencies

+ Platform (https://github.com/AnotherJohnH/Platform)
+ arm-none-eabi-gcc
+ cmake
+ ninja (make works too)
+ SDL2 (for native target)

## Hardware information

+ Currently building for an I2S DAC based around the Cirrus Logic CS4344 module
(https://thepihut.com/products/pico-audio-audio-module-for-raspberry-pi-pico-inc-speakers)
Works well but notice Cirrus Logic have discontinued this device. Should be easy to switch
to another I2S DAC.
+ The Pico is overclocked to 137.48 MHz to avoid jitter when driving the I2S chip
with a sample frequency of 49100 Hz
+ UART-0 used as a debug console
+ UART-1 implements the MIDI interface

## Status

Very early stages of development.

Done...
   + I2S (via PIO) 16-bit stereo DAC interface at 49100 Hz
   + Basic MIDI in support via UART
   + Voice allocator
   + YM21280 OPS simulation

To do, (at least)...
   - Test accuracy of OPS simulation
   - YM21290 EGS simulation
   - YM21290 LFO simulation
   - SysEx program storage and integration
   - Logarithmic OPS implementation

Limitations...
   + DAC implementation is quite different to the original. Suspect dynamic range is reduced.
   + Sample frequency of original is 49096 Hz not 49100 Hz
   + MC6800 CPU controller and firmware is not being emulated
   + not using logs for sine amplitude modulation in the OPS so not bit accurate
   + 8 voices not 16

## How to build

### Checkout

This repo uses git sub-modules, so checkout using --recurse to clone all the
dependent source...

    git clone --recurse https://github.com/AnotherJohnH/picoX7.git

or

    git clone --recurse ssh://git@github.com/AnotherJohnH/picoX7.git

### Build

Being developed on MacOS but should build fine on Linux too.

Indirect build of all supported targets (with cmake and ninja)...

    make

Build directly using cmake and make...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=rpipico -DCMAKE_TOOLCHAIN_FILE=Platform/MTL/rpipico/toolchain.cmake ..
    make

FLASHable image will be found under build sub directory here...

    .../Source/picoX7.uf2

## Thanks and acknowledgements

Ken Shirrif for his very helpful YM21280 reverse engineering blog series
(https://www.righto.com/2021/11/reverse-engineering-yamaha-dx7.html)
