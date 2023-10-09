#include <math.h>
#include <time.h>
#include <sys/utsname.h> //Utilizase en infosys
#include "shellopenfiles.h"
#include "historic.h"
#include "p1.h"

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
    char *comandosDisponibles[21]={"authors","pid","chdir","date","time","hist","command","open","close","dup","listopen","infosys","help","bye","exit","quit","create","stat","list","delete","deltree"};
    //21 de tamaño porque se el numero de comandos que hay. Aumentaran en practicas consecutivas

    for(i=0;i<21;i++){ //Cambiar segun tamaño array
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
            printf("javier.hernandez.martinez@udc.es\npaula.carril@udc.es\n");
        } else if (strcmp(trozos[1], "-n") == 0) {
            printf("Javier Hernandez Martinez\nPaula Carril Gontan\n");
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
    if (trozos[1] == NULL || trozos[1][0]!='-') {
        //Muestra el historial
        printListH(*historial);
    } else if (strcmp(trozos[1], "-c") == 0) {
        //Borra el historial
        deleteListH(historial);
    } else{
        char N[strlen(trozos[1])-1];
        int n = atoi(strcpy(N, trozos[1]+1));
        if(n >= 0 && n <= getItemH(lastH(*historial),*historial).id){
            tPosH  x = findItemH(n, *historial);
            for (tPosH i = firstH(*historial); i != x; i = nextH(i, *historial)) {
                tItemH comando = getItemH(i, *historial);
                printf("%d->%s\n", comando.id, comando.nombre);
            }
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
    bool primerModo=true;

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
        strcpy(d.mode,"");
        if(mode & O_CREAT){
            if(!primerModo){
                strcat(d.mode," ");
            }
            strcat(d.mode,"O_CREAT");
            primerModo=false;
        }
        if(mode & O_EXCL){
            if(!primerModo){
                strcat(d.mode," ");
            }
            strcat(d.mode,"O_EXCL");
            primerModo=false;
        }
        if(mode & O_RDONLY){
            if(!primerModo){
                strcat(d.mode," ");
            }
            strcat(d.mode,"O_RDONLY");
            primerModo=false;
        }
        if(mode & O_WRONLY){
            if(!primerModo){
                strcat(d.mode," ");
            }
            strcat(d.mode,"O_WRONLY");
            primerModo=false;
        }
        if(mode & O_RDWR){
            if(!primerModo){
                strcat(d.mode," ");
            }
            strcat(d.mode,"O_RDWR");
            primerModo=false;
        }
        if(mode & O_APPEND){
            if(!primerModo){
                strcat(d.mode," ");
            }
            strcat(d.mode,"O_APPEND");
            primerModo=false;
        }
        if(mode & O_TRUNC){
            if(!primerModo){
                strcat(d.mode," ");
            }
            strcat(d.mode,"O_TRUNC");
            primerModo=false;
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
    else {
        deleteAtPositionF(findItemF(df, *L), L);
    }
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
        free(p);
        return;
    }
    if(findItemF(df,*L)==NULL){
        free(p);
        return;
    }
    d=getItemF(findItemF(df,*L),*L);
    strcpy(p,d.nombre);
    
    sprintf (aux,"dup %d (%s)",df, p);

    dfdup=dup(df);
    if (dfdup==-1) {
        perror("Imposible duplicar descriptor");
    }

    if(strstr(d.mode,"O_CREAT")!=NULL){
        strcpy(j.mode,d.mode+8);
    }
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
    //Faltan las descripciones xd xd
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
        printf("create\n");
        printf("stat\n");
        printf("list\n");
        printf("delete\n");
        printf("deltree\n");
    } else {
        if (strcmp(trozos[1], "authors") == 0) {
            printf("authors: muestra los nombres y los inicios de sesión de los autores del programa\n");
            printf("authors -l: muesta solo los inicios de sesión\n");
            printf("authors -n: muestra solo los nombre\n");
        } else if (strcmp(trozos[1], "pid") == 0) {
            printf("pid: Muestra el pid del proceso que ejecuta el shell\n");
            printf("pid -p: Muestra el pid del proceso padre del shell\n");
        } else if (strcmp(trozos[1], "chdir") == 0) {
            printf("chdir [dir]: Cambia el directorio de trabajo actual del shell a dir\n");
            printf("chdir: Muestra el directorio de trabajo actual\n");
        } else if (strcmp(trozos[1], "date") == 0) {
            printf("date: Muestra la data actual\n");
        } else if (strcmp(trozos[1], "time") == 0) {
            printf("time: Muestra la hora actual\n");
        } else if (strcmp(trozos[1], "hist") == 0) {
            printf("hist: Muestra el historial de comandos ejecutados por el shell\n");
            printf("hist -c: Borra el historial\n");
            printf("hist -N: Muestra los N primeros comandos en el historial\n");
        } else if (strcmp(trozos[1], "comand") == 0) {
            printf("comand N: Repite el comando numero N del historial\n");
        } else if (strcmp(trozos[1], "open") == 0) {
            printf("open [file] mode: Abre un archivo y lo añade a la lista de archivos abiertos\n");
        } else if (strcmp(trozos[1], "close") == 0) {
            printf("close [df]: cierra el df y elimina su archivo correspondienta\n");
        } else if (strcmp(trozos[1], "dup") == 0) {
            printf("dup [df]: duplica el df y crea un nuevo archivo\n");
        } else if (strcmp(trozos[1], "listopen") == 0) {
            printf("listopen: Lista los archivos abiertos con su df y modo\n");
        } else if (strcmp(trozos[1], "infosys") == 0) {
            printf("infosys: Muestra informacion de la maquina ejecutando la shell\n");
        } else if (strcmp(trozos[1], "help") == 0) {
            printf("help: Lista los comandos disponibles\n");
            printf("help [cmd]: Da una breve descipcion del comando especificado\n");
        } else if (strcmp(trozos[1], "quit") == 0) {
            printf("quit: Termina la shell\n");
        } else if (strcmp(trozos[1], "exit") == 0) {
            printf("exit: Termina la shell\n");
        } else if (strcmp(trozos[1], "bye") == 0) {
            printf("bye: Termina la shell\n");
        } else if (strcmp(trozos[1], "create") == 0) {
            printf("create [-f] [name]      Crea un directorio o fichero (-f)"); //2 tabulados
        } else if (strcmp(trozos[1], "stat") == 0) {
            printf("stat [-long] [-link] [-acc] name1 name2 ...     lista ficheros;\n      -long: listado largo\n      -acc: accesstime\n      -link: si es enlace simbolico, el path contenido\n"); //2 tabulados
        } else if (strcmp(trozos[1], "list") == 0) {
            printf("list [-reca] [-recb] [-hid] [-long] [-link] [-acc] n1 n2 ...        list contenidos de directorios\n        -hid: incluye los ficheros ocultos\n        -recb: recursivo (antes)\n        -reca: recursivo (despues)\n        resto de parametros como stat\n"); //2 tabulados
        } else if (strcmp(trozos[1], "delete") == 0) {
            printf("delete [name1 name2 ...]        Borra ficheros o directorios vacios\n"); //2 tabulados
        } else if (strcmp(trozos[1], "deltree") == 0) {
            printf("deltree [name1 name2 ...]       Borra ficheros o directorios no vacios recursivamente\n"); //2 tabulados
        } else {
            printf("%s no encontrado\n", trozos[1]);
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
        case 16:
            create(trozos);
            break;
        case 17:
            //stat();
            break;
        case 18:
            //list();
            break;
        case 19:
            //delete();
            break;
        case 20:
            //deltree();
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
        if(strcmp(comando,"\n")!=0){
            //Formato para hist
        strcpy(comandoCompleto,comando);
        strtok(comandoCompleto,"\n");
        //Formato para hist
        TrocearCadena(comando, trozos);
        eleccionComando=elegirComando(trozos[0]);
        insertarComandoHist(&H, comandoCompleto);
        procesarComando(eleccionComando, trozos, &terminado, &L, &H);
        }
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
    tPosH p;
    int cnt=0;

    if(trozos[1]==NULL){
        printListH(*H);
    }
    else{
        if(strcmp(trozos[1],"0")==0){
            for(p=firstH(*H);p!=NULL;p=nextH(p,*H)){
            cnt++;
            }
            if(cnt==1){
                printf("Bucle infinito de command 0. No se ejecutará\n");
                return;
            }
        }
        int i=atoi(trozos[1]);
        tPosH q=findItemH(i,*H);

        if(q==NULL){
            printf("No hay elemento %i en el historico\n",i);
        }
        else{
            tItemH p=getItemH(q,*H);
        
            TrocearCadena(p.nombre,trozos);
            printf("Ejecutando el hist (%d): %s\n",i,trozos[0]);
            procesarComando(elegirComando(trozos[0]),trozos,terminado, L, H);
        }
    }
}