# Makefile

CC=gcc

all: ../obj/mutexd ../obj/mutexloop

../obj/mutexd: queue/queue.o common.o mutexd.o
	$(CC) queue/queue.o common.o mutexd.o -o ../obj/$@

../obj/mutexloop: mutexloop.o common.o
	$(CC) common.o mutexloop.o -o ../obj/$@

clean:
	rm -f *.o queue/*.o ../obj/mutexd ../obj/mutexloop

copy:
	mkdir -p /home/soa/2007/tp3/UNO/
	cp -r ../* /home/soa/2007/tp3/UNO/

