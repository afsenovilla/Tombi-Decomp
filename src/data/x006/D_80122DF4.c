// DATA 80122df4 56 X006
// Data of X006 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80122E50[];
extern char D_80122E68[];
extern char D_80122E80[];
extern char D_80122E88[];
extern char D_80122EA0[];
extern char D_80122EA8[];
extern char D_80122EB0[];
extern char D_80122EB8[];
extern char D_80122EC8[];
extern char D_80122ED0[];
extern char D_80122F38[];
extern char D_80122F40[];
extern char D_80122F48[];
extern char D_80122F50[];
extern void *ptrs_80122DF4[];
extern void *ptrs_80122E04[];
extern void *ptrs_80122E18[];

void *ptrs_80122DF4[4] = {
    D_80122ED0,
    D_80122F38,
    D_80122F48,
    D_80122F40,
};

void *ptrs_80122E04[5] = {
    D_80122EA0,
    D_80122EA8,
    D_80122EB0,
    D_80122EB8,
    D_80122EC8,
};

void *ptrs_80122E18[5] = {
    D_80122E80,
    D_80122E88,
    D_80122E50,
    D_80122E68,
    D_80122F50,
};
