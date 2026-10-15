// DATA 80133b98 32 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80133CFC[];
extern char D_80133D40[];
extern char D_80133D50[];
extern char D_80133E04[];
extern char D_80133E28[];
extern char D_801344A8[];
extern char D_801344C0[];
extern char D_801344D0[];
extern void *ptrs_80133B98[];
extern void *ptrs_80133B9C[];
extern void *ptrs_80133BA0[];
extern void *ptrs_80133BA4[];
extern void *ptrs_80133BA8[];
extern void *ptrs_80133BB4[];

void *ptrs_80133B98[1] = {
    D_80133E04,
};

void *ptrs_80133B9C[1] = {
    D_80133E28,
};

void *ptrs_80133BA0[1] = {
    D_80133CFC,
};

void *ptrs_80133BA4[1] = {
    D_80133D40,
};

void *ptrs_80133BA8[3] = {
    D_80133D50,
    D_801344A8,
    D_801344C0,
};

void *ptrs_80133BB4[1] = {
    D_801344D0,
};
