#ifndef __UTILS__
#define __UTILS__

/* Declaring the variables that will be used in the program. */
/* VARIABLES GLOBALES */

 /* Arreglo que almacena cada filtro de los flags (r, s, t) */
char *filtros[3];       

/* Arreglo para almacenar las opciones de impresion.
Cada posicion representa un atributo (c o nocount, l, size). Si esta en 0, quiere decir que no se utilizo, 1 que si */
int nocontar; /* c o nocount */
int listar; /* l o list*/
int mostrarTamArch; /* size */

/* Declaring functions*/
void abrirDirectorio(char *dirroot, int nivel, int *contador);
int capturarFlag(char *argv[], int argc);
int tamanoArchivo(char *filename);


#endif