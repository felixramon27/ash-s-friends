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

    while ((entrada = readdir(directorio)) != NULL)
    {
        char dir[sizeof(entrada->d_name) + sizeof(dirroot) + 1];
        sprintf(dir, "%s/%s", dirroot, entrada->d_name);
        
        if ((strcmp(entrada->d_name, ".") != 0) && (strcmp(entrada->d_name, "..") != 0))
        {

            /* Si el inodo es del tipo Regular File, entonces es un archivo */
            if (esArchivoRegular(dir))
            {
                if (nombreTarget != NULL) {
                    if (comienzaCon(entrada->d_name, nombreTarget) == 0) {
                        continue;
                    }
                }

                *(contador) += 1;

                /* Flag -l, para imprimir los nombres de los archivos */
                if (listar == 1 || mostrarTamArch == 1) {
                    printf("FileNo: %d ", *contador);

                    if (listar == 1) {
                        printf("-- Nombre: %s ", entrada->d_name);   
                    }

                    if (mostrarTamArch == 1) {
                        printf("-- Size: %d kb", tamanoArchivo(dir));   
                    }

                    printf("\n");   
                }
                
            }
            /* Si el inodo es del tipo DIR, entonces es un directorio */
            else if (esDirectorio(dir))
            {
                /* Aplicar los filtros del flag */
                if (filtros[nivel] == NULL || strcmp(filtros[nivel], entrada->d_name) == 0)
                {
                    abrirDirectorio(dir, nivel + 1, contador);
                }
            }
        }
    }

    if (closedir(directorio) == -1)
    {
        printf("Error al cerrar directorio.\n");
        /*FALTA: Exit */;
    }

    return;
}

/* Funcion que almacena los parametros obtenidos por los flags y verifica su correctitud */
int capturarFlag(char *argv[], int argc) {
    int i = 1;

    /* Aplicar opciones por defecto de impresion. 1 esta activo, 0 esta desactivada */
    nocontar = 0; /* c o nocount */
    listar = 0; /* l o list*/
    mostrarTamArch = 0; /* size */
    filtros[0], filtros[1], filtros[2] = NULL, NULL, NULL;
    nombreTarget = NULL;

    while(i < argc) {
        if  (strcmp(argv[i], "-r") == 0) {
            if (filtros[0] != NULL) {
                printf("Error, argumento de -r ya se habia asignado\n");
                return 1;
            }

            i++;
            if (i >= argc || argv[i][0] == '-' ) {
            printf("Error en argumentos de -r\n");
            return 1;
            }

            /* Almacenar la region */
            filtros[0] = argv[i];     
        } else if (strcmp(argv[i], "-s") == 0) {
            i++;
            if (filtros[1] != NULL || i >= argc || argv[i][0] == '-' ) {
                printf("Error en argumentos de -s\n");
                return 1;
            }

            /* Almacenar la especie */
            filtros[1] = argv[i];
        } else if (strcmp(argv[i], "-t") == 0) {
            i++;
            if (filtros[2] != NULL || i >= argc || argv[i][0] == '-' ) {
                printf("Error en argumentos de -t\n");
                return 1;
            }

            /* Almacenar el tipo de frecuencia */
            filtros[2] = argv[i];
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--nocount") == 0) {
            /* Almacenar flag c */
            nocontar = 1;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
            /* Almacenar flag l */
            listar = 1;
        } else if (strcmp(argv[i], "--size") == 0) {
            /* Almacenar flag size */
            mostrarTamArch = 1;
        } else {
            if (nombreTarget != NULL || argv[i][0] == '-' ) {
                printf("Error en argumentos\n");
                return 1;
            }

            nombreTarget = argv[i];
        }

        i++;
    }

    return 0;
}

/* Funcion que verifica si un inodo es un archivo regular */
int esArchivoRegular(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

/* Funcion que verifica si un inodo es un directorio */
int esDirectorio(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

/* Funcion que retorna el tamano de un archivo en KB */
int tamanoArchivo(char *filename) {
    FILE *fichero;
    long int tamano;

    fichero=fopen(filename,"r");

    fseek(fichero, 0L, SEEK_END);
    tamano = ftell(fichero);
    fclose(fichero);
    return tamano/1024 + 1;
}

int comienzaCon(const char *a, const char *b)
{
   if(strstr(a, b)) return 1;
   return 0;
}