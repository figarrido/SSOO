DIR = src
CFLAGS = -I.

compile: $(DIR)/*.c
	gcc -o simulator $(DIR)/*.c $(CFLAGS)

clean:
	rm simulator
