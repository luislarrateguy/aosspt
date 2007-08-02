all: monitor.o minix.o

include ./rtl.mk

monitor: monitor.c
	$(CC) ${INCLUDE} -Wall -O2 -o monitor monitor.c

minix: minix.c
	$(CC) ${INCLUDE} -Wall -O2 -o minix minix.c

clean:
	rm -f *.o
