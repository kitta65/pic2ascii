STB_VERSION = f7f20f39fe4f206c6f19e26ebfef7b261ee59ee4
CATCH2_VERSION = v3.6.0
CPP_VERSION = c++20
FLAG = -DPIC2ASCII_DEBUG

.PHONY: default
default: bin/main

libraries:
	mkdir libraries

libraries/stb_image.h: libraries
	curl -o $@ https://raw.githubusercontent.com/nothings/stb/$(STB_VERSION)/stb_image.h

libraries/stb_image_write.h: libraries
	curl -o $@ https://raw.githubusercontent.com/nothings/stb/$(STB_VERSION)/stb_image_write.h

libraries/catch_amalgamated.hpp: libraries
	curl -o $@ https://raw.githubusercontent.com/catchorg/Catch2/$(CATCH2_VERSION)/extras/catch_amalgamated.hpp

libraries/catch_amalgamated.cpp: libraries
	curl -o $@ https://raw.githubusercontent.com/catchorg/Catch2/$(CATCH2_VERSION)/extras/catch_amalgamated.cpp

src/matrix.hpp: src/xy.hpp
src/block.hpp: src/xy.hpp src/matrix.hpp
src/png.hpp: src/xy.hpp src/matrix.hpp
src/main.hpp: src/block.hpp src/png.hpp
src/test_matrix.hpp: libraries/catch_amalgamated.hpp
src/test_block.hpp: libraries/catch_amalgamated.hpp src/xy.hpp src/matrix.hpp src/block.hpp
src/test_png.hpp: libraries/catch_amalgamated.hpp src/xy.hpp src/matrix.hpp src/block.hpp src/png.hpp
src/test_other.hpp: libraries/catch_amalgamated.hpp src/xy.hpp src/matrix.hpp src/block.hpp src/png.hpp

objects/%.o: %.cpp %.hpp
	mkdir -p objects/src
	mkdir -p objects/libraries
	g++ $(FLAG) -std=$(CPP_VERSION) -o $@ -c $< -Wall

# NOTE
# this is exception.
# one cpp file should include one hpp file.
objects/src/png.o: libraries/stb_image.h libraries/stb_image_write.h

bin/%: objects/src/%.o
	mkdir -p bin
	g++ -std=$(CPP_VERSION) -o $@ $^

bin/main: objects/src/block.o objects/src/png.o
bin/test_matrix: objects/libraries/catch_amalgamated.o
bin/test_block: objects/libraries/catch_amalgamated.o objects/src/block.o
bin/test_png: objects/libraries/catch_amalgamated.o objects/src/block.o objects/src/png.o
bin/test_other: objects/libraries/catch_amalgamated.o objects/src/block.o objects/src/png.o

.PHONY: run
run: bin/main
	./bin/main ./input/sample.png ./output/makefile_run.png --block_width=8

.PHONY: test
test: bin/test_matrix bin/test_block bin/test_png bin/test_other
	./bin/test_matrix
	./bin/test_block
	./bin/test_png
	./bin/test_other

.PHONY: clean
clean:
	rm -rf libraries objects bin
