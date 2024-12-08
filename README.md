# pic2ascii

pic2ascii converts a line art into ascii art. Only PNG format is currently supported.

## Usage

```sh
git clone https://github.com/kitta65/pic2ascii.git
cd pic2ascii
make FLAG=-DPIC2ASCII_RELEASE
./bin/main path/to/input/image.png path/to/output/image.png --block_width=8 --transparent
```

## Options

||description|
|---|---|
|--block_width=N|N is the width of each character drawn.|
|--transparent|Background color becomes transparent.|

## Samples

|options|output|
|---|---|
|(original)|<img src="./input/sample.png" width="300">|
|--block_width=8|<img src="./output/sample_8.png" width="300">|
|--block_width=16|<img src="./output/sample_16.png" width="300">|
