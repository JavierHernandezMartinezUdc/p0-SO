#include "p3.h"

void Cmd_fork (char *tr[])
{
    pid_t pid;

    if ((pid=fork())==0){
/*		VaciarListaProcesos(&LP); Depende de la implementación de cada uno*/
        printf ("ejecutando proceso %d\n", getpid());
    }
    else if (pid!=-1)
        waitpid (pid,NULL,0);
}


int BuscarVariable (char * var, char *e[])  /*busca una variable en el entorno que se le pasa como parámetro*/
{
    int pos=0;
    char aux[1024];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    errno=ENOENT;   /*no hay tal variable*/
    return(-1);
}

int CambiarVariable(char * var, char * valor, char *e[]) /*cambia una variable en el entorno que se le pasa como parámetro*/
{                                                        /*lo hace directamente, no usa putenv*/
    int pos;
    char *aux;

    if ((pos=BuscarVariable(var,e))==-1)
        return(-1);

    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

void uid(char **trozos){
    uid_t ruid, euid;
    gid_t rgid, egid;

    ruid = getuid();
    euid = geteuid();

    if (trozos[1]== 0 || strcmp(trozos[1],"-get")==1) {
        printf("ID de usuario real (RUID): %d\n", ruid);
        printf("ID de usuario efectivo (EUID): %d\n", euid);
    } else if(strscmp(trozos[1],"-set")==1){
        if (strcmp(trozos[2],"-l")==1){

        }else{

        }
    }
}