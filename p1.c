#include "p1.h"


void create(char *trozos[]){
    if(trozos[1]==NULL){
        //Imprime ruta
    }
    else if(strcmp(trozos[1],"-f")==0){
        //Crear fichero
    }
    else{
        //Crear directorio
    }
}

void stat(char *trozos[]){
    if(trozos[1]==NULL ){
        //Imprime ruta
    }
    else if(strcmp(trozos[1],"-long")==0){
        //stat -long listado largo
    }
    else if(strcmp(trozos[1],"-acc")==0){
        //stat -acc acesstime
    }
    else if(strcmp(trozos[1],"-link")==0){
        //stat -link si es enlace simbolico, el path contenido
    }
    else {
        //Accede a fichero trozos[1]
    }
}


void list();


void delete();


void deltree();

