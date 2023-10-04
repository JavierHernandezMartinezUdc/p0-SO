#ifndef P0_P1_H
#define P0_P1_H

#include <string.h>

void create(char *trozos[]);
//create ruta
//create -f name crea un fichero
//create name crea un directorio
void stat(char *trozos[]);
//stat -long listado largo
//stat -acc acesstime
//stat -link si es enlace simbolico, el path contenido
//stat ruta
void list();
void delete();
void deltree();

#endif //P0_P1_H
