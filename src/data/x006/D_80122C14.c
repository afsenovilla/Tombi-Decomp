// DATA 80122c14 8 X006
// Data of X006 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80122D04[];
extern char D_80122D28[];
extern void *ptrs_80122C14[];
extern void *ptrs_80122C18[];

void *ptrs_80122C14[1] = {
    D_80122D04,
};

void *ptrs_80122C18[1] = {
    D_80122D28,
};
