#include "p3.h"
/*
void uid(char **trozos){
    if(trozos[1]==NUL){

    }
    else if(strcmp(trozos[1],'-get')==1){

    }
    else if(strcmp(trozos[1],'-set')==1){

    }
    else if(strcmp(trozos[1],''))
}*/

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

void showvar(char **trozos){
    extern char **environ;

    if(trozos[1]==NULL){
        //Mostrar todas las variables
        int i=0;
        for(char **env = environ; *env != NULL; env++){
            printf("%p->main arg3[%d]=(%p) %s\n",(void *)env,i,(void *)*env,*env);
            i++;
        }
    }
    else{
        char *valor=getenv(trozos[1]);

        for(char **env = environ; *env != NULL; env++){
            if(strncmp(*env,trozos[1],strlen(trozos[1]))==0){
                printf("Con arg3 main %s(%p) @%p\n",*env,(void *)*env, (void *)env);
                printf("  Con environ %s(%p) @%p\n",*env,(void *)*env, (void *)env);
            }
        }

        if(valor!=NULL){
            printf("   Con getenv %s(%p)\n",valor,(void *)valor);
        }
    }
}