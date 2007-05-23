/*

  Copyright (c) 2007 by Luis I. Larrateguy, Milton Pividori y
  César Sandrigo. All Rights Reserved

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include <string.h>
#include "queue/queue.h"
#include "datos.h"


/* Retorna una estructura 'puertos' con el numero de puerto
 * de cada mutex y el de su padre, leidos del archivo 'mutex.cfg' */ 
struct puertos leerPuertos () {
	FILE* archivo;
	char linea[TAM_LINEA];
	char* puerto;
	char* puertoPadre;
	struct puertos resultado;
	int i = 0;

	/* Abrimos el archivo */
	if ((archivo = fopen(RUTA_MUTEX_CNF, "r")) == NULL)
		fatal("No se pudo abrir el archivo /etc/mutexd.conf\n");
	
	while (!feof(archivo) && (i < CANT_MUTEXD))  {
		/* Leemos la linea */
		fgets(linea, TAM_LINEA, archivo);

		/* Quito el fin de linea ('\n') de la línea leída */
		linea[strlen(linea) - 1] = '\0';

		puerto = strtok(linea, " ");
		puertoPadre = strtok(NULL, "\0");
		
		if (puerto != NULL && puertoPadre != NULL) {
			resultado.puerto[i][0] = atoi(puerto);
			resultado.puerto[i][1] = atoi(puertoPadre);
		}

		i++;
	}
	
	/* Cerramos el archivo */
	if (fclose(archivo) == EOF)
		fatal("Error al cerrar el archivo\n");
	
	return resultado;
}


int main(int argc, char* argv[]) {
	return 0;
}

