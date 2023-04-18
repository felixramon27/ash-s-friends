#ifndef __UTILS__
#define __UTILS__

/* VARIABLES GLOBALES */
 /* Arreglo que almacena cada filtro de los flags (r, s, t) */
char *filtros[3];       
int nocontar; /* c o nocount */
int listar; /* l o list*/
int mostrarTamArch; /* size */
char *name;

/* Declaring functions*/
void abrirDirectorio(char *dirroot, int nivel, int *contador);
int capturarFlag(char *argv[], int argc);
int tamanoArchivo(char *filename);


#endif