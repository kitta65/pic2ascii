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

objects/libraries/catch_amalgamated.o: libraries/catch_amalgamated.hpp

objects/src/main.o: libraries/stb_image.h libraries/stb_image_write.h

objects/src/block.o: src/pixel.hpp

objects/src/test.o: libraries/catch_amalgamated.hpp

bin/%: objects/src/%.o
	mkdir -p bin
	g++ -std=$(CPP_VERSION) -o $@ $^

bin/test_pixel: objects/libraries/catch_amalgamated.o objects/src/pixel.o
bin/test_block: objects/libraries/catch_amalgamated.o objects/src/pixel.o objects/src/block.o

.PHONY: run
run: bin/main
	./bin/main ./input/black.png ./output/black.png

.PHONY: test
test: bin/test_pixel bin/test_block
	./bin/test_pixel
	./bin/test_block

.PHONY: clean
clean:
	rm -rf libraries objects bin
