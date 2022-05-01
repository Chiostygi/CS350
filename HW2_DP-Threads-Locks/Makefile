CC=gcc
CFLAGS=-I.
DEPS = meal.h kitchen.c
OBJ = kitchen.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kitchen: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
make:
	kitchen
clean:
	rm -f *.o kitchen

