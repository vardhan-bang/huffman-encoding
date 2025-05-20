all: encode

test: encode
	./encode file

encode: encode.c
	gcc -g -o encode encode.c

clean:
	rm encode