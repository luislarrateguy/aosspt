#include <rtl.h>
#include <time.h>
#include <pthread.h>
#include <linux/module.h>
#include <rtl_sched.h>
#include <asm/io.h>
#include <rtl_core.h>

#define LPT_PORT 0x378
MODULE_AUTHOR("Sistemas Operativos Avanzados");
MODULE_DESCRIPTION("Monitor");
MODULE_LICENSE("GPL");
static int frec = 1;   // frecuencia en Hz.
MODULE_PARM(frec,"i");   // especifica que frec es un parametro entero.

// irq del puerto paralelo (0x378/9/A)
#define IRQ	7	
#define MAX 502

pthread_t thread;
hrtime_t tiempo_ant, tiempo_fin, delta;
int i,a;
int error;
int j=MAX;
int mat[MAX];

char in_parallel_port(void)
{
	int n, res;

	n = inb(LPT_PORT+1);
	if (n & 0x80)		// si el bit 5 == 1...
	    n = n & 0x7F;	// lo ponemos a 0,
	else			// sino
	    n = n | 0x80;	// lo ponemos a 1 (logica invertida)
	res = ((n & 0x38)>>3)+((n & 0x80)>>4);	// arma el numero leido del 
					   // puerto con los bits 4,5,6 y 8.
	if (res == 0xF)            	   
	    return 'Q';	     // detectado fin de secuencia.
							
	return res + '0';
}


inline unsigned int intr_handler(unsigned int irq_num, struct pt_regs *regs)
{
        if (j == 0 ) { 
         return 0;
        }
	tiempo_fin = gethrtime();
	delta = (tiempo_fin - tiempo_ant) ;
        tiempo_ant = tiempo_fin;
	mat[j--] = delta;
        rtl_hard_enable_irq(IRQ);	    // habilita interrup. irq 7

        return 0;
}

void * start_routine(void *arg)
{
	struct sched_param p;
	long periodo = 1000000000 / frec;

	p . sched_priority = 1;
	pthread_setschedparam (pthread_self(), SCHED_FIFO, &p);
	pthread_make_periodic_np (pthread_self(), gethrtime(),  periodo); 

        tiempo_ant = 0; 
	for (j=MAX;j > 0;j--) {
	    outb(0xFF,LPT_PORT);
	    rtl_delay(1000);
	    outb(0x00,LPT_PORT);
	    pthread_wait_np ();
	}
	rtl_printf("Listo\n");
	return 0;
}

int init_module(void)
{
        tiempo_ant = 0;
	error = rtl_request_irq(IRQ, intr_handler); // instala el handler 
	if(error < 0) 				    // del irq 7.
	{		
	    rtl_printf("Error en rtl_request_irq (%d)\n", error);
	    return -1;
	}
	
	outb_p(inb_p(LPT_PORT + 2) | 0x10, LPT_PORT + 2); // habilita interrup. del puerto.
	rtl_hard_enable_irq(IRQ);	    // habilita interrup. irq 7
	rtl_printf("Modulo monitor inicializado exitosamente\n");
	rtl_printf("port 0x%x irq %d\n *** frecuencia %d\n",LPT_PORT,IRQ,frec);

	for (a = MAX; a > 0; a--) {
	  mat[a] = -1;
	}
	j=MAX;
        // return pthread_create (&thread, NULL, start_routine, 0);
        return 0;
}

void cleanup_module(void)
{
	error = rtl_free_irq(IRQ);   // libera el manejador de interrup.
	if(error < 0) {
	    rtl_printf("Error en rtl_hard_free_irq (%d)\n", error);
	    return;
	}

	for (a = 1; a <= MAX ; a++) {
	  rtl_printf("Delta[%d]: %ld\n",a,mat[a]);
	  rtl_delay(1000000);
	}

	rtl_printf("Modulo monitor finalizado exitosamente\n");
}
