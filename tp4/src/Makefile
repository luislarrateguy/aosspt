all: monitor.o minix.o

include ./rtl.mk

monitor: monitor.c
	$(CC) ${INCLUDE} -Wall -O2 -o monitor monitor.c

minix: minix.c
	$(CC) ${INCLUDE} -Wall -O2 -o minix minix.c

copy:
	mkdir -p /home/soa/2007/tp4/UNO/
	cp -r ../* /home/soa/2007/tp3/UNO/

clean:
	rm -f *.o
