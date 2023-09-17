#include "historic.h"
#include <stdlib.h>

void createEmptyListH(tListH *L) {
    *L = NULL;
}

tPosH firstH(tListH L) {
    return L;
}

tPosH nextH(tPosH p, tListH L) {
    return p -> next;
}

tPosH lastH(tListH L) {
    tPosH p;
    for(p = firstH(L); p -> next != NULL; p = p -> next);
    return p;
}
/*
tPosH previousH(tPosH p, tListH L) {
    tPosH q;
    if(p == firstH(L)) {
        return NULL;
    } else {
        for(q = firstH(L); q -> next != p; q = nextH(q,L));
        return q;
    }
}
*/
tPosH findItemH(int d, tListH L) {
    tPosH p;
    for(p = L;(p != NULL) && (p -> data.id != d); p = p -> next);
    return p;
}

tItemH getItemH(tPosH p, tListH L) {
    return p -> data;
}

bool isEmptyListH(tListH L) {
    return (L == NULL);
}
/*
void deleteAtPositionH(tPosH p, tListH *L) {
    tPosH q;
    if(p == firstH(*L)) {
        *L = (*L) -> next;
    } else {
        if(p -> next == NULL) {
            for(q = *L; q -> next != p; q = q-> next);
            q -> next = NULL;
        } else {
            q = p -> next;
            p -> data = q -> data;
            p -> next = q -> next;
            p = q;
        }
    }
    free(p);
    p = NULL;
}
*/
bool createNodeH(tPosH *p) {
    *p = malloc(sizeof(**p));
    return *p != NULL;
}

bool insertItemH(tItemH d, tListH *L){
    tPosH q,r;

    if(!createNodeH(&q)){
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

void deleteListH(tListH *L){
    tPosH p;
    while (*L != NULL){
        p=*L;
        *L=(*L)->next;
        free(p);
    }
}
