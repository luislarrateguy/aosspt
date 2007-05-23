# Makefile

CC=gcc

all: mutexd mutexloop

mutexd: queue/queue.o queue/queue.h mutexd.o
	$(CC) queue/queue.o mutexd.o -o $@

mutexloop: mutexloop.o
	$(CC) mutexloop.o -o $@

clean:
	rm -f *.o queue/*.o mutexd mutexloop

