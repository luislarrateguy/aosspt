/*
 * Written by Der Herr Hofrat, der.herr@hofr.at
 * (C) 2002 FSMLabs
 * License: GPL Version 2
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> /* exit */
#include <sys/io.h>
#define LPT 0x378

int 
main(void)
{
	int in;
	if (ioperm(LPT, 3 , 1) < 0) {
		fprintf(stderr,"ioperm: error accessing to IO-ports. Root Privileges required");
		exit(-1);
	}

        /* wait for ACK to go low (logic 1) on the paralellport 
         * and then produce a HI/LOW on D0-D8 to trigger an
         * interrupt as response , see PLIP.txt for pin-out
         */
	outb(0xff,LPT);

	exit(0);
}
