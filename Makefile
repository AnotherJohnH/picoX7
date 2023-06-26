
targets = rpipico \
          native

all: $(targets)

include Platform/build.make
