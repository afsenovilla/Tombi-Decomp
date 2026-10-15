// DATA 801159e8 32 X010
// Data of X010 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80131D4C[];
extern char D_80131D68[];
extern char D_80131D84[];
extern char D_80131DA0[];
extern void *ptrs_801159E8[];

void *ptrs_801159E8[8] = {
    D_80131D4C,
    D_80131D84,
    D_80131D68,
    D_80131DA0,
    D_80131D4C,
    D_80131D84,
    D_80131D68,
    D_80131DA0,
};
