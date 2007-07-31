
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> /* exit */
#include <sys/io.h>
#define LPT 0x3bc

int 
main(void)
{
	int in, i=0;
	if (ioperm(LPT, 3 , 1) < 0) {
		fprintf(stderr,"ioperm: error accessing to IO-ports. Root Privileges required");
		exit(-1);
	}

        /* wait for ACK to go low (logic 1) on the paralellport 
         * and then produce a HI/LOW on D0-D8 to trigger an
         * interrupt as response , see PLIP.txt for pin-out
         */
	while(i<500) {
		outb(0x0,LPT);    /* logic 1 on LPT*/
		i++;
  	}
	exit(0);
}
