#include "librerias.h"
#include "mem.h"

void mallocCmd(char **trozos, tListM *M);
void shared(char **trozos, tListM *M);
void CmdMmap(char *arg[], tListM *M);
void CmdRead (char *ar[]);
void CmdWrite(char **trozos);
void memDumpCmd(char **trozos);
void memFillCmd(char **trozos, int numWords);
void mem(char **trozos, tListM M);
void Recursiva(int n);