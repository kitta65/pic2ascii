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

objects/%.o: %.cpp
	mkdir -p objects/src
	mkdir -p objects/libraries
	g++ $(FLAG) -std=$(CPP_VERSION) -o $@ -c $< -Wall

# NOTE check include statements in .cpp file
objects/src/main.o: src/xy.hpp src/matrix.hpp src/block.hpp src/png.hpp src/main.hpp
objects/src/png.o: libraries/stb_image.h libraries/stb_image_write.h src/xy.hpp src/matrix.hpp src/block.hpp src/png.hpp
objects/src/block.o: src/xy.hpp src/matrix.hpp src/block.hpp
objects/src/test_matrix.o: libraries/catch_amalgamated.hpp src/xy.hpp src/matrix.hpp
objects/src/test_block.o: libraries/catch_amalgamated.hpp src/xy.hpp src/matrix.hpp src/block.hpp
objects/src/test_png.o: libraries/catch_amalgamated.hpp src/xy.hpp src/matrix.hpp src/block.hpp src/png.hpp
objects/src/test_other.o: libraries/catch_amalgamated.hpp src/xy.hpp src/matrix.hpp src/block.hpp src/png.hpp
objects/libraries/catch_amalgamated.o: libraries/catch_amalgamated.hpp
objects/stats_ssim.o: src/xy.hpp src/matrix.hpp src/block.hpp

bin/%: objects/src/%.o
	mkdir -p bin
	g++ -std=$(CPP_VERSION) -o $@ $^

# NOTE check include statements in .cpp file
bin/main: objects/src/block.o objects/src/png.o
bin/test_other: objects/libraries/catch_amalgamated.o objects/src/block.o objects/src/png.o
bin/test_png: objects/libraries/catch_amalgamated.o objects/src/block.o objects/src/png.o
bin/test_block: objects/libraries/catch_amalgamated.o objects/src/block.o
bin/test_matrix: objects/libraries/catch_amalgamated.o
bin/stats_ssim: objects/src/block.o

.PHONY: run
run: bin/main
	./bin/main ./input/sample.png ./output/makefile_run.png --block_width=8

.PHONY: test
test: bin/test_matrix bin/test_block bin/test_png bin/test_other
	./bin/test_matrix
	./bin/test_block
	./bin/test_png
	./bin/test_other

.PHONY: stats
stats: bin/stats_ssim
	./bin/stats_ssim

.PHONY: clean
clean:
	rm -rf libraries objects bin
