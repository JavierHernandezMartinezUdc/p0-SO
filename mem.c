#include "mem.h"

void createEmptyListM(tListM *L){
    *L=NULL;
}

bool isEmptyListM(tListM L){
    return (L==NULL);
}

tPosM firstM(tListM L){
    return L;
}

tPosM lastM(tListM L){
    tPosM p;
    for(p=firstM(L);p->next!=NULL;p=p->next);
    return p;
}

tItemM getItemM(tPosM pos, tListM L){
    return pos->data;
}

tPosM findItemMallocM(size_t tam, tListM L) {
    tPosM p;
    for(p = L;(p != NULL) && (p -> data.size != tam); p = p -> next);
    return p;
}

tPosM findItemSharedM(int key, tListM L) {
    tPosM p=NULL;
    for(p = L;(p != NULL) && (p -> data.Type.key != key); p = p -> next);
    return p;
}

tPosM findItemMmapM(char *nombre, tListM L){
    tPosM p=NULL;
    for(p = L;(p != NULL) && (strcmp(p->data.Type.file.nombre,nombre)!=0); p = p -> next);
    return p;
}

void deleteAtPositionM(tPosM pos, tListM *L){
    tPosM q;

    if(pos==*L){
        *L=(*L)->next;
    }
    else if(pos->next==NULL){
        for(q=*L;q->next!=pos;q=q->next);
        q->next=NULL;
    }
    else{
        q=pos->next;
        pos->data=q->data;
        pos->next=q->next;
        pos=q;
    }

    free(pos);
}

void deleteListM(tListM *L){
    tPosM p;
    while(!isEmptyListM(*L)){
        p=*L;
        *L=(*L)->next;
        free(p);
    }
}

bool createNodeM(tPosM *p){
    *p=malloc(sizeof(**p));
    return *p!=NULL;
}

bool insertItemM(tItemM d, tListM *L){
    tPosM q,r;

    if(!createNodeM(&q)){
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

tPosM nextM(tPosM p, tListM L){
    return p->next;
}