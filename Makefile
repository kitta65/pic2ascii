STB_VERSION = f7f20f39fe4f206c6f19e26ebfef7b261ee59ee4
CATCH2_VERSION = v3.6.0
CPP_VERSION = c++20

.PHONY: default
default: bin/main

libraries:
	mkdir libraries

libraries/stb_image.h: libraries
	curl -o libraries/stb_image.h https://raw.githubusercontent.com/nothings/stb/$(STB_VERSION)/stb_image.h

libraries/catch_amalgamated.hpp: libraries
	curl -o libraries/catch_amalgamated.hpp https://raw.githubusercontent.com/catchorg/Catch2/$(CATCH2_VERSION)/extras/catch_amalgamated.hpp

libraries/catch_amalgamated.cpp: libraries
	curl -o libraries/catch_amalgamated.cpp https://raw.githubusercontent.com/catchorg/Catch2/$(CATCH2_VERSION)/extras/catch_amalgamated.cpp

objects:
	mkdir objects

objects/catch_amalgamated.o: objects libraries/catch_amalgamated.hpp libraries/catch_amalgamated.cpp
	g++ -std=$(CPP_VERSION) -o objects/catch_amalgamated.o -c libraries/catch_amalgamated.cpp

objects/main.o: objects libraries/stb_image.h utils.hpp main.cpp
	g++ -std=$(CPP_VERSION) -o objects/main.o -c main.cpp

objects/utils.o: objects utils.hpp utils.cpp
	g++ -std=$(CPP_VERSION) -o objects/utils.o -c utils.cpp

objects/test.o: objects libraries/catch_amalgamated.hpp utils.hpp test.cpp
	g++ -std=$(CPP_VERSION) -o objects/test.o -c test.cpp

bin:
	mkdir bin

bin/main: bin objects/main.o objects/utils.o
	g++ -std=$(CPP_VERSION) -o bin/main objects/utils.o objects/main.o

bin/test: bin objects/catch_amalgamated.o objects/utils.o objects/test.o
	g++ -std=$(CPP_VERSION) -o bin/test objects/catch_amalgamated.o objects/utils.o objects/test.o

.PHONY: run
run: bin/main
	./bin/main ./pictures/black.png

.PHONY: test
test: bin/test
	./bin/test

.PHONY: clean
clean:
	rm -rf libraries objects bin
