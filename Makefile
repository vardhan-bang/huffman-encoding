all: encode

test: encode
	./encode input.ppm

encode: encode.c
	gcc -g -o encode encode.c

clean:
	rm encode