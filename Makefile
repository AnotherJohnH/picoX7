
targets = rpipico \
          mbedLPC1768 \
          native

all: $(targets)

include Platform/build.make
