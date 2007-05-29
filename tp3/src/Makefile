# Makefile
DISQUETTE_DIR=./temp-disquette
IMAGEN_D=./imagenDisquette
MOUNT_DIR=./temp-mount-disquette

CC=gcc
SRC=common.h common.c Makefile mutexd.c mutexloop.c
QUEUE_SRC=queue/queue.h queue/queue.c queue/testQueue.c
OBJ=../obj/mutexd ../obj/mutexloop
ETC=../etc/mutex.cfg

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

doc:
	cd ../doc && ./compilarPDF

# Arma un directorio temporal con el contenido del disquette.
disquette_dir: all doc
	rm -rf ${DISQUETTE_DIR}
	mkdir ${DISQUETTE_DIR}

	# Copio el código fuente del TP
	mkdir ${DISQUETTE_DIR}/src
	cp ${SRC} ${DISQUETTE_DIR}/src

	# Copio el código fuente de la cola
	mkdir ${DISQUETTE_DIR}/src/queue
	cp ${QUEUE_SRC} ${DISQUETTE_DIR}/src/queue

	# Copio los binarios
	mkdir ${DISQUETTE_DIR}/obj
	cp ${OBJ} ${DISQUETTE_DIR}/obj

	# Copio el archivo de configuración
	mkdir ${DISQUETTE_DIR}/etc
	cp ${ETC} ${DISQUETTE_DIR}/etc

	# Copio la documentación
	mkdir ${DISQUETTE_DIR}/doc
	cp ../doc/main.pdf ${DISQUETTE_DIR}/doc/main_doc.pdf

# Crea una imagen de un disquette de 1.44 MB
imagen_disquette: disquette_dir
	# Creo el diretorio donde voy a montar la imagen
	rm -rf ${MOUNT_DIR}
	mkdir ${MOUNT_DIR}

	# Creo la imagen
	dd if=/dev/zero of=${IMAGEN_D} bs=1440k count=1

	# Le doy formato ext2
	yes s | mkfs.ext2 ${IMAGEN_D}

	# Monto la imagen un directorio temporal. El 'sudo' siguiente funciona
	# en Ubuntu, pero no se si en otras distros.
	sudo mount -o loop ${IMAGEN_D} ${MOUNT_DIR}

	# Copio los archivos del disquette a ese directorio de montaje
	cp -r ${DISQUETTE_DIR}/* ${MOUNT_DIR}

	# Desmonto la imagen del disquette
	sudo umount ${MOUNT_DIR}

	# Elimino el directorio de montaje y el del contenido del disquette
	rm -rf ${MOUNT_DIR}
	rm -rf ${DISQUETTE_DIR}

# Copia el contenido de la imagen del disquette a un disquette físico
disquette: imagen_disquette
	sudo cat ${IMAGEN_D} > /dev/fd0
	rm -rf ${IMAGEN_D}

