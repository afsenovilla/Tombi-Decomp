// DATA 80134d0c 24 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80134E3C[];
extern char D_80134E44[];
extern char D_80134E4C[];
extern char D_80134EBC[];
extern char D_80134F3C[];
extern char D_80134F44[];
extern void *ptrs_80134D0C[];
extern void *ptrs_80134D18[];
extern void *ptrs_80134D1C[];
extern void *ptrs_80134D20[];

void *ptrs_80134D0C[3] = {
    D_80134EBC,
    D_80134F3C,
    D_80134F44,
};

void *ptrs_80134D18[1] = {
    D_80134E3C,
};

void *ptrs_80134D1C[1] = {
    D_80134E44,
};

void *ptrs_80134D20[1] = {
    D_80134E4C,
};
