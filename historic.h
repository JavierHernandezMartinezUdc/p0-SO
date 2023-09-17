#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define NAME_LENGTH_LIMIT 25
#define CNULL NULL

typedef struct tItem {
    int id;
    char nombre[NAME_LENGTH_LIMIT];
} tItem;

typedef struct tNode *tPos;

struct tNode {
    tItem data;
    tPos next;
};

typedef tPos tList;

void createEmptyList(tList *L);
bool isEmptyList(tList L);
tPos first(tList L);
tPos last(tList L);
tPos next(tPos p, tList L);
tPos previous(tPos p, tList L);
tPos findItem(int d, tList L);
bool insertItem(tItem t, tPos p, tList *L);
void deleteAtPosition (tPos p, tList *L);
tItem getItem(tPos p, tList L);
void updateItem(tItem i, tList *L);
void deleteList(tList *L);
#endif
