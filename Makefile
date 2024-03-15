CC=gcc
CFLAGS=-I.

verify: clean client

client:
	$(CC) -o client client.c requests.c helpers.c buffer.c cJSON.c -Wall

run: client
	./client
	
clean:
	rm -f *.o client client helpers.h.gch
