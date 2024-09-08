
all: rpipico rpipico2 native

send: rpipico
	cp build_rpipico/Source/picoX7_WAVESHARE_GPIO_LCD.uf2 /Volumes/RPI-RP2

include Platform/build.make
