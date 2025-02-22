#include "p1.h"

char LetraTF (mode_t m){
     switch (m&__S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case __S_IFSOCK: return 's'; /*socket */
        case __S_IFLNK: return 'l'; /*symbolic link*/
        case __S_IFREG: return '-'; /* fichero normal*/
        case __S_IFBLK: return 'b'; /*block device*/
        case __S_IFDIR: return 'd'; /*directorio */ 
        case __S_IFCHR: return 'c'; /*char device*/
        case __S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

char *ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&__S_ISVTX) permisos[9]='t';
    
    return permisos;
}

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


void printRoute(){
    char path[1024];

    if(getcwd(path,1024)!=NULL){
        printf("%s\n",path);
    }
    else{
        perror("Error al obtener el directorio");
    }
}

void create(char *trozos[]){
    int df;

    if(trozos[1]==NULL){
        printRoute();
    }else if(strcmp(trozos[1],"-f")==0){
        if(trozos[2]==NULL){
            printRoute();
        }
        else{
            df=open(trozos[2],O_CREAT | O_EXCL, 0644);
            if(df==-1){
                perror("Imposible crear");
            }
            else{
                close(df);
            }
        }
    }
    else{
        if(mkdir(trozos[1],0777)!=0){
            perror("Imposible crear");
        }
    }
}

