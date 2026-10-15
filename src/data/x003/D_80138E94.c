// DATA 80138e94 12 X003
// Data of X003 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80139010[];
extern char D_80139018[];
extern char D_80139020[];
extern void *ptrs_80138E94[];
extern void *ptrs_80138E98[];
extern void *ptrs_80138E9C[];

void *ptrs_80138E94[1] = {
    D_80139010,
};

void *ptrs_80138E98[1] = {
    D_80139020,
};

void *ptrs_80138E9C[1] = {
    D_80139018,
};
