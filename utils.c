#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils.h"

/* Funcion de lectura y evaluación de la entrada*/
/**
 * The function recursively opens directories and counts the number of regular files, while applying
 * filters and printing file names and sizes based on flags.
 * 
 * @param dirroot a string representing the path of the directory to be opened and searched for files
 * and subdirectories.
 * @param nivel an integer representing the current level of recursion in the directory tree traversal.
 * It starts at 0 for the initial directory and increments by 1 for each subdirectory encountered.
 * @param contador A pointer to an integer variable that keeps track of the number of files processed
 * by the function. The function increments this variable by 1 each time it processes a regular file.
 * 
 * @return The function does not return anything, as its return type is `void`.
 */
void abrirDirectorio(char *dirroot, int nivel, int *contador)
{
    /* Directorio */
    DIR *directorio;

    /* punto a cada entrada en el directorio */
    /* `struct dirent *entrada;` is declaring a pointer variable `entrada` of type `struct dirent`.
    This pointer will be used to point to a `dirent` structure that contains information about a
    directory entry (file or subdirectory) when iterating through a directory using the `readdir()`
    function. */
    struct dirent *entrada;

    /* `directorio = opendir(dirroot);` is opening the directory specified by the `dirroot` parameter
    and returning a pointer to a `DIR` structure that contains information about the directory. This
    pointer is stored in the `directorio` variable. */
    directorio = opendir(dirroot);

    /*Chequear si No se puede abrir el directorio */
    /* This code block is checking if the directory specified by the `dirroot` parameter can be opened.
    If the directory cannot be opened, it prints an error message "Error al abrir directorio" (Error
    opening directory) to the console. */
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

    /* This code block is checking if the directory specified by the `dirroot` parameter can be closed
    using the `closedir()` function. If the directory cannot be closed, it prints an error message
    "Error al cerrar directorio" (Error closing directory) to the console. If the directory is
    successfully closed, the function returns without doing anything else. */
    if (closedir(directorio) == -1)
    {
        printf("Error al cerrar directorio.\n");
        /*FALTA: Exit */;
    }

    return;
}

/* Funcion que almacena los parametros obtenidos por los flags y verifica su correctitud */
/**
 * The function captures command line flags and their arguments for a program.
 * 
 * @param argv An array of strings containing the command line arguments passed to the program.
 * @param argc The number of arguments passed to the program, including the name of the program itself.
 * 
 * @return an integer value, which is either 0 or 1.
 */
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
/**
 * The function checks if a given path corresponds to a regular file.
 * 
 * @param path A string representing the path to a file.
 * 
 * @return The function `esArchivoRegular` returns an integer value indicating whether the file at the
 * given path is a regular file or not. It returns 1 if the file is a regular file, and 0 otherwise.
 */
int esArchivoRegular(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

/* Funcion que verifica si un inodo es un directorio */
/**
 * The function checks if a given path is a directory.
 * 
 * @param path A string representing the path of the file or directory to be checked.
 * 
 * @return The function `esDirectorio` returns an integer value of 0 or 1, indicating whether the given
 * path is a directory or not.
 */
int esDirectorio(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

/* Funcion que retorna el tamano de un archivo en KB */
/**
 * The function calculates the size of a file in kilobytes and returns the result.
 * 
 * @param filename A string containing the name of the file for which we want to determine the size.
 * 
 * @return the size of the file in kilobytes (KB), rounded up to the nearest integer.
 */
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
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}