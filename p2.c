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
        if((m.direccion=malloc(atoi(trozos[1])))==NULL){
            perror("Sin memoria");
        }
        else{
            m.size=atoi(trozos[1]);
            time(&m.allocTime);
            m.allocType=MALLOC;
            insertItemM(m,M);
            printf("Asignados %d bytes en %p\n",(int)m.size,m.direccion);
        }
    }
}

void LlenarMemoria (void *p, size_t cont, unsigned char byte){
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
		arr[i]=byte;
}

void memFillCmd(char **trozos){
    //Control de datos
    //LlenarMemoria(); Chamamos a funcion despois do control
}

void Recursiva (int n){
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}