void printTime(time_t t){
    struct tm tm;

    tm=*localtime(&t);

    printf("%d/%d/%d-%d:%d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,  tm.tm_hour, tm.tm_min);
}

void getStats(char *nombre){
    struct stat stats;

    if(lstat(nombre,&stats)==-1){
        perror("stat error");
    }
    else{
        printf("%9ld %s\n", stats.st_size, nombre);
    }
}

void getStatsLargo(char *nombre, bool access, bool enlace){
    struct stat stats;
    struct passwd *usuario;
    struct group *grupo;
    char user[1024];
    char group[1024];
    char link[1024];

    if(lstat(nombre,&stats)==-1){
        perror("stat error");
    }
    else{
        if(access){
            printTime(stats.st_atime); //atime -> access time
        }
        else{
            printTime(stats.st_mtime); //mtime -> modification time
        }

        usuario=getpwuid(stats.st_uid); //si podemos escribimos el nombre, sino el uid
        if(usuario!=NULL){
            strcpy(user,usuario->pw_name);
        }
        else{
            sprintf(user,"%d",stats.st_uid);
        }

        grupo=getgrgid(stats.st_gid); //si podemos escribimos el nombre, sino el gid
        if(grupo!=NULL){
            strcpy(group,grupo->gr_name);
        }
        else{
            sprintf(group, "%d", stats.st_gid);
        }

        printf("%5lu (%8lu) %8s %8s %s\t%ld %s", stats.st_nlink, stats.st_ino, user, group, ConvierteModo2(stats.st_mode), stats.st_size, nombre);

        if(enlace && S_ISLNK(stats.st_mode)){
            if(readlink(nombre,link,1024)<0){
                perror("link error");
            }
            else{
                printf(" -> %s",link);
            }
        }
        printf("\n");
    }
}

void stats(char *trozos[], int numWords){
    bool largo=false, access=false, enlace=false;
    int i=0;

    for(i=1;i<numWords;i++){
        if(strcmp(trozos[i],"-long")==0){
            largo=true;
        }
        else if(strcmp(trozos[i],"-acc")==0){
            access=true;
        }
        else if(strcmp(trozos[i],"-link")==0){
            enlace=true;
        }
        else{
            break;
        }
    }

    //i indica aqui el numero de palabras antes de los ficheros, por lo que si i=4 significa que trozos[4] (i) es el primer fichero
    //En caso de que no haya nombre de fichero entonces imprime la ruta 

    if(trozos[i]==NULL){
        printRoute();
    }
    else{
        //Aqui se pillan as stats de verdad
        while(trozos[i]!=NULL){
            if(largo){
                //Stats en formato largo
                getStatsLargo(trozos[i],access, enlace);
            }
            else{
                //Stats en formato corto
                getStats(trozos[i]);
            }
            i++;
        }
    }
}

void listDir(char *path, bool largo, bool access, bool enlace, bool reca, bool recb, bool hid){
    DIR *dir;
    struct dirent *entry;
    char ruta[1024];
    char perrormsg[1024];
    struct stat sts;
    char **subdirs=NULL;
    char **files=NULL;
    int sizeD=0, sizeF=0;
    char nombre[4096];

    char* util=strrchr(path,'/');
    if(util!=NULL && path[0]!='/'){
        util++;
    }
    else{
        util=path;
    }

    dir=opendir(util);

    if(dir!=NULL){

        getcwd(ruta,1024);
        chdir(util);

        while((entry=readdir(dir))!=NULL){

            char *util=strrchr(entry->d_name,'/');
            if(util!=NULL){
                util++;
            }
            else{
                util=entry->d_name;
            }

            if(hid || util[0]!='.'){
                if(lstat(util,&sts)==-1){
                    printf("%s\n",path);
                    //printf("%s\n",entry->d_name);
                    printf("%s\n",util);
                    perror("stat list error");

                    for(int i=0;i<sizeD;i++){
                        free(subdirs[i]);
                    }
                    for(int i=0;i<sizeF;i++){
                        free(files[i]);
                    }

                    free(files);
                    free(subdirs);

                    closedir(dir);

                    return;
                }
                if(reca || recb){
                    if(S_ISDIR(sts.st_mode) && strcmp(util, ".")!=0 && strcmp(util, "..")!=0){
                        sizeD++;
                        subdirs=(char **)realloc(subdirs,sizeD*sizeof(char*));
                        sprintf(nombre,"%s/%s",path,util);
                        subdirs[sizeD-1]=strdup(nombre);
                        //subdirs[sizeD-1]=strdup(entry->d_name);
                    }
                    else{
                        sizeF++;
                        files=(char **)realloc(files,sizeF*sizeof(char*));
                        files[sizeF-1]=strdup(util);
                    }
                }
                else{
                    if(largo){
                        getStatsLargo(util,access,enlace);
                    }
                    else{
                        getStats(util);
                    }
                }
            }
        }

        if(reca){
            //padre hijo
            for(int i=0;i<sizeF;i++){
                if(largo){
                    getStatsLargo(files[i],access,enlace);
                }
                else{
                    getStats(files[i]);
                }
            }
            for(int i=0;i<sizeD;i++){
                printf("************%s\n",subdirs[i]);

                listDir(subdirs[i],largo,access,enlace,reca,recb,hid);
            }
        }
        else if(recb){
            //hijo padre
            for(int i=0;i<sizeD;i++){
                //printf("************%s\n",subdirs[i]);

                listDir(subdirs[i],largo,access,enlace,reca,recb,hid);
            }
            printf("************%s\n",path);
            for(int i=0;i<sizeF;i++){
                if(largo){
                    getStatsLargo(files[i],access,enlace);
                }
                else{
                    getStats(files[i]);
                }
            }
        }

        for(int i=0;i<sizeD;i++){
            free(subdirs[i]);
        }
        for(int i=0;i<sizeF;i++){
            free(files[i]);
        }

        free(files);
        free(subdirs);

        chdir(ruta);
        closedir(dir);
    }
    else{
        sprintf(perrormsg,"Imposible abrir %s",path);
        perror(perrormsg);
    }
}

void list(char **trozos, int numWords){
    bool largo=false, access=false, enlace=false, reca=false, recb=false, hid=false;
    int i=0;
    struct stat sts;
    char perrormsg[1024];

    for(i=1;i<numWords;i++){
        if(strcmp(trozos[i],"-long")==0){
            largo=true;
        }
        else if(strcmp(trozos[i],"-acc")==0){
            access=true;
        }
        else if(strcmp(trozos[i],"-link")==0){
            enlace=true;
        }
        else if(strcmp(trozos[i],"-reca")==0){
            reca=true;
        }
        else if(strcmp(trozos[i],"-recb")==0){
            recb=true;
        }
        else if(strcmp(trozos[i],"-hid")==0){
            hid=true;
        }
        else{
            break;
        }
    }

    //i indica aqui el numero de palabras antes de los ficheros, por lo que si i=4 significa que trozos[4] (i) es el primer fichero
    //En caso de que no haya nombre de fichero entonces imprime la ruta 

    if(trozos[i]==NULL){
        printRoute();
    }
    else{
        while(trozos[i]!=NULL){
            if(lstat(trozos[i],&sts)==-1){
                sprintf(perrormsg,"error al acceder a %s",trozos[i]);
                perror(perrormsg);
            }
            else{
                if(S_ISDIR(sts.st_mode)){
                    //Directorio
                    if(reca){
                        printf("************%s\n",trozos[i]);
                    }
                    listDir(trozos[i],largo,access,enlace,reca,recb,hid);
                }
                else{
                    //Archivo
                    if(largo){
                        getStatsLargo(trozos[i],access,enlace);
                    }
                    else{
                        getStats(trozos[i]);
                    }
                }
            }
            i++;
        }
    }
}

void deleteEmpty(char **trozos){
    int i=1;
    char perrormsg[1024];

    while(trozos[i]!=NULL){
        if(remove(trozos[i])==-1){
            sprintf(perrormsg, "Imposible borrar %s", trozos[i]);
            perror(perrormsg);
        }
        i++;
    }
}

void delDir(char *path){
    DIR *dir;
    struct dirent *entry;
    char perrormsg[1024];
    struct stat stats;
    char ruta[1024];

    dir=opendir(path);

    if(dir!=NULL){
        while((entry=readdir(dir))!=NULL){
            if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0){

                getcwd(ruta,1024); //Gardo a ruta actual
                chdir(path); //Entro no directorio que vou eliminar

                if(lstat(entry->d_name,&stats)==-1){
                    perror("stat error");
                    return;
                }
                else{
                    if(S_ISDIR(stats.st_mode)){
                        delDir(entry->d_name);
                    }
                    else{
                        if(remove(entry->d_name)==-1){
                            sprintf(perrormsg, "Imposible borrar %s", path);
                            perror(perrormsg);
                        }
                    }
                }
                chdir(ruta); //Volvo o directorio pai
            }
        }
        closedir(dir);
    }

    if(remove(path)==-1){
        sprintf(perrormsg, "Imposible borrar %s", path);
        perror(perrormsg);
    }
}

void deltree(char **trozos){
    struct stat stats;
    int i=1;
    char perrormsg[1024];

    if(trozos[1]==NULL){
        printRoute();
    }
    
    while(trozos[i]!=NULL){
        if(lstat(trozos[i],&stats)==-1){
            perror("stat error");
        }
        else{
            if(S_ISDIR(stats.st_mode)){
                delDir(trozos[i]);
            }
            else{
                if(remove(trozos[i])==-1){
                    sprintf(perrormsg, "Imposible borrar %s", trozos[i]);
                    perror(perrormsg);
                }
            }
        }
        i++;
    }
}