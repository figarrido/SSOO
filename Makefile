DIR = src
CFLAGS = -Wall -I.

compile: $(DIR)/*.c
	gcc -o simulator $(DIR)/*.c $(CFLAGS)

clean:
	rm simulator
