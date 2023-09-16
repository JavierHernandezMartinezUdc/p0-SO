#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h> //Utilizase en infosys
#include "shellopenfiles.h"
#include <fcntl.h> //utilizase en open

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

void imprimirPrompt(){
    printf("->");
}

void leerComando(char *comando){
    fgets(comando,100,stdin);
}

int elegirComando(char *comando){
    int i;
    char *comandosDisponibles[16]={"authors","pid","chdir","date","time","hist","comand","open","close","dup","listopen","infosys","help","bye","exit","quit"};
    //16 de tamaño porque se el numero de comandos que hay, en este caso 14. Aumentaran en practicas consecutivas

    for(i=0;i<16;i++){ //Cambiar segun tamaño array
        if(strcmp(comando,comandosDisponibles[i])==0){
            return i;
        }
    }

    return i;
}

void authors(char *trozos[]){

    if(trozos[1]==NULL){
        printf("Javier Hernandez Martinez: javier.hernandez.martinez@udc.es\nPaula Carril Gontan: paula.carril@udc.es\n");
    }
    else {
        if (strcmp(trozos[1], "-l") == 0) {
            printf("javier.hernandez.martinez@udc.es\npaula.carril@udc.es\n"); //TODO revisar login paula
        } else if (strcmp(trozos[1], "-n") == 0) {
            printf("Javier Hernandez Martinez\nPaula Carril Gontan\n"); //TODO revisar nombre paula
        }
    }
}

void pid(char *trozos[]){

    if(trozos[1]==NULL){
        printf("PID: %d\n",getpid());
    }
    else{
        printf("PPID: %d\n",getppid());
    }
}

void changedir(char *trozos[]){
    char *path=malloc(100);

    if(trozos[1]==NULL){
        getcwd(path,100);
        printf("%s\n",path);
    }
    else{
        if(chdir(trozos[1])==-1){
            perror("Imposible cambiar directorio");
        }
    }

    free(path);
}

void fecha(){
    time_t actual; //tipo de dato de libreria time.h
    struct tm *fecha; //struct que conten informacion de dia, mes, ano, horas, minutos e segundos a partir de time_t

    time(&actual); //tempo desde epoch (1 de enero de 1970)
    fecha = localtime(&actual); //localtime devolve o struct en funcion da zona horaria

    int dia = fecha->tm_mday;
    int mes = fecha->tm_mon + 1;
    int anho = fecha->tm_year + 1900;

    printf("%02d/%02d/%04d\n", dia, mes, anho);
}

void hora(){
    time_t actual; //igual que funcion fecha pero datos distintos do struct
    struct tm *hora;

    time(&actual);
    hora = localtime(&actual);

    int hor = hora->tm_hour;
    int min = hora->tm_min;
    int sec = hora->tm_sec;

    printf("%02d:%02d:%02d\n", hor, min, sec);
}

void printList(tList L) {
    tPos p;
    tItem d;

    for (p = first(L); p != NULL; p = next(p, L)) {
        d = getItem(p, L);
        printf("descriptor: %d -> %s %s\n", d.df, d.nombre, d.mode);
    }
}

void Cmd_open (char * tr[], tList *L) {
    int i, df, mode = 0;
    tItem d;

    if (tr[1] == NULL) { /*no hay parametro*/
        printList(*L);
        return;
    }
    for (i = 2; tr[i] != NULL; i++) {
        if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
        else break;
    }
    df=open(tr[1], mode, 0777);
    if (df==-1) {
        perror("Imposible abrir fichero");
    }
    else {
        //mapeo de modos de apertura
        if(tr[2]==NULL){
            strcpy(d.mode,"");
        }
        else{
            switch (mode) {
                case O_CREAT:
                    strcpy(d.mode,"O_CREAT");
                    break;
                case O_EXCL:
                    strcpy(d.mode,"O_EXCL");
                    break;
                case O_RDONLY:
                    strcpy(d.mode,"O_RDONLY");
                    break;
                case O_WRONLY:
                    strcpy(d.mode,"O_WRONLY");
                    break;
                case O_RDWR:
                    strcpy(d.mode,"O_RDWR");
                    break;
                case O_APPEND:
                    strcpy(d.mode,"O_APPEND");
                    break;
                case O_TRUNC:
                    strcpy(d.mode,"O_TRUNC");
                    break;
                default:
                    strcpy(d.mode,"Desconocido");
            }
        }
        d.df=df;
        strcpy(d.nombre,tr[1]);
        insertItem(d,L);
        printf("Anadida entrada %d a la tabla ficheros abiertos\n",df);
    }
}

