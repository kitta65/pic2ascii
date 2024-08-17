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
	mkdir -p objects/libraries
	g++ -std=$(CPP_VERSION) -o $@ -c $<

objects/libraries/catch_amalgamated.o: libraries/catch_amalgamated.hpp

objects/main.o: libraries/stb_image.h utils.hpp

objects/block.o: pixel.hpp

objects/utils.o: utils.hpp

objects/test.o: libraries/catch_amalgamated.hpp

bin/%: objects/%.o
	mkdir -p bin
	g++ -std=$(CPP_VERSION) -o $@ $^

bin/main: objects/utils.o

bin/test: objects/libraries/catch_amalgamated.o objects/block.o objects/pixel.o objects/utils.o

.PHONY: run
run: bin/main
	./bin/main ./pictures/black.png

.PHONY: test
test: bin/test
	./bin/test

.PHONY: clean
clean:
	rm -rf libraries objects bin
