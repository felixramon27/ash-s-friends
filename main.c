#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

/* `#define PATH_MAX 100` is defining a preprocessor macro named `PATH_MAX` with a value of 100. This
macro is used to represent the maximum length of a file path on the system. By defining it as a
macro, it can be used throughout the program to specify the size of character arrays used to store
file paths. */
#define PATH_MAX 100

/* Programa principal */
int main(int argc, char *argv[])
{
    /* `int contador = 0;` is initializing an integer variable named `contador` with a value of 0. This
    variable will be used to count the number of elements in the directories. */
    int contador = 0; /* Para contar los elementos */

    /* `char cwd[PATH_MAX];` is declaring a character array named `cwd` with a size of `PATH_MAX`. This
    array will be used to store the current working directory path. `PATH_MAX` is a constant defined
    in the `limits.h` header file, and it represents the maximum length of a file path on the
    system. */
    char cwd[PATH_MAX]; /* Directorio actual */

    /* Chequear precondicions dadas por los flags */
    /* `if (capturarFlag(argv, argc) == 1) return 1;` is checking if the function `capturarFlag`
    returns a value of 1. If it does, then the program immediately returns with a value of 1,
    indicating an error. The `capturarFlag` function is used to check the command line arguments
    passed to the program and set global variables based on the flags provided. If an error is
    detected in the command line arguments, `capturarFlag` will return 1, and the program will exit
    with an error. */
    if (capturarFlag(argv, argc) == 1) return 1;

   /* This code block is using the `getcwd` function to get the current working directory path and
   store it in the `cwd` character array. If `getcwd` is successful, it appends the string
   `"/directorio"` to the end of the `cwd` array using the `strcat` function. This is done to
   specify the directory where the files are stored. If `getcwd` fails, it prints an error message
   using `printf` and returns with a value of 1, indicating an error. */
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
    /* `abrirDirectorio(cwd, 0, &contador);` is calling the function `abrirDirectorio` with three
    arguments: `cwd` (a string containing the current working directory), `0` (an integer indicating
    the depth of recursion, which is initially set to 0), and `&contador` (a pointer to an integer
    variable that will be used to count the number of elements in the directories). The function
    `abrirDirectorio` will recursively open the directories and count the elements, updating the
    value of `contador` as it goes. */
    abrirDirectorio(cwd, 0, &contador);

    /* Imprimir el total */
    /* This code block is checking if the global variable `nocontar` is equal to 0. If it is, then it
    means that the program should count and print the total number of elements in the directories.
    The variable `contador` holds the count of elements, and it is printed using `printf` with the
    format string `"Total: %d\n"`. If `nocontar` is not equal to 0, then the program will not print
    the total count. */
    if (nocontar == 0) {
        printf("Total: %d\n", contador);
    }

    return 0;
}

