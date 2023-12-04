#include "librerias.h"

typedef enum status{
    FINISHED, STOPPED, SIGNALED, ACTIVE
} status;

typedef struct tItemP{
    pid_t pid;
    time_t time;
    status estado;
    char *command;
} tItemP;

typedef struct tNodeP *tPosP;

struct tNodeP{
    tItemP data;
    tPosP next;
};

typedef tPosP tListP;

void createEmptyListP(tListP *L);
bool isEmptyListP(tListP L);
tPosP firstP(tListP L);
tPosP lastP(tListP L);
tItemP getItemP(tPosP pos, tListP L);
//findItemP
void deleteAtPositionP(tPosP pos, tListP *L);
void deleteListP(tListP *L);
bool insertItemP(tItemP d, tListP *L);
bool updateItemP(tItemP d, tPosP p, tListP *L);
tPosP nextP(tPosP p, tListP L);