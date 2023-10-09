#include "p1.h"

void printRoute(){
    char path[1024];

    if(getcwd(path,1024)!=NULL){
        printf("%s\n",path);
    }
    else{
        perror("Error al obtener el directorio");
    }
}

void create(char *trozos[]){
    if(trozos[1]==NULL){
        //Imprime ruta
        printRoute();
    }else if(strcmp(trozos[1],"-f")==0){
        if(trozos[2]==NULL){
            printRoute();
        }
        else{
            //open() df = open(nombreArchivo, O_CREAT | O_WRONLY, 0644);
            //Crear fichero
            if(open(trozos[2],O_CREAT | O_EXCL | O_WRONLY, 0644)==-1){
                perror("Imposible crear");
            }
        }
    }
    else{
        //Crear directorio
        if(mkdir(trozos[1],0777)!=0){
            perror("Imposible crear");
        }
    }
}

void stats(char *trozos[]){
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

