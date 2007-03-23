    /*
    ** cliente.c
    */

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <errno.h>
    #include <string.h>
    #include <netdb.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <sys/socket.h>

    #define DESTPORT 2222
    #define DESTADDR "0.0.0.0"

    #define MAXDATASIZE 100 

    int main(int argc, char *argv[])
    {
        int fd, cant;  
        char buf[MAXDATASIZE];
        struct sockaddr_in dest_addr; 		// información de la dirección de destino 

        if (argc != 3) {
		//Ayuda del uso
            fprintf(stderr,"Se usa: cliente SERVER_IP MSG\n");
            exit(1);
        }


        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        dest_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina 
        dest_addr.sin_port = htons(DESTPORT);  // short, Ordenación de bytes de la red 
        dest_addr.sin_addr.s_addr = htonl(inet_addr(argv[1]));
        memset(&(dest_addr.sin_zero),'\0', 8);  // poner a cero el resto de la estructura 

        if (connect(fd, (struct sockaddr *)&dest_addr,
                                              sizeof(struct sockaddr)) == -1) {
            perror("connect");
            exit(1);
        }
	if (send(fd, argv[2], 14, 0) == -1)
		perror("send");

        if ((cant=recv(fd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[cant] = '\0';

	// Imprimo el mensaje que me viene del servidor
        printf("El servidor dijo: %s",buf);

        close(fd);

        return 0;
    } 
