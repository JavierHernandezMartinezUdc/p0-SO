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

void getStats(){
    struct stat stats;

    if(lstat()){

    }
}

void stats(char *trozos[], int numWords){
    bool largo=false, access=false, enlace=false;
    int i=0;

    for(i=1;i<numWords;i++){
        if(strcmp(trozos[i],"-long")==0){
            largo=true;
        }
        else if(strcmp(trozos[i],"-acc")==0){
            access=true;
        }
        else if(strcmp(trozos[i],"-link")==0){
            enlace=true;
        }
        else{
            break;
        }
    }

    //i indica aqui el numero de palabras antes de los ficheros, por lo que si i=4 significa que trozos[4] (i) es el primer fichero
    //En caso de que no haya nombre de fichero entonces imprime la ruta 

    if(trozos[i]==NULL){
        printRoute();
    }
    else{
        //Aqui se pillan as stats de verdad
        if(largo){
            //Stats en formato largo
        }
        else{
            //Stats en formato corto
            getStats();
        }
    }
}


void list(char *trozos[]){
    int subcommand,noncommand;
    int x=1,l,i;
    if(trozos[1]==NULL){
        //
    }
    else if(trozos[1][0]!='-'){
        //
    }
    else {
        for(l=0; trozos[l][0]=='-'; l++){
            noncommand = l+1;
        }
        while(x<noncommand) {
            char *commandlist[6] = {"-hid", "-recb", "-reca", "-long", "-acc", "-link"};
            for (i = 0; i <= 6; i++) {
                if (strcmp(trozos[x], commandlist[i]) == 0) {
                    subcommand = i;
                }
            }
            switch (subcommand) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3,4,5:
                    break;
                default:
                    break;
            }
        }
    }
}



void delete();


void deltree();

