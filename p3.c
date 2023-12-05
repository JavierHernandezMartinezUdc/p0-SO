#include "p3.h"

void uid(char **trozos){
    uid_t ruid, euid;

    ruid = getuid();
    euid = geteuid();

    if (trozos[1]== 0 || strcmp(trozos[1],"-get")==1) {
        printf("ID de usuario real: %d, (%s)\n", ruid, getpwuid(ruid)->pw_name);
        printf("ID de usuario efectivo: %d, (%s)\n", euid, getpwuid(euid)->pw_name);
    } else if(strcmp(trozos[1],"-set")==0){
        if (strcmp(trozos[2],"-l")==0){
            //login
        }else{
            //numero (id)
        }
    }
}

void showvar(char **trozos, char **arg3, char **environ){
    if(trozos[1]==NULL){
        //Mostrar todas las variables
        int i=0;
        for(char **env = arg3; *env != NULL; env++){
            printf("%p->main arg3[%d]=(%p) %s\n",(void *)env,i,(void *)*env,*env);
            i++;
        }
    }
    else{
        char *valor=getenv(trozos[1]);

        for(char **env = arg3; *env != NULL; env++){
            if(strncmp(*env,trozos[1],strlen(trozos[1]))==0){
                printf("Con arg3 main %s(%p) @%p\n",*env,(void *)*env, (void *)env);
            }
        }

        for(char **env = environ; *env != NULL; env++){
            if(strncmp(*env,trozos[1],strlen(trozos[1]))==0){
                printf("  Con environ %s(%p) @%p\n",*env,(void *)*env, (void *)env);
            }
        }

        if(valor!=NULL){
            printf("   Con getenv %s(%p)\n",valor,(void *)valor);
        }
    }
}

int BuscarVariable (char * var, char *e[])  /*busca una variable en el entorno que se le pasa como parÃ¡metro*/
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

int CambiarVariable(char * var, char * valor, char *e[]) /*cambia una variable en el entorno que se le pasa como parÃ¡metro*/
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

void changevar(char **trozos, char **arg3, char **environ){
    if(trozos[1]==NULL || trozos[2]==NULL){
        printf("Uso: changevar [-a|-e|-p] var valor\n");
    }
    else{
        if(strcmp(trozos[1],"-a")==0){
            if(trozos[3]==NULL){
                printf("Uso: changevar [-a|-e|-p] var valor\n");
            }
            else{
                //Modificar valor en arg3
                int result=CambiarVariable(trozos[2],trozos[3],arg3);
                if(result==-1){
                    perror("Imposible cambiar variable");
                }
            }
        }
        else if(strcmp(trozos[1],"-e")==0){
            if(trozos[3]==NULL){
                printf("Uso: changevar [-a|-e|-p] var valor\n");
            }
            else{
                //Modificar valor usando environ
                int result=CambiarVariable(trozos[2],trozos[3],environ);
                if(result==-1){
                    perror("Imposible cambiar variable");
                }
            }
        }
        else if(strcmp(trozos[1],"-p")==0){
            if(trozos[3]==NULL){
                printf("Uso: changevar [-a|-e|-p] var valor\n");
            }
            else{
                if(getenv(trozos[2])==NULL){
                    //Crear variable
                    setenv(trozos[2],trozos[3],0);
                }
                else{
                    //Modificar variable
                    setenv(trozos[2],trozos[3],1);
                }
            }
        }
        else{
            printf("Uso: changevar [-a|-e|-p] var valor\n");
        }
    }
}

int CambiarNombre(char * viejo, char * nuevo, char * valor, char *e[]) /*cambia una variable en el entorno que se le pasa como parÃ¡metro*/
{                                                        /*lo hace directamente, no usa putenv*/
  int pos;
  char *aux;
   
  if ((pos=BuscarVariable(viejo,e))==-1)
    return(-1);
 
  if ((aux=(char *)malloc(strlen(valor)+strlen(nuevo)+2))==NULL)
	return -1;
  strcpy(aux,nuevo);
  strcat(aux,"=");
  strcat(aux,valor);
  e[pos]=aux;
  return (pos);
}

void subsvar(char **trozos, char **arg3, char **environ){
    if(trozos[1]==NULL || trozos[2]==NULL){
        printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
    }
    else{
        if(strcmp(trozos[1],"-a")==0){
            if(trozos[3]==NULL || trozos[4]==NULL){
                printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
            }
            else{
                //Modificar valor en arg3
                int result=CambiarNombre(trozos[2],trozos[3],trozos[4],arg3);
                if(result==-1){
                    perror("Imposible cambiar variable");
                }
            }
        }
        else if(strcmp(trozos[1],"-e")==0){
            if(trozos[3]==NULL || trozos[4]==NULL){
                printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
            }
            else{
                //Modificar valor usando environ
                int result=CambiarNombre(trozos[2],trozos[3],trozos[4],environ);
                if(result==-1){
                    perror("Imposible cambiar variable");
                }
            }
        }
        else{
            printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
        }
    }
}

void showenv(char **trozos, char **arg3, char **environ){
    if(trozos[1]==NULL){
        int i=0;
        for(char **env = arg3; *env != NULL; env++){
            printf("%p->main arg3[%d]=(%p) %s\n",(void *)env,i,(void *)*env,*env);
            i++;
        }
    }
    else if(strcmp(trozos[1],"-environ")==0){
        int i=0;
        for(char **env = environ; *env != NULL; env++){
            printf("%p->environ[%d]=(%p) %s\n",(void *)env,i,(void *)*env,*env);
            i++;
        }
    }
    else if(strcmp(trozos[1],"-addr")==0){
        printf("environ: %p (almacenado en %p)\n",(void *)environ[0],(void *)environ);
        printf("main arg3: %p (almacenado en %p)\n",(void *)environ[0],(void *)arg3);
    }
}

void Cmd_fork (tListP *P)
{
	pid_t pid;
	
	if ((pid=fork())==0){
        deleteListP(P);
		printf ("ejecutando proceso %d\n", getpid());
	}
	else if (pid!=-1)
		waitpid (pid,NULL,0);
}

void exec(char **trozos, int numWords){
    char comando[1024]="";

    for(int i=1;i<numWords;i++){
        strcat(comando,trozos[i]);
        if(i!=numWords-1){
            strcat(comando," ");
        }
    }

    if(access(trozos[1],X_OK)!=0){
        perror("Imposible ejecutar");
    }
    else{
        system(comando);
    }
}