CC := gcc
LDFLAGS :=
C_SOURCES := $(wildcard *.c)
C_EXECUTABLE := $(C_SOURCES:.c=)
LIB=-pthread

make:
		$(CC) $< $(C_SOURCES)  -o $(C_EXECUTABLE)
clean:
	rm -f *.o $(C_EXECUTABLE)
