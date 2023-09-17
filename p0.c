#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h> //Utilizase en infosys
#include "shellopenfiles.h"
#include <fcntl.h> //utilizase en open
#include "historic.h"

void comandN(char *trozos[], tListH *H, bool *terminado, tListF *L);

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
    char *comandosDisponibles[16]={"authors","pid","chdir","date","time","hist","command","open","close","dup","listopen","infosys","help","bye","exit","quit"};
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

void printListH(tListH L){
    for (tPosH i = firstH(L); i != NULL; i = nextH(i, L)) {
            tItemH comando = getItemH(i, L);
            printf("%d->%s\n", comando.id, comando.nombre);
        }
}

void hist(tListH *historial, char *trozos[]) {
    if (trozos[1] == NULL) {
        // Si no se proporciona ninguna opción, muestra todo el historial
        printListH(*historial);
    } else if (strcmp(trozos[1], "-c") == 0) {
        deleteListH(historial);
    } else if (strcmp(trozos[1], "-N") == 0 && trozos[2] != NULL) {
        int N = atoi(trozos[2]);
        tPosH n = findItemH(N, *historial);
        for (tPosH i = firstH(*historial); i != n; i = nextH(i, *historial)) {
            tItemH comando = getItemH(i, *historial);
            printf("%d->%s\n", comando.id, comando.nombre);
        }
    }
}

void printListF(tListF L) {
    tPosF p;
    tItemF d;

    for (p = firstF(L); p != NULL; p = nextF(p, L)) {
        d = getItemF(p, L);
        printf("descriptor: %d -> %s %s\n", d.df, d.nombre, d.mode);
    }
}

void Cmd_open (char * tr[], tListF *L) {
    int i, df, mode = 0;
    tItemF d;

    if (tr[1] == NULL) { /*no hay parametro*/
        printListF(*L);
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
        insertItemF(d,L);
        printf("Anadida entrada %d a la tabla ficheros abiertos\n",df);
    }
}

void Cmd_close(char *tr[], tListF *L){
    int df;

    if(tr[1]!=NULL){
        df=atoi(tr[1]);
    }

    if ((tr[1]==NULL) || (df<0)) { //no hay parametro
        printListF(*L); //o el descriptor es menor que 0
        return;
    }
    if (close(df)==-1)
        perror("Imposible cerrar descriptor");
    else deleteAtPositionF(findItemF(df, *L), L);
}

void Cmd_dup (char * tr[], tListF *L){
    int df, dfdup;
    char aux[4096], *p=malloc(1000);
    tItemF d, j;

    if(tr[1]!=NULL){
        df=atoi(tr[1]);
    }
    if ((tr[1]==NULL) || (df<0)) { //no hay parametro
        printListF(*L); //o el descriptor es menor que 0
        return;
    }

    d=getItemF(findItemF(df,*L),*L);
    strcpy(p,d.nombre);
    sprintf (aux,"dup %d (%s)",df, p);

    dfdup=dup(df);
    if (dfdup==-1) {
        perror("Imposible duplicar descriptor");
    }

    strcpy(j.mode,""); //Sin modo de apertura
    j.df=dfdup;
    strcpy(j.nombre,aux);
    insertItemF(j,L);

    free(p);
}

void infosys(){
    struct utsname infosys;

    uname(&infosys);
    printf("%s (%s) %s-%s %s\n",infosys.nodename,infosys.machine,infosys.sysname,infosys.release,infosys.version);
}

