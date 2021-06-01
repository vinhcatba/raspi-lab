LIBS  = -lm -lrt -pthread
CFLAGS = -Wall

all: client server

client:
	gcc -o client client.c $(CFLAGS) $(LIBS)
server:
	gcc -o server server.c $(CFLAGS) $(LIBS)
clean:
	rm -rf client server
