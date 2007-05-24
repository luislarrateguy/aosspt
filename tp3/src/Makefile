# Makefile

CC=gcc

all: mutexd mutexloop

mutexd: queue/queue.o common.o mutexd.o
	$(CC) queue/queue.o common.o mutexd.o -o $@

mutexloop: mutexloop.o
	$(CC) common.o mutexloop.o -o $@

clean:
	rm -f *.o queue/*.o mutexd mutexloop

