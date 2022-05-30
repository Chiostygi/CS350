CC=gcc
CFLAGS=-I.
OBJ_CLIENT = echo_client.o
OBJ_SERVER = echo_server.o

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

make: echo_client echo_server

echo_client: $(OBJ_CLIENT)
	$(CC) -o $@ $^ $(CFLAGS)
echo_server: $(OBJ_SERVER)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o echo_client
	rm -f *.o echo_server