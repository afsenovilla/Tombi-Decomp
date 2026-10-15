// DATA 801357b0 8 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_801359A4[];
extern char D_801359AC[];
extern void *ptrs_801357B0[];

void *ptrs_801357B0[2] = {
    D_801359A4,
    D_801359AC,
};
