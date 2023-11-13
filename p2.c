#include "p2.h"

#define TAMANO 2048

void mallocCmd(char **trozos, tListM *M){
    tItemM m;

    if(strcmp(trozos[1],"-free")==0){
        tPosM p;

        //Libera a primeira que encontra, a mais antigua
        p=findItemM(atoi(trozos[2]),MALLOC,*M);
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

void Recursiva (int n){
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}