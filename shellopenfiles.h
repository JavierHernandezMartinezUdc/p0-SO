#ifndef P0_SHELLOPENFILES_H
#define P0_SHELLOPENFILES_H
#include <stdbool.h>

typedef struct tItemF{
    int df;
    char nombre[4096]; //tamaño maximo de una ruta en linux
    char mode[9]; //nombre de modo mas largo
} tItemF;

typedef struct tNodeF* tPosF;
struct tNodeF{
    tPosF next;
    tItemF data;
};
typedef tPosF tListF;

void createEmptyListF(tListF *L);
bool isEmptyListF(tListF L);
bool insertItemF(tItemF d, tListF *L);
void deleteAtPositionF(tPosF p, tListF *L);
tPosF findItemF(int df, tListF L);
tPosF firstF(tListF L);
tPosF nextF(tPosF p, tListF L);
tItemF getItemF(tPosF p, tListF L);

#endif //P0_SHELLOPENFILES_H