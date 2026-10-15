// DATA 8001473c 8 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8001476C[];
extern char D_80014774[];
extern void *ptrs_8001473C[];
extern void *ptrs_80014740[];

void *ptrs_8001473C[1] = {
    D_8001476C,
};

void *ptrs_80014740[1] = {
    D_80014774,
};
