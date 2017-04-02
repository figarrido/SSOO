DIR = src
CFLAGS = -Wall -I.

compile: $(DIR)/*.c
	@gcc -o simulator $(DIR)/*.c $(CFLAGS)
	@echo "Compilación completada"
clean:
	@rm simulator
	@echo "Ejecutable eliminado"
