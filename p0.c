#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>

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
    char *comandosDisponibles[16]={"authors","pid","chdir","date","time","hist","comand","open","close","dup","listopen","infosys","help","bye","exit","quit"}; //16 de tamaño porque se el numero de comandos que hay, en este caso 14. Aumentaran en practicas consecutivas

    for(i=0;i<16;i++){ //Cambiar segun tamaño array
        if(strcmp(comando,comandosDisponibles[i])==0){
            return i; //Preguntar si se pode break
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

void pid(char *trozos[]){ //Preguntar como se comproba

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
    time_t actual;
    struct tm *fecha;

    time(&actual);
    fecha = localtime(&actual);

    int dia = fecha->tm_mday;
    int mes = fecha->tm_mon + 1;
    int anho = fecha->tm_year + 1900;

    printf("%02d/%02d/%04d\n", dia, mes, anho);
}

void hora(){
    time_t actual;
    struct tm *hora;

    time(&actual);
    hora = localtime(&actual);

    int hor = hora->tm_hour;
    int min = hora->tm_min;
    int sec = hora->tm_sec;

    printf("%02d:%02d:%02d\n", hor, min, sec);
}

void infosys(){
    struct utsname infosys;

    uname(&infosys);
    printf("%s (%s) %s-%s %s\n",infosys.nodename,infosys.machine,infosys.sysname,infosys.release,infosys.version);
}

void salir(bool *terminado){
    *terminado=1;
}

void procesarComando(int comando, char *trozos[], bool *terminado){
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
            printf("Comando 7\n");
            break;
        case 8:
            printf("Comando 8\n");
            break;
        case 9:
            printf("Comando 9\n");
            break;
        case 10:
            printf("Comando 10\n");
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

int main(){

    bool terminado=0;
    char *comando=malloc(100);
    char **trozos=malloc(100);
    int eleccionComando;

    while(!terminado){
        imprimirPrompt();
        leerComando(comando);
        TrocearCadena(comando, trozos);
        //Aqui insertase na lista historial o comando
        eleccionComando=elegirComando(trozos[0]);
        procesarComando(eleccionComando, trozos, &terminado);
    }

    //printf("Numero de trozos del comando: %d\n",TrocearCadena(comando, trozos));
    //printf("Comando %s. argumento %s\n",comando,trozos[1]);

    free(comando);
    free(trozos);

    return 0;
}