void Cmd_close(char *tr[], tList *L){
    int df;

    if(tr[1]!=NULL){
        df=atoi(tr[1]);
    }

    if ((tr[1]==NULL) || (df<0)) { //no hay parametro
        printList(*L); //o el descriptor es menor que 0
        return;
    }
    if (close(df)==-1)
        perror("Imposible cerrar descriptor");
    else deleteAtPosition(findItem(df, *L), L);
}

void Cmd_dup (char * tr[], tList *L){
    int df, dfdup;
    char aux[4096], *p=malloc(1000);
    tItem d, j;

    if(tr[1]!=NULL){
        df=atoi(tr[1]);
    }
    if ((tr[1]==NULL) || (df<0)) { //no hay parametro
        printList(*L); //o el descriptor es menor que 0
        return;
    }

    d=getItem(findItem(df,*L),*L);
    strcpy(p,d.nombre);
    sprintf (aux,"dup %d (%s)",df, p);

    dfdup=dup(df);
    if (dfdup==-1) {
        perror("Imposible duplicar descriptor");
    }

    strcpy(j.mode,""); //Sin modo de apertura
    j.df=dfdup;
    strcpy(j.nombre,aux);
    insertItem(j,L);

    free(p);
}

void infosys(){
    struct utsname infosys;

    uname(&infosys);
    printf("%s (%s) %s-%s %s\n",infosys.nodename,infosys.machine,infosys.sysname,infosys.release,infosys.version);
}

void salir(bool *terminado){
    *terminado=1;
}

void procesarComando(int comando, char *trozos[], bool *terminado, tList *L){
    switch(comando){
        case 0:
            authors(trozos);
            break;
        case 1:
            pid(trozos);
            break;
        case 2:
            changedir(trozos);
            break;
        case 3:
            fecha();
            break;
        case 4:
            hora();
            break;
        case 5:
            printf("Comando 5\n");
            break;
        case 6:
            printf("Comando 6\n");
            break;
        case 7:
            Cmd_open(trozos, L);
            break;
        case 8:
            Cmd_close(trozos, L);
            break;
        case 9:
            Cmd_dup(trozos, L);
            break;
        case 10:
            printList(*L);
            break;
        case 11:
            infosys();
            break;
        case 12:
            printf("Comando 12\n");
            break;
        case 13:
            salir(terminado);
            break;
        case 14:
            salir(terminado);
            break;
        case 15:
            salir(terminado);
            break;
        default:
            printf("Comando desconocido\n");
            break;
    }
}

void insertarESstd(tList *L){
    tItem stdin, stdout, stderr;

    //stdin
    stdin.df=STDIN_FILENO;
    strcpy(stdin.mode,"O_RDWR");
    strcpy(stdin.nombre,"entrada estandar");
    insertItem(stdin,L);

    //stdout
    stdout.df=STDOUT_FILENO;
    strcpy(stdout.mode,"O_RDWR");
    strcpy(stdout.nombre,"salida estandar");
    insertItem(stdout,L);

    //stderr
    stderr.df=STDERR_FILENO;
    strcpy(stderr.mode,"O_RDWR");
    strcpy(stderr.nombre,"error estandar");
    insertItem(stderr,L);
}

void listMemFree(tList *L){
    while(!isEmptyList(*L)) deleteAtPosition(first(*L),L);
}

int main(){
    bool terminado=0;
    char *comando=malloc(100);
    char **trozos=malloc(100);
    int eleccionComando;
    tList L;

    createEmptyList(&L);
    insertarESstd(&L);


    while(!terminado){
        imprimirPrompt();
        leerComando(comando);
        TrocearCadena(comando, trozos);
        eleccionComando=elegirComando(trozos[0]);
        //Aqui insertase na lista historial o comando
        procesarComando(eleccionComando, trozos, &terminado, &L);
    }

    //printf("Numero de trozos del comando: %d\n",TrocearCadena(comando, trozos));
    //printf("Comando %s. argumento %s\n",comando,trozos[1]);

    listMemFree(&L);
    free(comando);
    free(trozos);

    return 0;
}