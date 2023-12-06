#include "p3.h"
#include <signal.h>

void uid(char **trozos){
    uid_t ruid, euid;

    ruid = getuid();
    euid = geteuid();

    if (trozos[1] == NULL || strcmp(trozos[1],"-get")==0) {
        printf("ID de usuario real: %d, (%s)\n", ruid, getpwuid(ruid)->pw_name);
        printf("ID de usuario efectivo: %d, (%s)\n", euid, getpwuid(euid)->pw_name);
    } else if(strcmp(trozos[1],"-set")==0){
        if (strcmp(trozos[2],"-l")==0){
            //login
            
        }else{
            //numero (id)
            if(setuid(atoi(trozos[2]))==-1){
                perror("Error setuid");
                return;
            }
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

    system(comando);
}

void FechaHoraP(time_t t, char *dest){
    struct tm *local_time = localtime(&t);

    if (local_time == NULL) {
        perror("localtime");
        return;
    }

    int dia = local_time->tm_mday;
    int mes = local_time->tm_mon + 1;
    int year = local_time->tm_year + 1900;
    int hor = local_time->tm_hour;
    int min = local_time->tm_min;
    int seg = local_time->tm_sec;

    sprintf(dest,"%d/%d/%d %d:%d:%d",year,mes,dia,hor,min,seg);
}

void StatusToString(status status, char *dest){
    if(status==FINISHED){
        strcpy(dest,"FINISHED");
    }
    else if(status==STOPPED){
        strcpy(dest,"STOPPED");
    }
    else if(status==SIGNALED){
        strcpy(dest,"SIGNALED");
    }
    else if(status==ACTIVE){
        strcpy(dest,"ACTIVE");
    }
}

void jobs(tListP P){
    tPosP p;
    tItemP x;
    char fecha[1024];
    char status[1024];
    int prio;
    int endValue;

    for(p=firstP(P);p!=NULL;p=nextP(p,P)){
        x=getItemP(p,P);

        if(waitpid(x.pid, &endValue, 0)==x.pid){
            if(WIFEXITED(endValue)){
                x.estado=FINISHED;
                x.endValue=WEXITSTATUS(endValue);
            }
            else if (WIFSTOPPED(endValue)){
                x.estado=STOPPED;
                x.endValue=WTERMSIG(endValue);
            }
            else if (WIFSIGNALED(endValue)){
                x.estado=SIGNALED;
                x.endValue=WTERMSIG(endValue);
            }
        }

        prio=getpriority(PRIO_PROCESS,x.pid);

        updateItemP(x,p,&P);

        uid_t uid=getuid();
        struct passwd *userInfo = getpwuid(uid);
        if (userInfo == NULL) {
            perror("Error al obtener la información del usuario");
            return;
        }

        FechaHoraP(x.time,fecha);
        StatusToString(x.estado,status);

        printf("  %d       %s p=%d %s %s (%d) %s\n",x.pid,userInfo->pw_name,prio,fecha,status,WEXITSTATUS(endValue),x.command);
    }
}

void deljobs(char **trozos, tListP *P){
    tPosP p;

    if(trozos[1]==NULL){
        jobs(*P);
    }
    else if(strcmp(trozos[1],"-term")==0){
        for(p=firstP(*P);p!=NULL;p=nextP(p,*P)){
            if(getItemP(p,*P).estado==FINISHED){
                deleteAtPositionP(p,P);
                p=firstP(*P);
            }
        }
    }
    else if(strcmp(trozos[1],"-sig")==0){
        for(p=firstP(*P);p!=NULL;p=nextP(p,*P)){
            if(getItemP(p,*P).estado==SIGNALED){ //Condicion???
                deleteAtPositionP(p,P);
                p=firstP(*P);
            }
        }
    }
    else{
        jobs(*P);
    }
}

void job(char **trozos, tListP *P){
    tPosP p;
    tItemP x;
    char fecha[1024];
    char status[1024];
    int prio;
    int endValue;

    if(trozos[1]!=NULL){
        if(strcmp(trozos[1],"-fg")==0){ //Traer a primer plano
            //TODO
        }
        else{ //Mostrar info
            p=findItem(atoi(trozos[1]), *P);
            if(p==NULL){
                return;
            }
            x=getItemP(p,*P);

            if(waitpid(x.pid, &endValue, 0)==x.pid){
                if(WIFEXITED(endValue)){
                    x.estado=FINISHED;
                    x.endValue=WEXITSTATUS(endValue);
                }
                else if (WIFSTOPPED(endValue)){
                    x.estado=STOPPED;
                    x.endValue=WTERMSIG(endValue);
                }
                else if (WIFSIGNALED(endValue)){
                    x.estado=SIGNALED;
                    x.endValue=WTERMSIG(endValue);
                }
            }

            prio=getpriority(PRIO_PROCESS,x.pid);

            updateItemP(x,p,P);

            uid_t uid=getuid();
            struct passwd *userInfo = getpwuid(uid);
            if (userInfo == NULL) {
                perror("Error al obtener la información del usuario");
                return;
            }

            FechaHoraP(x.time,fecha);
            StatusToString(x.estado,status);

            printf("  %d       %s p=%d %s %s (%d) %s\n",x.pid,userInfo->pw_name,prio,fecha,status,WEXITSTATUS(endValue),x.command);
        }
    }
}

void newProcess(char **trozos, tListP *P, int numWords){
    pid_t pid;
    char **args=malloc(sizeof(char*) * numWords);
    tItemP x;
    char comando[1024]="";

    for(int i=0;i<numWords;i++){ //Copia todo menos "&"
        if(i!=numWords-1){
            args[i]=trozos[i];
        }
    }
    args[numWords-1]=NULL;

    /* Mini test de args
    for(int i=0;i<numWords-1;i++){
        printf("%s\n",args[i]);
    }
    return;
    */

    for(int i=0;i<numWords;i++){
        if(i!=numWords-1){
            strcat(comando,trozos[i]);
            if(i!=numWords-2){
                strcat(comando," ");
            }
        }
    }

    if(strcmp(trozos[numWords-1],"&")==0){ //Background
        pid=fork();

        if(pid==0){
            x.pid=getpid();
            execvp(trozos[0],args);
            exit(EXIT_SUCCESS);
        }
        else if(pid!=-1){
            //El padre no espera por el hijo
        }

        time(&x.time);
        x.estado=ACTIVE;
        strcpy(x.command,comando);
        if(!insertItemP(x,P)){
            printf("Imposible insertar\n");
            return;
        }
    }
    else{ //Foreground
        pid=fork();

        if(pid==0){
            execvp(trozos[0],args);
            exit(EXIT_SUCCESS);
        }
        else if(pid!=-1){
            waitpid(pid,NULL,0);
        }
    }

    free(args);
}