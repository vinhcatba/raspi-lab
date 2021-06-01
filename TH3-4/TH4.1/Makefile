LIBS  = -lm -lrt -pthread
CFLAGS = -Wall

all: no-thread thread

no-thread:
	gcc -o no-thread no-thread.c $(CFLAGS) $(LIBS)
thread:
	gcc -o thread thread.c $(CFLAGS) $(LIBS)
clean:
	rm -rf thread no-thread 
