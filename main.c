#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "archivos.h"

/* VARIABLES GLOBALES */

 /* Arreglo que almacena cada filtro de los flags (r, s, t) */
char *filtros[3];       

/* Arreglo para almacenar las opciones de impresion.
Cada posicion representa un atributo (c o nocount, l, size). Si esta en 0, quiere decir que no se utilizo, 1 que si */
int atributosMostrar[3]; 

/* Programa principal */
int main(int argc, char *argv[])
{
    int contador = 0; /* Para contar los elementos */

    /* Chequear precondicions dadas por los flags */
    capturarFlag(argv);

    /* Abrir los directorios recursivamente, chequeando los flags */
    abrirDirectorio("./directorio", 0, &contador);

    /* Imprimir el total */
    printf("Total: %d\n", contador);

    return 0;
}

void abrirDirectorio(char *dirroot, int nivel, int *contador)
{
    /* Directorio */
    DIR *directorio;

    /* punto a cada entrada en el directorio */
    struct dirent *entrada;

    directorio = opendir(dirroot);

    /*Chequear si No se puede abrir el directorio */
    if (directorio == NULL)
    {
        printf("Error al abrir directorio.\n");
    }

    while ((entrada = readdir(directorio)) != NULL)
    {
        if ((strcmp(entrada->d_name, ".") != 0) && (strcmp(entrada->d_name, "..") != 0))
        {
            /* Si el inodo es del tipo Regular File, entonces es un archivo */
            if (entrada->d_type == DT_REG)
            {
                *(contador) += 1;

                /* Flag -l, para imprimir los nombres de los archivos */
                if (atributosMostrar[1] == 1)
                {
                    printf("%d %s\n", *contador, entrada->d_name);
                }
            }
            /* Si el inodo es del tipo DIR, entonces es un directorio */
            else if (entrada->d_type == DT_DIR)
            {
                /* Aplicar los filtros del flag */
                if (filtros[nivel] == NULL || strcmp(filtros[nivel], entrada->d_name) == 0)
                {
                    char nombre[sizeof(entrada->d_name) + sizeof(dirroot) + 1];
                    sprintf(nombre, "%s/%s", dirroot, entrada->d_name);
                    printf("Este print es de Test: Filtro aplicado: %s\n Abrir directorio: %s\n", filtros[nivel], nombre);
                    abrirDirectorio(nombre, nivel + 1, contador);
                }
            }
        }
    }

    if (closedir(directorio) == -1)
    {
        printf("Error closing directory.\n");
        /*FALTA: Exit */;
    }

    return;
}

void capturarFlag(char *argv[]) {
    /*FALTA: Captar estos atributos mediante los flags */

    /* Se usa de prueba filtrando por region y tipo. Es NULL si no se aplico algun filtro */
    char *region = NULL;
    char *tipo = "pokemon";
    char *frecuencia = "one_time";

    /* Aplicar filtros de busqueda*/
    filtros[0] = region; /* -r */
    filtros[1] = tipo; /* -s */
    filtros[2] = frecuencia; /* -t */

    /* Aplicar opciones de impresion */
    atributosMostrar[0] = 0; /* c o nocount */
    atributosMostrar[1] = 1; /* l o list*/
    atributosMostrar[2] = 0; /* size */


}