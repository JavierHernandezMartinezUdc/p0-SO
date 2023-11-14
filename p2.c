#include "p2.h"

#define TAMANO 2048

//VARIABLES GLOBALES CREADAS
int int_glocal = 0;
char char_glocal = 'r';
double float_glocal = 1.2;

void FechaHora(time_t t, char *dest){
    struct tm *local_time = localtime(&t);
    char mesString[1024];

    if (local_time == NULL) {
        perror("localtime");
        return;
    }

    int dia = local_time->tm_mday;
    int mes = local_time->tm_mon + 1;
    int hor = local_time->tm_hour;
    int min = local_time->tm_min;

    switch (mes) {
        case 1:
            strcpy(mesString,"Ene");
            break;
        case 2:
            strcpy(mesString,"Feb");
            break;
        case 3:
            strcpy(mesString,"Mar");
            break;
        case 4:
            strcpy(mesString,"Abr");
            break;
        case 5:
            strcpy(mesString,"May");
            break;
        case 6:
            strcpy(mesString,"Jun");
            break;
        case 7:
            strcpy(mesString,"Jul");
            break;
        case 8:
            strcpy(mesString,"Ago");
            break;
        case 9:
            strcpy(mesString,"Sep");
            break;
        case 10:
            strcpy(mesString,"Oct");
            break;
        case 11:
            strcpy(mesString,"Nov");
            break;
        case 12:
            strcpy(mesString,"Dic");
            break;
        default:
            strcpy(mesString,"???");
            break;
    }

    sprintf(dest,"%s %d %d:%d",mesString,dia,hor,min);
}

void list_print(tAllocType ref, tListM M){
    tPosM p;
    tItemM m;
    char tipo[7];
    char fecha[13];

    if(ref==null){
        for(p=firstM(M);p!=NULL;p=nextM(p,M)){
            m=getItemM(p,M);
            FechaHora(m.allocTime,fecha);

            if(m.allocType==MALLOC){
                strcpy(tipo,"malloc");
            }
            else if(m.allocType==SHARED){
                strcpy(tipo,"shared");
            }
            else if(m.allocType==MMAP){
                strcpy(tipo,"mmap");
            }
            printf("      %p\t\t%d (%s) %s\n",m.direccion,(int)m.size,fecha,tipo);
        }
    }
    else{
        for(p=firstM(M);p!=NULL;p=nextM(p,M)){
            m=getItemM(p,M);
            FechaHora(m.allocTime,fecha);

            if(m.allocType==ref){
                if(ref==MALLOC){
                    strcpy(tipo,"malloc");
                }
                else if(ref==SHARED){
                    strcpy(tipo,"shared");
                }
                else if(ref==MMAP){
                    strcpy(tipo,"mmap");
                }
                printf("      %p\t\t%d (%s) %s\n",m.direccion,(int)m.size,fecha,tipo);
            }
        }
    }
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam,tListM *M)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    tItemM item;

    if (tam)     /* tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
    {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);

    item.allocType=SHARED;
    item.size=tam;
    time(&item.allocTime);
    item.Type.key=clave;
    item.direccion=p;
    insertItemM(item, M);
    
    return (p);
}
void SharedCreate (char *tr[],tListM *M)
{
    key_t cl;
    size_t tam;
    void *p;

    cl=(key_t)  strtoul(tr[2],NULL,10);
    tam=(size_t) strtoul(tr[3],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p=ObtenerMemoriaShmget(cl,tam,M))!=NULL)
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void SharedDelkey (char *args[])
{
    key_t clave;
    int id;
    char *key=args[2];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar id de memoria compartida\n");
}

void shared (char **trozos, tListM *M){
    tPosM p;
    tItemM x;
    void *q;

    if(trozos[1]==NULL){
        printf("******Lista de bloques asignados shared para el proceso %d\n",getpid());
        list_print(SHARED,*M);
    }
    else if (strcmp(trozos[1], "-create") == 0) {
        if (trozos[2]==NULL || trozos[3]==NULL) {
            printf("******Lista de bloques asignados shared para el proceso %d\n",getpid());
            list_print(SHARED,*M);
            return;
        }
        SharedCreate(trozos,M);}
    else if (strcmp(trozos[1], "-delkey") == 0) {SharedDelkey(trozos);}
    else if (strcmp(trozos[1], "-free") == 0) {
        p=findItemSharedM(atoi(trozos[2]), SHARED, *M);

        if(p==NULL){
            printf("No hay bloque de esa clave mapeado en el proceso\n");
            return;
        }

        x=getItemM(p,*M);
        if (shmdt(x.direccion) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

        deleteAtPositionM(p,M);
    }
    else{
        //TODO Esto da error por algun motivo
        struct shmid_ds s;;
        if (shmctl(atoi(trozos[1]), IPC_STAT, &s) == -1) {
            perror("shmctl info");
            return;
        }

        //Asignar memoria a partir de una clave
        q=shmat(atoi(trozos[1]),NULL,0777);

        x.direccion=q;
        time(&x.allocTime);
        x.size=s.shm_segsz;
        x.allocType=SHARED;
        x.Type.key=atoi(trozos[1]);

        printf("Memoria compartida en clave %d en %p",atoi(trozos[1]),q);
    }
}
/*
                |####################################################################################################|
                |--------------------------------------DESDE AQUI TODO CORRECTO--------------------------------------|
                |####################################################################################################|
*/
void mallocCmd(char **trozos, tListM *M){
    tItemM m;

    if(trozos[1]==NULL){
        printf("******Lista de bloques asignados malloc para el proceso %d\n",getpid());
        list_print(MALLOC,*M);
    }
    else if(strcmp(trozos[1],"-free")==0){
        tPosM p;

        //Libera a primeira que encontra, a mais antigua
        p=findItemMallocM(atoi(trozos[2]),MALLOC,*M);
        if(p==NULL){
            printf("No hay bloque de ese tamano asignado con malloc\n");
        }
        else{
            m=getItemM(p,*M);
            free(m.direccion);
            deleteAtPositionM(p, M);
        }
    }
    else{
        if(atoi(trozos[1])!=0){
            if((m.direccion=calloc(atoi(trozos[1]),1))==NULL){
                perror("Sin memoria");
            }
            else{
                m.size=atoi(trozos[1]);
                time(&m.allocTime);
                m.allocType=MALLOC;
                if(insertItemM(m,M)==false){
                    printf("Imposible hacer malloc\n");
                }
                else{
                    printf("Asignados %d bytes en %p\n",(int)m.size,m.direccion);
                }
            }
        }
        else{
            printf("No se asignan bloques de 0 bytes\n");
        }
    }
}

