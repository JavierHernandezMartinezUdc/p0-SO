#include "historic.h"

void createEmptyList(tList *L) {
    *L = CNULL;
}

tPos first(tList L) {
    return L;
}

tPos next(tPos p, tList L) {
    return p -> next;
}

tPos last(tList L) {
    tPos p;
    for(p = first(L); p -> next != CNULL; p = p -> next);
    return p;
}

tPos previous(tPos p, tList L) {
    tPos q;
    if(p == first(L)) {
        return CNULL;
    } else {
        for(q = first(L); q -> next != p; q = next(q,L));
        return q;
    }
}

tPos findItem(int d, tList L) {
    tPos p;
    for(p = L;(p != CNULL) && (p -> data.id != d); p = p -> next);
    return p;
}

tItem getItem(tPos p, tList L) {
    return p -> data;
}

bool isEmptyList(tList L) {
    return (L == CNULL);
}

void updateItem (tItem d, tPos p, tList *L) {
    p -> data = d;
}

void deleteAtPosition (tPos p, tList *L) {
    tPos q;
    if(p == first(*L)) {
        *L = (*L) -> next;
    } else {
        if(p -> next == CNULL) {
            for(q = *L; q -> next != p; q = q-> next);
            q -> next = CNULL;
        } else {
            q = p -> next;
            p -> data = q -> data;
            p -> next = q -> next;
            p = q;
        }
    }
    free(p);
    p = CNULL;
}

bool createNode(tPos *p) {
    *p = malloc(sizeof(**p));
    return *p != CNULL;
}

bool insertItem(tItem d, tList *l) {
    tPos q, r;
    if (!createNode(&q)) {
        return false;
    } else {
        q->data = d;
        q->next = CNULL;
        if (*l == CNULL) {
            *l = q;
        } else {
                r = *l;
                while (r->next != CNULL)
                    r = r->next;
                r->next = q;
            }
        }
        return true;
    }
}

void deleteList(tList *L){
    tPos p;
    while (*L != CNULL){
        p=*L;
        *L=(*L)->next;
        free(p);
    }
}
