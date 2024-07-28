STB_VERSION = f7f20f39fe4f206c6f19e26ebfef7b261ee59ee4
CATCH2_VERSION = v3.6.0
CPP_VERSION = c++20

.PHONY: default
default: bin/main

libraries:
	mkdir libraries

libraries/stb_image.h: libraries
	curl -o $@ https://raw.githubusercontent.com/nothings/stb/$(STB_VERSION)/stb_image.h

libraries/catch_amalgamated.hpp: libraries
	curl -o $@ https://raw.githubusercontent.com/catchorg/Catch2/$(CATCH2_VERSION)/extras/catch_amalgamated.hpp

libraries/catch_amalgamated.cpp: libraries
	curl -o $@ https://raw.githubusercontent.com/catchorg/Catch2/$(CATCH2_VERSION)/extras/catch_amalgamated.cpp

objects/%.o: %.cpp
	mkdir -p objects
	g++ -std=$(CPP_VERSION) -o $@ -c $<

objects/catch_amalgamated.o: libraries/catch_amalgamated.hpp 

objects/main.o: utils.hpp libraries/stb_image.h

objects/utils.o: utils.cpp utils.hpp

objects/test.o: libraries/catch_amalgamated.hpp

bin:
	mkdir bin

bin/main: bin objects/main.o objects/utils.o
	g++ -std=$(CPP_VERSION) -o $@ objects/utils.o objects/main.o

bin/test: bin objects/catch_amalgamated.o objects/utils.o objects/test.o
	g++ -std=$(CPP_VERSION) -o $@ objects/catch_amalgamated.o objects/utils.o objects/test.o

.PHONY: run
run: bin/main
	./bin/main ./pictures/black.png

.PHONY: test
test: bin/test
	./bin/test

.PHONY: clean
clean:
	rm -rf libraries objects bin