ssize_t LeerFichero (char *f, void *p, size_t cont)
{
   struct stat s;
   ssize_t  n;  
   int df,aux;

   if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	    return -1;     
   if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	    cont=s.st_size;
   if ((n=read(df,p,cont))==-1){
	    aux=errno;
	    close(df);
	    errno=aux;
	    return -1;
   }
   close (df);
   return n;
}

void CmdRead (char *ar[])
{
   void *p;
   size_t cont=-1;  /* -1 indica leer todo el fichero*/
   ssize_t n;

   if (ar[1]==NULL || ar[2]==NULL){
        printf ("Faltan parametros\n");
        return;
   }
   //p=cadtop(ar[2]);  /*convertimos de cadena a puntero*/
   p=(void *)strtol(ar[2],NULL,16);
   if (ar[3]!=NULL)
	    cont=(size_t) atoll(ar[3]);

   if ((n=LeerFichero(ar[1],p,cont))==-1)
	    perror ("Imposible leer fichero");
   else
	    printf ("Leidos %lld bytes de %s en %p\n",(long long) n,ar[1],p);
}

void EscribirFichero(char *f, void *p, size_t cont, bool o){
    char ruta[1024];
    getcwd(ruta,1024);

    DIR *dir=opendir(ruta);
    struct dirent *entrada;
    bool existe=false;
    int df;

    if(dir==NULL){
        perror("Error al abrir el directorio");
        return;
    }

    while((entrada=readdir(dir))!=NULL){
        if(strcmp(entrada->d_name,f)==0){
            existe=true;
        }
    }

    closedir(dir);

    if(existe && o){
        FILE *archivo=fopen(f,"w");
        if(archivo==NULL){
            perror("Error al abrir");
            return;
        }

        //Escribir archivo
        size_t escritos=fwrite((char *)p, sizeof(char), cont, archivo);
        if(escritos!=cont){
            perror("Error al escribir");
            fclose(archivo);
            return;
        }

        //Cerrar archivo
        fclose(archivo);

        printf("Escritos %lld bytes en %s en %p\n",(long long)escritos, f, p);
    }
    else{
        df=open(f,O_CREAT | O_EXCL, 0777);
        if(df==-1){
            perror("Imposible crear");
            return;
        }
        else{
            close(df);
        }

        //Abrir archivo
        FILE *archivo=fopen(f,"w");
        if(archivo==NULL){
            perror("Error al abrir");
            return;
        }

        //Escribir archivo
        size_t escritos=fwrite((char *)p, sizeof(char), cont, archivo);
        if(escritos!=cont){
            perror("Error al escribir");
            fclose(archivo);
            return;
        }

        //Cerrar archivo
        fclose(archivo);

        printf("Escritos %lld bytes en %s en %p\n",(long long)escritos, f, p);
    }

}

void CmdWrite(char **trozos){
    void *p;
    
    if(strcmp(trozos[1],"-o")==0){
        if(trozos[4]==NULL){
            printf("Faltan parametros\n");
            return;
        }
        p=(void *)strtol(trozos[3],NULL,16);
        EscribirFichero(trozos[2],p,atoi(trozos[4]),true);
    }
    else{
        if(trozos[3]==NULL){
            printf("Faltan parametros\n");
            return;
        }
        p=(void *)strtol(trozos[2],NULL,16);
        EscribirFichero(trozos[1],p,atoi(trozos[3]),false);
    }

}

