CC=gcc
CFLAGS=-I.
OBJ_CLIENT = client.o
OBJ_SERVER = server.o

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

make: client server

client: $(OBJ_CLIENT)
	$(CC) -o $@ $^ $(CFLAGS)
server: $(OBJ_SERVER)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o client
	rm -f *.o server