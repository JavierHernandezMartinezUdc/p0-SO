#include "p2.h"

#define TAMANO 2048

void mallocCmd(char **trozos, tListM *M){
    tItemM m;

    if(strcmp(trozos[1],"-free")==0){
        tPosM p;

        //liberar a primeira que encontre
        p=findItemM(atoi(trozos[2]),MALLOC,*M);
        m=getItemM(p,*M);
        free(m.direccion);
        deleteAtPositionM(p, M);
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

void memFill (void *p, size_t cont, unsigned char byte){
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
		arr[i]=byte;
}

void memFill2 (char **trozos){
    void *p=(void *)strtoull(trozos[1], NULL, 16);;
    size_t cont=atoi(trozos[2]);
    unsigned char byte=trozos[3][0];

  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
		arr[i]=byte;
}

void Do_MemPmap(){
   pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
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

void Recursiva (int n){
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}