// DATA 80133b74 16 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80133C74[];
extern char D_80133CB8[];
extern char D_801340EC[];
extern char D_8013413C[];
extern void *ptrs_80133B74[];
extern void *ptrs_80133B78[];
extern void *ptrs_80133B80[];

void *ptrs_80133B74[1] = {
    D_801340EC,
};

void *ptrs_80133B78[2] = {
    D_8013413C,
    D_80133C74,
};

void *ptrs_80133B80[1] = {
    D_80133CB8,
};
