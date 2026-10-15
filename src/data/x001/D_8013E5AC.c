// DATA 8013e5ac 16 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013E7B8[];
extern char D_8013E7C0[];
extern char D_8013E7C8[];
extern char D_8013E7D0[];
extern void *ptrs_8013E5AC[];
extern void *ptrs_8013E5B0[];

void *ptrs_8013E5AC[1] = {
    D_8013E7B8,
};

void *ptrs_8013E5B0[3] = {
    D_8013E7C0,
    D_8013E7C8,
    D_8013E7D0,
};
