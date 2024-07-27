default: out/main

libraries/stb_image.h:
	mkdir -p libraries
	curl -o libraries/stb_image.h https://raw.githubusercontent.com/nothings/stb/f7f20f39fe4f206c6f19e26ebfef7b261ee59ee4/stb_image.h

out/main: libraries/stb_image.h main.cpp
	mkdir -p out
	g++ -std=c++20 -o out/main main.cpp