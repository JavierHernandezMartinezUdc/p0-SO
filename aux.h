#include "librerias.h"

typedef void* tAux;

typedef struct tNodeAux *tPosAux;

struct tNodeAux{
    tAux data;
    tPosAux next;
};

typedef tPosAux tListAux;

void createEmptyListAux(tListAux *L);
bool isEmptyListAux(tListAux L);
void deleteListAux(tListAux *L);
bool insertItemAux(tAux d, tListAux *L);