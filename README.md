# picoX7

![picox7](docs/picoX7.png)

A software simulation of the Yamaha DX7 with a USB and physical MIDI interface for the Raspberry Pi Pico

Slightly boring [video](https://youtu.be/ZSuNV67vHK8) of the picoX7 playing the Yamaha DX7 ROM cartridge 1A patches...

[![picoX7 rom1A](http://img.youtube.com/vi/7_5bl6q7xVs/0.jpg)](https://youtu.be/ZSuNV67vHK8)

## Status

Done...
   + I2S (via PIO) 16-bit stereo DAC interface at 49096 Hz
   + Use of the 2nd Arm Cortex-M, 4 voices per core
   + MIDI in support via UART and USB (including SYSEX in)
   + YM21280 OPS simulation (logarithmic)
   + YM21290 EGS simulation
   + Firmware simulation including pitch envelope, LFO, key sensitivity, level scaling, amplitude modulation
   + DX7 voice patches sounding close

To do...
   - Tune EGS simulation
   - Rate scaling
   - Portamento and glissando
   - Optimise to squeeze in more voices

Limitations...
   - DAC implementation is quite different to the original. Dynamic range might be reduced?
   - Hitachi 6303 (~MC6803) CPU controller and firmware is not being emulated or fully simulated
   - [8 voices](https://github.com/AnotherJohnH/picoX7/wiki/Software-Blog#16th-july-2023) (not 16!) - Running on two arm cores at ~85% CPU usage
   - 4 voices are on the left channel and 4 on the right ... stereo output needs to mixed to mono externally

Deviations/enhancements compared to a real DX7...
   + Each voice has it's own LFO
   + Each voice has it's own patch state (in theory when supported could load a different patch into each voice)
   + Support for 128 voice patches (DX7 had 32 internal and 32 cartridge)

See the [software blog](https://github.com/AnotherJohnH/picoX7/wiki/Software-Blog) and [hardware blog](https://github.com/AnotherJohnH/picoX7/wiki/Hardware-Blog)  for more detailed progress updates.

## Software dependencies

+ https://github.com/AnotherJohnH/Platform
+ arm-none-eabi-gcc
+ cmake via UNIX make or auto detection of ninja if installed
+ Python3
+ SDL2 (only for native test target)

Both Pico and native targets should build on MacOS or Linux.

## Hardware information

![prototype](docs/breadboard_v0.04.jpg)

For more information on the circuit above see  the [schematic](https://github.com/AnotherJohnH/picoX7/blob/main/docs/schematic_v0.05.pdf) and the [hardware blog](https://github.com/AnotherJohnH/picoX7/wiki/Hardware-Blog)

+ Currently building for an I2S DAC based around the Cirrus Logic CS4344 from WaveShare
(see below)
Works well but notice Cirrus Logic have discontinued this device. Should be easy to switch
to another I2S DAC
+ The Pico is significantly overclocked to 191.08 MHz to support 8 voices and generate a jitter free sample rate of 49096 Hz.
+ UART-0 used as a debug console  (115200 baud)
+ UART-1 (RX) implements the physical MIDI-IN interface
+ Two PIO state machines are used to generate I2S and MCLK for the DAC module

|Pin|Peripheral|Allocation|
|---|---|---|
|1|UART-0|Debug console out (UART) TX|
|2|UART-0|Debug console in (UART) RX|
|3|GND|UART GND|
|6|UART-1|MIDI IN|
|24|I2C1|SDA (for I2C LCD)|
|25|I2C1|SCL (for I2C LCD)|
|31|PIO|I2S DATA|
|32|PIO|I2S DATA CLK|
|33|PIO|I2S LR CLK|

### Hardware targets

Software builds for the following hardware targets...
+ PIMORONI_PICO_AUDIO - Use the Pimoroni Pico Audio I2S DAC
+ I2S_DAC - Any I2S DAC supporting stereo 16-bit samples at 49 KHz (no MCLK)
+ PWM_DAC - DAC implemented using the on-chip PWM and a few external resistors and capacitors on pins 21 and 22 (sound quality is poor)
+ WAVESHARE_REV2_1 - For the Waveshare I2S DAC (Rev2.1) with I2C LCD on pins 19+20
+ PIMORONI_VGA_DEMO - Support for the Pimoroni Pico VGA Demo Base (!!!! untested !!!!)

NOTE: The LCD and LED displays are optional and will not block operation if not fitted

In addition the above targets are built for and work with RP2350 Picos as well as the older RP2040 Picos.

### Components

|Name|Description|Source|
|---|---|---|
|Raspberry Pi Pico|DIP module hosting an RP2040 MCU and 2 MiB Flash|[PiHut](https://thepihut.com/products/raspberry-pi-pico?variant=41925332566211)|
|Waveshare Pico Audio|I2S DAC module that directly mounts under the Pico|[PiHut](https://thepihut.com/products/pico-audio-audio-module-for-raspberry-pi-pico-inc-speakers)|
|MIDI controller|Music keyboard with actual MIDI out (MIDI over the USB connector also works)|Old Roland PC-200 works for me|
|MIDI-in adapter|An optically isolated 5-pin DIN to UART interface|3 resistors, a diode and an op-amp work well if the controller is battery powered ;-)|
|Reset button|Reset the Pico and allow Flashing|Why not included on the Pico module ?!@#|
|16x2 I2C LCD|Optional voice name display|[PiHut](https://thepihut.com/products/lcd1602-i2c-module)|
|Raspberry Pi Debug probe|Not essential but very helpful|[PiHut](https://thepihut.com/products/raspberry-pi-debug-probe)|

**NOTE** No particular affiliation with PiHut but they have a nice selection and excellent service

## How to build

### Checkout

This repo uses git sub-modules, so checkout using --recurse to clone all the
dependent source...

    git clone --recurse https://github.com/AnotherJohnH/picoX7.git

or

    git clone --recurse ssh://git@github.com/AnotherJohnH/picoX7.git

### Build

Being developed on MacOS but should build on Linux too.

Indirect build of all supported targets, rpipico, rpipico2 and native with cmake and make (or ninja)...

    make

Build a single hardware target e.g. rpipico2 using cmake...

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DPLT_TARGET=rpipico2 -DCMAKE_TOOLCHAIN_FILE=Platform/MTL/rpipico2/toolchain.cmake ..
    make

flashable images will be found under the build sub-directory here...

    build/picoX7_I2S_DAC.uf2
    build/picoX7_PWM_DAC.uf2
    build/picoX7_PIMORONI_PICO_AUDIO.uf2
    build/picoX7_PIMORONI_VGA_DEMO.uf2
    build/picoX7_WAVESHARE_REV2_1.uf2

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
