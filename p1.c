#include "p1.h"

//En trozos[0] gardase o comando e a partir de trozos[1] os argumentos
int TrocearCadena(char* cadena, char* trozos[]){
    int i=1;

    if((trozos[0]=strtok(cadena," \n\t"))==NULL){
        return 0;
    }
    while((trozos[i]=strtok(NULL," \n\t"))!=NULL){
        i++;
    }
    return i;
}


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
    int df;

    if(trozos[1]==NULL){
        printRoute();
    }else if(strcmp(trozos[1],"-f")==0){
        if(trozos[2]==NULL){
            printRoute();
        }
        else{
            df=open(trozos[2],O_CREAT | O_EXCL, 0644);
            if(df==-1){
                perror("Imposible crear");
            }
            else{
                close(df);
            }
        }
    }
    else{
        if(mkdir(trozos[1],0777)!=0){
            perror("Imposible crear");
        }
    }
}

void stats(char *trozos[], int numWords){
    bool largo=false, access=false, enlace=false;
    int i=0;

    if(trozos[1]==NULL){
        printRoute();
    }
    else if(trozos[2]==NULL){
        printf("hola\n");
    }
    else{
        for(i=1;i<numWords;i++){
            if(strcmp(trozos[i],"-long")==0){
                largo=true;
            }
            else if(strcmp(trozos[i],"-acc")==0){
                access=true;
            }
            else if(strcmp(trozos[i],"-link")==0){
                enlace=false;
            }
            else{
                break;
            }
        }
    }
    
    //i++; //Para saber donde acaban los argumentos. Si hay 1 argumento entonces trozos[2] es fichero e "i" vale 2
    printf("%d %d %d %d\n",i,largo,access,enlace);
    
}


void list();


void delete();


void deltree();