void memDump(void *address, size_t size){
    unsigned char *ptr = (unsigned char *)address;
    int veces=0,pos_char,lineas=0;
    size_t i,j;
    
    for(i=0;i<size;i++){
        if(ptr[i]>=32 && ptr[i]<=126){
            printf("%4c",ptr[i]);
        }else if(ptr[i]==10){ //Caso particular para \n
            printf("%4s","\\n");
        }else{
            printf("    ");
        }
        veces++;

        if((veces%25==0 && veces!=0) || veces+25*lineas==size){
            lineas++;
            printf("\n");
            pos_char=i+1-veces;;
            for(j=pos_char;j<pos_char+veces;j++){
                if(ptr[j]==0){ 
                    printf("  00");
                }
                else{
                    printf("  %02X",ptr[j]);
                }
                //printf("  %02X",ptr[j]);
            }
            printf("\n");
            veces=0;
        }
    }
}

void memDumpCmd(char **trozos){
    void *p;

    p=(void *)strtol(trozos[1],NULL,16);

    if(trozos[1]!=NULL){
        if(trozos[2]==NULL){
            memDump(p,25);
        }
        else{
            memDump(p,atoi(trozos[2]));
        }
    }
}

void LlenarMemoria (void *p, size_t cont, unsigned char byte){
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  printf("Llenando %d bytes de memoria con el byte %c(%02x) a partir de la direccion %p\n",(int)cont, byte, byte, p);

  for (i=0; i<cont;i++)
		arr[i]=byte;
}

void memFillCmd(char **trozos, int numWords){
    void *p;
    unsigned char porDefecto = 'A';

    if(numWords==1 || numWords>4){
        printf("Numero de argumentos no validos\n");
    }
    else{
        p=(void *)strtol(trozos[1],NULL,16);

        if(numWords==2){
            //printf("Llenando 128 bytes de memoria con el byte %c(%x) a partir de la direccion %p\n",porDefecto,porDefecto,p);
            LlenarMemoria(p, 128, porDefecto);
        }
        else if(numWords==3){
            //printf("Llenando %d bytes de memoria con el byte %c(%x) a partir de la direccion %p\n",atoi(trozos[2]),porDefecto,porDefecto,p);
            LlenarMemoria(p, atoi(trozos[2]), porDefecto);
        }
        else{
            if(trozos[3][1]=='x'){
                //printf("Llenando %d bytes de memoria con el byte %c(%x) a partir de la direccion %p\n",atoi(trozos[2]),(int)strtol(trozos[3],NULL,16),(int)strtol(trozos[3],NULL,16),p);
                LlenarMemoria(p, atoi(trozos[2]), strtol(trozos[3],NULL,16));
            }
            else{
                //printf("Llenando %d bytes de memoria con el byte %c(%x) a partir de la direccion %p\n",atoi(trozos[2]),(int)strtol(trozos[3],NULL,10),(int)strtol(trozos[3],NULL,10),p);
                LlenarMemoria(p, atoi(trozos[2]), strtol(trozos[3],NULL,10));
            }
        }
    }
}

void Do_MemPmap (void) /*sin argumentos*/
{ pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){ /*proceso hijo*/
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        exit(1);
    }
    waitpid (pid,NULL,0);
}

void mem(char **trozos, tListM M){ //TODO cambiar un pouco estructura pa evitar a copia
    bool blocks = false, funcs = false, vars = false, pmap = false;

    //Variables locales creadas
    int int_local = 0;
    char char_local = 'r';
    double float_local = 1.2;

    //Variables estáticas creadas
    static int int_slocal = 0;
    static char char_slocal = 'r';
    static double float_slocal = 1.2;

    if (trozos[1]==NULL || (strcmp(trozos[1], "-all")==0)){
        blocks = true;
        funcs = true;
        vars = true;
    }
    else if (strcmp(trozos[1], "-blocks")==0){
        blocks = true;
    }
    else if (strcmp(trozos[1], "-funcs")==0){
        funcs = true;
    }
    else if (strcmp(trozos[1], "-vars")==0){
        vars = true;
    }
    else if (strcmp(trozos[1], "-pmap")==0){
        pmap = true;
    }
    else {
        printf("%s no es una opción\n", trozos[1]);
    }

    if(vars){
        printf("Variables locales\t%p,\t%p,\t%p\n", &int_local, &char_local, &float_local);
        printf("Variables globales\t%p,\t%p,\t%p\n", &int_glocal, &char_glocal, &float_glocal);
        //printf("Var (N.I) globales\t%p,\t%p,\t%p\n", );
        printf("Variables estaticas\t%p,\t%p,\t%p\n", &int_slocal, &char_slocal, &float_slocal);
        //printf("Var (N.I) estaticas\t%p,\t%p,\t%p\n", );
    }
    if(funcs){
        printf("Funciones programa\t%p,\t%p,\t%p\n", &mallocCmd, &LlenarMemoria, &EscribirFichero);
        printf("Funciones libreria\t%p,\t%p,\t%p\n", &printf, &calloc, &shmget);
    }
    if(blocks){
        printf("******Lista de bloques asignados para el proceso %d\n", getpid());
        list_print(null,M);
    }
    if (pmap){
        Do_MemPmap();
    }
    vars = false;
    funcs = false;
    blocks = false;
    pmap = false;
}

void Recursiva (int n){
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}