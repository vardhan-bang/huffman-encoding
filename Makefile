all: encode

test: encode
	./encode input.ppm

encode: main.c
	gcc -o encode main.c