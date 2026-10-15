// DATA 8012e020 12 X009
// Data of X009 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8012E64C[];
extern char D_8012E654[];
extern char D_8012E700[];
extern void *ptrs_8012E020[];
extern void *ptrs_8012E024[];
extern void *ptrs_8012E028[];

void *ptrs_8012E020[1] = {
    D_8012E700,
};

void *ptrs_8012E024[1] = {
    D_8012E64C,
};

void *ptrs_8012E028[1] = {
    D_8012E654,
};
