// DATA 8013577c 8 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_801358D0[];
extern char D_801358D8[];
extern void *ptrs_8013577C[];

void *ptrs_8013577C[2] = {
    D_801358D0,
    D_801358D8,
};
