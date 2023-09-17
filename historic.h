#ifndef LIST_H
#define LIST_H
#include <stdbool.h>

#define NAME_LENGTH_LIMIT 25

typedef struct tItemH {
    int id;
    char nombre[NAME_LENGTH_LIMIT];
} tItemH;

typedef struct tNodeH *tPosH;

struct tNodeH {
    tItemH data;
    tPosH next;
};

typedef tPosH tListH;

void createEmptyListH(tListH *L);
bool isEmptyListH(tListH L);
tPosH firstH(tListH L);
tPosH lastH(tListH L);
tPosH nextH(tPosH p, tListH L);
//tPosH previousH(tPosH p, tListH L);
tPosH findItemH(int d, tListH L);
bool insertItemH(tItemH t, tListH *L);
void deleteAtPositionH(tPosH p, tListH *L);
tItemH getItemH(tPosH p, tListH L);
void deleteListH(tListH *L);
#endif
