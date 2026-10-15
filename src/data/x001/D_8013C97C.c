// DATA 8013c97c 8 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013E670[];
extern char D_8013E67C[];
extern void *ptrs_8013C97C[];

void *ptrs_8013C97C[2] = {
    D_8013E670,
    D_8013E67C,
};
