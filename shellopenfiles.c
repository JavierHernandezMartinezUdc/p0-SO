#include "shellopenfiles.h"
#include <stdlib.h>

void createEmptyList(tList *L){
    *L=NULL;
}

bool createNode(tPos *p){
    *p=malloc(sizeof(struct tNode));
    return p!=NULL;
}

bool insertItem(tItem d, tList *L){
    tPos q,r;

    if(!createNode(&q)){
        return false;
    }
    else {
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

void deleteAtPosition(tPos p, tList *L){
    tPos q;

    if(p==*L){
        *L=(*L)->next;
    }
    else if(p->next==NULL){
        for(q=*L;q->next!=p;q=q->next);
        q->next=NULL;
    }
    else{
        q=p->next;
        p->data=q->data;
        p->next=q->next;
        p=q;
    }

    free(p);
}

tPos findItem(int df, tList L){
    tPos p;

    for(p=L;p!=NULL && p->data.df!=df;p=p->next);

    return p;
}

tPos first(tList L){
    return L;
}

tPos next(tPos p, tList L){
    return p->next;
}

tItem getItem(tPos p, tList L){
    return (p->data);
}