#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils.h"




/* Funcion de lectura y evaluación de la entrada*/
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
        int contarPoke = 1;

    while ((entrada = readdir(directorio)) != NULL)
    {
        char nombre[sizeof(entrada->d_name) + sizeof(dirroot) + 1];
        sprintf(nombre, "%s/%s", dirroot, entrada->d_name);
        
        if ((strcmp(entrada->d_name, ".") != 0) && (strcmp(entrada->d_name, "..") != 0))
        {

            /* Si el inodo es del tipo Regular File, entonces es un archivo */
            if (esArchivoRegular(nombre))
            {
            printf("    File: %d %s\n", contarPoke , nombre );

                *(contador) += 1;
                contarPoke++;


                /* Flag -l, para imprimir los nombres de los archivos */
                if (atributosMostrar[1] == 1)
                {
                    printf("%d %s\n", *contador, entrada->d_name);
                }
            }
            /* Si el inodo es del tipo DIR, entonces es un directorio */
            else if (isDirectory(nombre))
            {
            printf("Folder: %s %d\n", nombre, isDirectory(nombre) );

                /* Aplicar los filtros del flag */
                if (filtros[nivel] == NULL || strcmp(filtros[nivel], entrada->d_name) == 0)
                {
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
    char *tipo = NULL;
    char *frecuencia = NULL;

    /* Aplicar filtros de busqueda*/
    filtros[0] = region; /* -r */
    filtros[1] = tipo; /* -s */
    filtros[2] = frecuencia; /* -t */

    /* Aplicar opciones de impresion */
    atributosMostrar[0] = 0; /* c o nocount */
    atributosMostrar[1] = 0; /* l o list*/
    atributosMostrar[2] = 0; /* size */
}

int esArchivoRegular(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}