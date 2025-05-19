all: encode

test: encode
	./encode input.ppm

encode: encode.c
	gcc -o encode encode.c