void help(char *trozos[]) {
  //Faltan las descripciones xd
    if (trozos[1] == NULL) {
        printf("help [cmd] ayuda sobre comandos\nComandos disponibles:\n");
        printf("authors [-l|-n]\n");
        printf("pid [-p]\n");
        printf("chdir [dir]\n");
        printf("date\n");
        printf("time\n");
        printf("hist [-c|-N]\n");
        printf("comand N\n");
        printf("open [file] mode\n");
        printf("close [df]\n");
        printf("dup [df]\n");
        printf("listopen\n");
        printf("infosys\n");
        printf("help [cmd]\n");
        printf("quit\n");
        printf("exit\n");
        printf("bye\n");
    } else {
        if (strcmp(trozos[1], "authors") == 0) {
            printf("authors: \n");
            printf("authors -l: \n");
            printf("authors -n: \n");
        } else if (strcmp(trozos[1], "pid") == 0) {
            printf("pid: \n");
            printf("pid -p: \n");
        } else if (strcmp(trozos[1], "chdir") == 0) {
            printf("chdir [dir]: \n");
        } else if (strcmp(trozos[1], "date") == 0) {
            printf("date: \n");
        } else if (strcmp(trozos[1], "time") == 0) {
            printf("time: \n");
        } else if (strcmp(trozos[1], "hist") == 0) {
            printf("hist: \n");
            printf("hist -c: \n");
            printf("hist -N: \n");
        } else if (strcmp(trozos[1], "comand") == 0) {
            printf("comand N: \n");
        } else if (strcmp(trozos[1], "open") == 0) {
            printf("open [file] mode: \n");
        } else if (strcmp(trozos[1], "close") == 0) {
            printf("close [df]: \n");
        } else if (strcmp(trozos[1], "dup") == 0) {
            printf("dup [df]: \n");
        } else if (strcmp(trozos[1], "listopen") == 0) {
            printf("listopen: \n");
        } else if (strcmp(trozos[1], "infosys") == 0) {
            printf("infosys: \n");
        } else if (strcmp(trozos[1], "help") == 0) {
            printf("help: \n");
            printf("help [cmd]: \n");
        } else if (strcmp(trozos[1], "quit") == 0) {
            printf("quit: \n");
        } else if (strcmp(trozos[1], "exit") == 0) {
            printf("exit: \n");
        } else if (strcmp(trozos[1], "bye") == 0) {
            printf("bye: \n");
        } else {
            printf("%s no encontrado\n",trozos[1]);
        }
    }
}

void salir(bool *terminado){
    *terminado=1;
}

void procesarComando(int comando, char *trozos[], bool *terminado, tListF *L, tListH *H){
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
            hist(H,trozos);
            break;
        case 6:
            comandN(trozos,H,terminado,L);
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
            printListF(*L);
            break;
        case 11:
            infosys();
            break;
        case 12:
            help(trozos);
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
            printf("No ejecutado: No such file or directory\n");
            break;
    }
}

void insertarESstd(tListF *L){
    tItemF stdin, stdout, stderr;

    //stdin
    stdin.df=STDIN_FILENO;
    strcpy(stdin.mode,"O_RDWR");
    strcpy(stdin.nombre,"entrada estandar");
    insertItemF(stdin,L);

    //stdout
    stdout.df=STDOUT_FILENO;
    strcpy(stdout.mode,"O_RDWR");
    strcpy(stdout.nombre,"salida estandar");
    insertItemF(stdout,L);

    //stderr
    stderr.df=STDERR_FILENO;
    strcpy(stderr.mode,"O_RDWR");
    strcpy(stderr.nombre,"error estandar");
    insertItemF(stderr,L);
}

void listMemFree(tListF *L){
    while(!isEmptyListF(*L)) deleteAtPositionF(firstF(*L),L);
}

void insertarComandoHist(tListH *L, char *comando){
    tItemH p, r;

    if(isEmptyListH(*L)){
        p.id=0;
    }
    else{
        r=getItemH(lastH(*L),*L);
        p.id=r.id+1;
    }

    strcpy(p.nombre, comando);

    insertItemH(p,L);
}

int main(){
    bool terminado=0;
    char *comando=malloc(100);
    char *comandoCompleto=malloc(100);
    char **trozos=malloc(100);
    int eleccionComando;
    tListF L;
    tListH H;

    createEmptyListF(&L);
    insertarESstd(&L);

    createEmptyListH(&H);

    while(!terminado){
        imprimirPrompt();
        leerComando(comando);
        //Formato para hist
        strcpy(comandoCompleto,comando);
        strtok(comandoCompleto,"\n");
        //Formato para hist
        TrocearCadena(comando, trozos);
        eleccionComando=elegirComando(trozos[0]);
        insertarComandoHist(&H, comandoCompleto);
        procesarComando(eleccionComando, trozos, &terminado, &L, &H);
    }

    //printf("Numero de trozos del comando: %d\n",TrocearCadena(comando, trozos));
    //printf("Comando %s. argumento %s\n",comando,trozos[1]);

    listMemFree(&L);
    deleteListH(&H);
    free(comando);
    free(trozos);
    free(comandoCompleto);

    return 0;
}

void comandN(char *trozos[], tListH *H, bool *terminado, tListF *L){

    if(trozos[1]==NULL){
        printListH(*H);
    }
    else{
        tItemH p=getItemH(findItemH(atoi(trozos[1]),*H),*H);
        
        TrocearCadena(p.nombre,trozos);
        procesarComando(elegirComando(trozos[0]),trozos,terminado, L, H);

    }
}