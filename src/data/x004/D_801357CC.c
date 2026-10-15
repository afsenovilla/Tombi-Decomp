// DATA 801357cc 20 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80135A10[];
extern char D_80135A34[];
extern char D_80135A3C[];
extern char D_80135A74[];
extern char D_80135A7C[];
extern void *ptrs_801357CC[];
extern void *ptrs_801357D8[];
extern void *ptrs_801357DC[];

void *ptrs_801357CC[3] = {
    D_80135A10,
    D_80135A34,
    D_80135A3C,
};

void *ptrs_801357D8[1] = {
    D_80135A74,
};

void *ptrs_801357DC[1] = {
    D_80135A7C,
};
