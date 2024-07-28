.PHONY: default
default: bin/main

libraries/stb_image.h:
	mkdir -p libraries
	curl -o libraries/stb_image.h https://raw.githubusercontent.com/nothings/stb/f7f20f39fe4f206c6f19e26ebfef7b261ee59ee4/stb_image.h

libraries/catch_amalgamated.hpp:
	mkdir -p libraries
	curl -o libraries/catch_amalgamated.hpp https://raw.githubusercontent.com/catchorg/Catch2/v3.6.0/extras/catch_amalgamated.hpp

libraries/catch_amalgamated.cpp:
	mkdir -p libraries
	curl -o libraries/catch_amalgamated.cpp https://raw.githubusercontent.com/catchorg/Catch2/v3.6.0/extras/catch_amalgamated.cpp

objects/catch_amalgamated.o: libraries/catch_amalgamated.hpp libraries/catch_amalgamated.cpp
	mkdir -p objects
	g++ -std=c++20 -o objects/catch_amalgamated.o -c libraries/catch_amalgamated.cpp

objects/main.o: libraries/stb_image.h utils.hpp main.cpp
	mkdir -p obejects
	g++ -std=c++20 -o objects/main.o -c main.cpp

objects/utils.o: utils.hpp utils.cpp
	mkdir -p obejects
	g++ -std=c++20 -o objects/utils.o -c utils.cpp

objects/test.o: libraries/catch_amalgamated.hpp utils.hpp test.cpp
	mkdir -p obejects
	g++ -std=c++20 -o objects/test.o -c test.cpp

bin/main: objects/main.o
	mkdir -p bin
	g++ -std=c++20 -o bin/main objects/utils.o objects/main.o

bin/test: objects/catch_amalgamated.o objects/utils.o objects/test.o
	mkdir -p bin
	g++ -std=c++20 -o bin/test objects/catch_amalgamated.o objects/utils.o objects/test.o

.PHONY: run
run: bin/main
	./bin/main ./pictures/black.png

.PHONY: test
test: bin/test
	./bin/test

.PHONY: clean
clean:
	rm -rf libraries objects bin
