default: out/main

libraries/stb_image.h:
	curl -o libraries/stb_image.h https://raw.githubusercontent.com/nothings/stb/f7f20f39fe4f206c6f19e26ebfef7b261ee59ee4/stb_image.h

out/main: libraries/stb_image.h main.cpp
	g++ -o out/main main.cpp