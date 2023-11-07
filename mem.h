#include "librerias.h"

typedef struct{
    int df;
    char nombre[1024];
}tFile;

typedef enum{
    MALLOC, SHARED, MMAP
} tAllocType;

typedef struct tItemM{
    void *direccion;
    size_t size;
    time_t allocTime;
    tAllocType allocType;
    union{
        int key;
        tFile file;
    } Type;
} tItemM;

typedef struct tNodeM *tPosM;

struct tNodeM{
    tItemM data;
    tPosM next;
};

typedef tPosM tListM;

void createEmptyListM(tListM *L);
bool isEmptyListM(tListM L);
tPosM firstM(tListM L);
tPosM lastM(tListM L);
tItemM getItemM(tPosM pos, tListM L);
tPosM findItemM(size_t tam, tAllocType allocType, tListM L);
void deleteAtPositionM(tPosM pos, tListM *L);
void deleteListM(tListM *L);
bool insertItemM(tItemM d, tListM *L);
tPosM nextM(tPosM p, tListM L);