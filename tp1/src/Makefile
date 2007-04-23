DISQUETTE_DIR=./temp-disquette
IMAGEN_D=./imagenDisquette
MOUNT_DIR=./temp-mount-disquette

SRC=datos.h cliente.c servidor.c strings.c Makefile
OBJ=../obj/rphone ../obj/rphoned

all: ${OBJ}

../obj/rphone: cliente.c
	gcc -o ../obj/rphone cliente.c

../obj/rphoned: servidor.c strings.c
	gcc -o ../obj/rphoned servidor.c

${OBJ}: datos.h

doc:
	cd ../doc && ./compilarPDF

clean:
	rm -rf ${OBJ}

# Arma un directorio temporal con el contenido del disquette.
disquette_dir: all doc
	rm -rf ${DISQUETTE_DIR}
	mkdir ${DISQUETTE_DIR}

	# Copio el código fuente
	mkdir ${DISQUETTE_DIR}/src
	cp ${SRC} ${DISQUETTE_DIR}/src

	# Copio los binarios
	mkdir ${DISQUETTE_DIR}/obj
	cp ${OBJ} ${DISQUETTE_DIR}/obj

	# Copio la documentación
	mkdir ${DISQUETTE_DIR}/doc
	cp ../doc/main.pdf ${DISQUETTE_DIR}/doc/documentacion.pdf

# Copia todos los datos del disquette a la ubicación especificada en el
# enunciado. Este comando sólo tiene utilidad para ser ejecutado por el
# profesor, y no los integrantes del grupo.
copy:
	# Creo el arbol de directorios y copio el contenido del disquette
	mkdir -p /home/soa/2007/tp1/UNO/
	cp -r ../* /home/soa/2007/tp1/UNO/

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
