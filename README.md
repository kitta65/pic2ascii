# pic2ascii

> [!warning]
> This project is still a work in progress!

pic2ascii converts a line art into ascii art. Only PNG format is currently supported.

## Usage

```sh
git clone https://github.com/kitta65/pic2ascii.git
cd pic2ascii
make FLAG=-DPIC2ASCII_RELEASE
./bin/main path/to/input/image.png path/to/output/image.png --block_width=8 --transparent
```

## Options

|option|description|
|---|---|
|--block_width=N|N is the width of each character drawn.|
|--transparent|Background color becomes transparent.|

## Sample

|input|output|
|---|---|
|![input](./input/sample.png)|![output](./output/sample.png)|
