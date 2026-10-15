// DATA 8013e6d8 12 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013E97C[];
extern char D_8013E990[];
extern char D_8013E9B4[];
extern void *ptrs_8013E6D8[];
extern void *ptrs_8013E6DC[];
extern void *ptrs_8013E6E0[];

void *ptrs_8013E6D8[1] = {
    D_8013E97C,
};

void *ptrs_8013E6DC[1] = {
    D_8013E990,
};

void *ptrs_8013E6E0[1] = {
    D_8013E9B4,
};
