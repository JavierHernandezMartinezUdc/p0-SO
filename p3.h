#include "librerias.h"
#include "proc.h"

void uid(char **trozos);
void showvar(char **trozos, char **arg3, char **environ);
void changevar(char **trozos, char **arg3, char **environ);
void subsvar(char **trozos, char **arg3, char **environ);
void showenv(char **trozos, char **arg3, char **environ);
void Cmd_fork (tListP *P);
void exec(char **trozos, int numWords);