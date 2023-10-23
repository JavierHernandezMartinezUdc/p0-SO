#ifndef P0_P1_H
#define P0_P1_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> //Utilizase en open
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <dirent.h>

char LetraTF (mode_t m);
char *ConvierteModo2 (mode_t m);
int TrocearCadena(char* cadena, char* trozos[]);

//list
#include <dirent.h>
#include <libgen.h>

void create(char *trozos[]);
//create ruta
//create -f name crea un fichero
//create name crea un directorio
void stats(char *trozos[], int numWords);
//stat -long listado largo
//stat -acc acesstime
//stat -link si es enlace simbolico, el path contenido
//stat ruta
void list();

void delete(char **trozos);

void deltree(char **trozos);


#endif //P0_P1_H
