# picoX7

![picox7](docs/picoX7.png)

A software simulation of the Yamaha DX7 with a USB and physical MIDI interface for the Raspberry Pi Pico

Slightly boring [video](https://youtu.be/ZSuNV67vHK8) of the picoX7 playing the Yamaha DX7 ROM cartridge 1A patches...

[![picoX7 rom1A](http://img.youtube.com/vi/7_5bl6q7xVs/0.jpg)](https://youtu.be/ZSuNV67vHK8)

## Status

Done...
   + I2S (via PIO) 16-bit stereo DAC interface at 49096 Hz
   + Use of the 2nd Arm Cortex-M, 8 voices per core
   + MIDI in support via UART and USB (including SYSEX in)
   + YM21280 OPS simulation (logarithmic)
   + YM21290 EGS simulation
   + Firmware simulation including pitch envelope, LFO, key sensitivity, level scaling, amplitude modulation
   + DX7 voice patches sounding close

To do...
   - Tune EGS simulation
   - Rate scaling
   - Portamento and glissando
   - Optimise to squeeze in more voices (?)

Limitations...
   - DAC implementation is quite different to the original. Dynamic range might be reduced?
   - Hitachi 6303 (~MC6803) CPU controller and firmware are not being emulated or fully simulated
   - 8 voices are on the left channel and 8 on the right ... stereo output needs to mixed to mono externally

Deviations/enhancements compared to a real DX7...
   + Each voice has it's own LFO
   + Each voice has it's own patch state (in theory when supported could load a different patch into each voice)
   + Support for 128 voice patches (DX7 had 32 internal and 32 cartridge)

See the [software blog](https://github.com/AnotherJohnH/picoX7/wiki/Software-Blog) and [hardware blog](https://github.com/AnotherJohnH/picoX7/wiki/Hardware-Blog)  for more detailed progress updates.

## Hardware

![prototype](https://github.com/AnotherJohnH/Hardware/raw/main/picoX7/docs/breadboard_v0.04.jpg)

[Schematic](https://github.com/AnotherJohnH/Hardware/blob/main/picoX7/docs/schematic_v0.05.pdf) for the above.

For more information see [README](https://github.com/AnotherJohnH/Hardware/blob/main/picoX7/README.md) and the [hardware blog](https://github.com/AnotherJohnH/picoX7/wiki/Hardware-Blog)

The Pico is significantly overclocked to 191.08 MHz to support 16 voices and generate a jitter free sample rate of 49096 Hz.

RP2350 Picos as well as the older RP2040 Picos are supported.

## Software

### Checkout

This repo uses git sub-modules, so checkout using --recurse to clone all the
dependent source...

    git clone --recurse https://github.com/AnotherJohnH/picoX7.git

or

    git clone --recurse ssh://git@github.com/AnotherJohnH/picoX7.git

### Software dependencies

+ https://github.com/AnotherJohnH/Platform
+ arm-none-eabi-gcc
+ cmake via UNIX make or auto detection of ninja if installed
+ Python3
+ SDL2 (only for native test target)

### Build

Being developed on MacOS but should build on Linux too.

Indirect build of all supported targets, rpipico, rpipico2 and native with cmake and make (or ninja)...

    make

Build a single hardware target e.g. rpipico2 using cmake...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=rp2350 -DCMAKE_TOOLCHAIN_FILE=Platform/MTL/rp2350/target/toolchain.cmake ..
    make

flashable images will be found under the build sub-directory here...

    build/<target>/picoX7_RPIPIOC_I2S_DAC.uf2
    build/<target>/picoX7_RPIPIOC_PWM_DAC.uf2
    build/<target>/picoX7_RPIPIOC_PIMORONI_PICO_AUDIO.uf2
    build/<target>/picoX7_RPIPIOC_WAVESHARE_REV2_1.uf2
    build/<target>/picoX7_PIMORONI_VGA_DEMO.uf2

Build the native target...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=native ..
    make

a runable binary image will be found under the build subdirectory here...

    build/Source/picoX7_NATIVE

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgements

 + [Ken Shirriff](https://github.com/shirriff) for his very helpful series of [blogs](https://www.righto.com/2021/11/reverse-engineering-yamaha-dx7.html) reverse engineering the YM21280
 + [AJXS](https://github.com/ajxs) for their annotated [disassembly](https://ajxs.me/blog/Yamaha_DX7_Firmware_ROM_Disassembly.html) of the DX7 firmware ROM
 + The [Dexed](https://asb2m10.github.io/dexed) developers for providing an easily accessible DX7 audio reference
 + The raspberry-pi foundation for the innovative [RP2040](https://www.raspberrypi.com/documentation/microcontrollers/rp2040.html)
 + The engineers at [Yamaha](https://www.yamaha.com/en/about/design/synapses/id_009)
