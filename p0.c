#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    char *comandosDisponibles[14]={"authors","pid","chdir","date","time","hist","comand","open","close","dup","listopen","infosys","help","bye"}; //14 de tamaño porque se el numero de comandos que hay, en este caso 14. Aumentaran en practicas consecutivas

    for(i=0;i<14;i++){
        if(strcmp(comando,comandosDisponibles[i])==0){
            return i; //Preguntar si se pode break
        }
    }

    return i;

}

void bye(bool *terminado){
    printf("Comando 13: Salir del Shell\n");
    *terminado=1;
}

void procesarComando(int comando, char *trozos[], bool *terminado){
    switch(comando){
        case 0:
            printf("Comando 0\n");
            break;
        case 1:
            printf("Comando 1\n");
            break;
        case 2:
            printf("Comando 2\n");
            break;
        case 3:
            printf("Comando 3\n");
            break;
        case 4:
            printf("Comando 4\n");
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
            printf("Comando 11\n");
            break;
        case 12:
            printf("Comando 12\n");
            break;
        case 13:
            bye(terminado);
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

    //printf("Numero de trozos de la funcion: %d\n",TrocearCadena(comando, trozos));

    free(comando);
    free(trozos);

    return 0;
}