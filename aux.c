#include "aux.h"

void createEmptyListAux(tListAux *L){
    *L=NULL;
}

bool isEmptyListAux(tListAux L){
    return (L==NULL);
}

void deleteListAux(tListAux *L){
    tPosAux p;
    
    while(!isEmptyListAux(*L)){
        p=*L;
        *L=(*L)->next;
        free(p);
    }
}

bool createNodeAux(tPosAux *p){
    *p=malloc(sizeof(**p));
    return *p!=NULL;
}

bool insertItemAux(tAux d, tListAux *L){
    tPosAux q,r;

    if(!createNodeAux(&q)){
        return false;
    }
    else{
        q->data=d;
        q->next=NULL;

        if(*L==NULL){
            *L=q;
        }
        else{
            for(r=*L;r->next!=NULL;r=r->next);
            r->next=q;
        }
        return true;
    }
}