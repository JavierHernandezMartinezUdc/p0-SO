#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> //Utilizase en open
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <libgen.h>
#include <math.h>
#include <sys/utsname.h> //Utilizase en infosys
#include <sys/wait.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/resource.h>
//#include <signal.h> Por algun motivo no funciona si se incluye aqui