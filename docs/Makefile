
CXXFLAGS=-Wall -Wno-gnu-anonymous-struct -Wno-nested-anon-types \
         --pedantic --std=gnu++14

SOURCE=exp_dx7.cpp Table_dx7_rom_1.cpp
BINARY=exp_dx7

$(BINARY): $(SOURCE) DX7_v1_8.h Table_dx7_rom_1.h
	gcc $(CXXFLAGS) $(SOURCE) -o $(BINARY)

clean:
	rm $(BINARY)
