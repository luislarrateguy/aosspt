# Introducción #

Dudas/Consultas referidas al trabajo practico nº 3.



# Consultas #

  1. En todo momento estan funcionando los 7 mutexd o pueden ir "apareciendo" de a poco? **Respuesta:** Si, van apareciendo
  1. Debemos considerar que los mutexd pueden dejar de funcionar en algun momento? **Respuesta:** No.
  1. En la figura del enunciado donde aprece la jerarquía de procesos mutexd, S1, S2, etc, donde S1 tiene el token... ¿cómo distinguir qué proceso mutexd va a tener el token al comienzo? ¿con el archivo de configuración /etc/mutex.cfg? ¿quizá simplemente al primer proceso ubicado en dicho archivo? Otra pregunta relacionada: ¿como armar esa estructura jerárquica? ¿cómo hacer que S4 y S5 dependan de S2? **Respuesta:** Colocar en el archivo de configuración quién es el holder de cada mutexd.
  1. El archivo /etc/mutexd.cfg sirve para que, cuando cada mutexd se inicie sepa en que puertos escuchan los demas servidores? **Respuesta:** Exacto.

## Inicialización del algoritmo ##
Esta es una descripción de cómo yo entiendo que funciona el algoritmo de Raymond al inicializarse:
  1. Un proceso mutexd se ejecuta de la siguiente forma: $ ./mutexd nroPuerto
  1. Luego busca en el archivo de configuración el puerto nroPuerto. Podría comprobar que nadie este ocupando dicho puerto.
  1. Una vez que tomó el puerto nroPuerto en la máquina local (todos los mutexd se ejecutan en la misma máquina), lee los otros registros del archivo mutex.cfg para saber cuales son los otros servidores. El primero que se ejecute no se va a poder conectar (mensaje HELLO) con ninguno. No importa... cuando se ejecute el segundo le va a avisar (con un HELLO, y el primero le contesta con HELLO). Y cuando se ejecute el tercero le va a avisar a los otros dos, y así. De esta forma van armando su lista de procesos mutexd online. **No comienza el algoritmo hasta que** todos los mutexd tengan online a todos los otros mutexd que figuran en el archivo.
**Respuesta:** El razonamiento es correcto.