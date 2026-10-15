// DATA 8013b1b4 8 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013B690[];
extern char D_8013B6AC[];
extern void *ptrs_8013B1B4[];
extern void *ptrs_8013B1B8[];

void *ptrs_8013B1B4[1] = {
    D_8013B690,
};

void *ptrs_8013B1B8[1] = {
    D_8013B6AC,
};
