#ifndef P0_SHELLOPENFILES_H
#define P0_SHELLOPENFILES_H
#include <stdbool.h>

typedef struct tItem{
    int df;
    char nombre[4096]; //tamaño maximo de una ruta en linux
    char mode[9]; //nombre de modo mas largo
} tItem;

typedef struct tNode* tPos;
struct tNode{
    tPos next;
    tItem data;
};
typedef tPos tList;

void createEmptyList(tList *L);
bool isEmptyList(tList L);
bool insertItem(tItem d, tList *L);
void deleteAtPosition(tPos p, tList *L);
tPos findItem(int df, tList L);
tPos first(tList L);
tPos next(tPos p, tList L);
tItem getItem(tPos p, tList L);

#endif //P0_SHELLOPENFILES_H