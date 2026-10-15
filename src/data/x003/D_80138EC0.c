// DATA 80138ec0 8 X003
// Data of X003 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_801390A0[];
extern char D_801390E0[];
extern void *ptrs_80138EC0[];

void *ptrs_80138EC0[2] = {
    D_801390A0,
    D_801390E0,
};
