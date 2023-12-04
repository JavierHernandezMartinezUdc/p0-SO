#include "proc.h"

void createEmptyListP(tListP *L){
    *L=NULL;
}

bool isEmptyListP(tListP L){
    return (L==NULL);
}

tPosP firstP(tListP L){
    return L;
}

tPosP lastP(tListP L){
    tPosP p;
    for(p=firstP(L);p->next!=NULL;p=p->next);
    return p;
}

tItemP getItemP(tPosP pos, tListP L){
    return pos->data;
}

//findItemP

void deleteAtPositionP(tPosP pos, tListP *L){
    tPosP q;

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

void deleteListP(tListP *L){
    tPosP p;
    while(!isEmptyListP(*L)){
        p=*L;
        *L=(*L)->next;
        free(p);
    }
}

bool createNodeP(tPosP *p){
    *p=malloc(sizeof(**p));
    return *p!=NULL;
}

bool insertItemP(tItemP d, tListP *L){
    tPosP q,r;

    if(!createNodeP(&q)){
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

bool updateItemP(tItemP d, tPosP p, tListP *L){
    tPosP i;

    for(i = *L;(i != NULL) && (i != p); i = i -> next);
    if (i != NULL) {
        i->data = d;
        return true;
    }
    return false;
}

tPosP nextP(tPosP p, tListP L){
    return p->next;
}