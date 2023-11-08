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
            if((m.direccion=malloc(atoi(trozos[1])))==NULL){
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

void memDump(void *address, size_t size){
    unsigned char *ptr = (unsigned char *)address;
    int veces=0,pos_char;
    size_t i,j;
    
    for(i=0;i<size;i++){
        if(ptr[i]>=32 && ptr[i]<=126){
            printf("%4c",ptr[i]);
        }else{
            printf("    ");
        }
        veces++;

        if(veces%25==0 && veces!=0){
            printf("\n");
            pos_char=i+1-veces;;
            for(j=pos_char;j<pos_char+veces;j++){
                /* TODO preguntar por calloc
                if(ptr[j]==0){ 
                    printf("  00");
                }
                else{
                    printf("  %02X",ptr[j]);
                }
                */
                printf("  %02X",ptr[j]);
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

  printf("Llenando %d bytes de memoria con el byte %c(%x) a partir de la direccion %p\n",(int)cont, byte, byte, p);

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