#include "p3.h"

extern char **environ;

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

void showvar(char **trozos, char **arg3){
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

void changevar(char **trozos, char **arg3){
    if(trozos[1]==NULL || trozos[2]==NULL){
        printf("Uso: changevar [-a|-e|-p] var valor\n");
    }
    else{
        if(strcmp(trozos[1],"-a")==0){
            if(trozos[2]==NULL || trozos[3]==NULL){
                printf("Uso: changevar [-a|-e|-p] var valor\n");
            }
            else{
                //Modificar valor en arg3
            }
        }
        else if(strcmp(trozos[1],"-e")==0){
            if(trozos[2]==NULL || trozos[3]==NULL){
                printf("Uso: changevar [-a|-e|-p] var valor\n");
            }
            else{
                //Modificar valor usando environ
            }
        }
        else if(strcmp(trozos[1],"-p")==0){
            if(trozos[2]==NULL || trozos[3]==NULL){
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