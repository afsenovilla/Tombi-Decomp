// DATA 8013b17c 40 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013B874[];
extern char D_8013B87C[];
extern char D_8013B88C[];
extern char D_8013B8A0[];
extern char D_8013B8C0[];
extern char D_8013B8E8[];
extern char D_8013B900[];
extern char D_8013B910[];
extern char D_8013B918[];
extern char D_8013B920[];
extern void *ptrs_8013B17C[];
extern void *ptrs_8013B180[];
extern void *ptrs_8013B184[];
extern void *ptrs_8013B188[];
extern void *ptrs_8013B18C[];
extern void *ptrs_8013B19C[];
extern void *ptrs_8013B1A0[];

void *ptrs_8013B17C[1] = {
    D_8013B874,
};

void *ptrs_8013B180[1] = {
    D_8013B87C,
};

void *ptrs_8013B184[1] = {
    D_8013B88C,
};

void *ptrs_8013B188[1] = {
    D_8013B8A0,
};

void *ptrs_8013B18C[4] = {
    D_8013B8C0,
    D_8013B910,
    D_8013B918,
    D_8013B920,
};

void *ptrs_8013B19C[1] = {
    D_8013B8E8,
};

void *ptrs_8013B1A0[1] = {
    D_8013B900,
};
