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

int TrocearCadena(char* cadena, char* trozos[]);

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
void delete();
void deltree();

#endif //P0_P1_H
