#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

#define PATH_MAX 100

/* Programa principal */
int main(int argc, char *argv[])
{
    int contador = 0; /* Para contar los elementos */
    char cwd[PATH_MAX]; /* Directorio actual */

    /* Chequear precondicions dadas por los flags */
    if (capturarFlag(argv, argc) == 1) return 1;

   if (getcwd(cwd, sizeof(cwd))) 
   {
        /* Directorio donde estan almacenados los archivos */
        strcat(cwd,"/directorio");
   } 
   else
    {
       printf("getcwd() error");
       return 1;
   }

    /* Abrir los directorios recursivamente, chequeando los flags */
    abrirDirectorio(cwd, 0, &contador);

    /* Imprimir el total */
    if (nocontar == 0) {
        printf("Total: %d\n", contador);
    }

    return 0;
}

