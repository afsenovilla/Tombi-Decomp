// DATA 8013e5dc 12 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013E7F8[];
extern char D_8013E800[];
extern char D_8013E808[];
extern void *ptrs_8013E5DC[];

void *ptrs_8013E5DC[3] = {
    D_8013E7F8,
    D_8013E800,
    D_8013E808,
};
