STB_VERSION = f7f20f39fe4f206c6f19e26ebfef7b261ee59ee4
CATCH2_VERSION = v3.6.0
CPP_VERSION = c++20

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
	g++ -std=$(CPP_VERSION) -o $@ -c $<

# NOTE check include statements in .cpp file
objects/src/main.o: src/pixel.hpp src/block.hpp src/png.hpp
objects/src/png.o: libraries/stb_image.h libraries/stb_image_write.h src/pixel.hpp src/block.hpp src/png.hpp
objects/src/block.o: src/pixel.hpp src/block.hpp
objects/libraries/catch_amalgamated.o: libraries/catch_amalgamated.hpp

bin/%: objects/src/%.o
	mkdir -p bin
	g++ -std=$(CPP_VERSION) -o $@ $^

# NOTE check include statements in .cpp file
bin/main: objects/src/pixel.o objects/src/block.o objects/src/png.o
bin/test_png: objects/libraries/catch_amalgamated.o objects/src/pixel.o objects/src/block.o objects/src/png.o
bin/test_block: objects/libraries/catch_amalgamated.o objects/src/pixel.o objects/src/block.o
bin/test_pixel: objects/libraries/catch_amalgamated.o objects/src/pixel.o

.PHONY: run
run: bin/main
	./bin/main ./input/black.png ./output/black.png

.PHONY: test
test: bin/test_pixel bin/test_block bin/test_png
	./bin/test_pixel
	./bin/test_block
	./bin/test_png

.PHONY: clean
clean:
	rm -rf libraries objects bin
