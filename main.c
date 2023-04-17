#include <stdio.h>
#include <string.h>
#include "utils.h"

#define PATH_MAX 100

/* Programa principal */
int main(int argc, char *argv[])
{
    int contador = 0; /* Para contar los elementos */
    char cwd[PATH_MAX];

    /* Chequear precondicions dadas por los flags */
    capturarFlag(argv);

   if (getcwd(cwd, sizeof(cwd))) {
   } else {
       perror("getcwd() error");
       return 1;
   }

    strcat(cwd,"/directorio");
    printf("Current working dir: %s\n", cwd);
    /* Abrir los directorios recursivamente, chequeando los flags */
    abrirDirectorio(cwd, 0, &contador);

    /* Imprimir el total */
    printf("Total: %d\n", contador);

    return 0;
}

