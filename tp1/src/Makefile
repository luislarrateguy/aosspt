all: ../obj/rphone ../obj/rphoned

../obj/rphone: cliente.c
	gcc -o ../obj/rphone cliente.c

../obj/rphoned: servidor.c
	gcc -o ../obj/rphoned servidor.c

clean:
	rm -rf ../obj/rphone ../obj/rphoned

copy:
	cp ../* /home/soa/2007/tp1/UNO/ -r
