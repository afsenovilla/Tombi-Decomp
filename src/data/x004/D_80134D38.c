// DATA 80134d38 24 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80134E54[];
extern char D_80134E5C[];
extern char D_80134E64[];
extern char D_80134E6C[];
extern char D_80134EFC[];
extern char D_80134F04[];
extern void *ptrs_80134D38[];
extern void *ptrs_80134D44[];

void *ptrs_80134D38[3] = {
    D_80134E6C,
    D_80134E54,
    D_80134E5C,
};

void *ptrs_80134D44[3] = {
    D_80134E64,
    D_80134EFC,
    D_80134F04,
};
