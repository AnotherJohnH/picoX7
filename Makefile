
targets = rp2040 rp2350 native

all: $(targets)

include Platform/build.make
