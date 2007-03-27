/*
* conexion.h
*/

#include "conexion.h"



int abrirSocket() {
	int fd;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		error("ERROR al abrir el Socket");
		exit(1);
	}
	return fd;
}
void inicializarDireccion(struct sockaddr_in *dir,long addr,int port) {
	bzero((char *) dir, sizeof(dir));
	dir->sin_family = AF_INET;
	dir->sin_addr.s_addr = addr;
	dir->sin_port = htons(port);
}
void conectar(int fd,struct sockaddr_in *addr) {
	if (bind(fd, (struct sockaddr *) addr,	sizeof(*addr)) < 0) {
		perror("bind");
		exit(1);
	}
}
void escuchar(int fd) {
	// Activo la ecucha de peticiones.
	if (listen(fd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
}
int aceptarRequest(int fd, int *nfd, struct sockaddr_in *addr) {
	int sin_size = sizeof(struct sockaddr_in);
	if ((*nfd = accept(fd, (struct sockaddr *) addr, &sin_size)) == -1) {
		perror("accept");
		return -1;
	}
	return 0;
}
void error(char *msg) {
	perror(msg);
	exit(1);
}

