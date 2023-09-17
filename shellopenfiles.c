#include "shellopenfiles.h"
#include <stdlib.h>

void createEmptyListF(tListF *L){
    *L=NULL;
}

bool isEmptyListF(tListF L){
    return L==NULL;
}

bool createNodeF(tPosF *p){
    *p=malloc(sizeof(struct tNodeF));
    return p!=NULL;
}

bool insertItemF(tItemF d, tListF *L){
    tPosF q,r;

    if(!createNodeF(&q)){
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

void deleteAtPositionF(tPosF p, tListF *L){
    tPosF q;

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

tPosF findItemF(int df, tListF L){
    tPosF p;

    for(p=L;p!=NULL && p->data.df!=df;p=p->next);

    return p;
}

tPosF firstF(tListF L){
    return L;
}

tPosF nextF(tPosF p, tListF L){
    return p->next;
}

tItemF getItemF(tPosF p, tListF L){
    return (p->data);
}