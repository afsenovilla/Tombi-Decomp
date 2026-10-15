// DATA 8013b224 40 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013B5F8[];
extern char D_8013B600[];
extern char D_8013B6E8[];
extern char D_8013B6F0[];
extern char D_8013BA98[];
extern char D_8013BAA0[];
extern char D_8013BAA8[];
extern char D_8013BAB0[];
extern char D_8013BB2C[];
extern char D_8013BB34[];
extern void *ptrs_8013B224[];
extern void *ptrs_8013B230[];
extern void *ptrs_8013B238[];
extern void *ptrs_8013B23C[];
extern void *ptrs_8013B244[];

void *ptrs_8013B224[3] = {
    D_8013BAB0,
    D_8013BA98,
    D_8013BAA0,
};

void *ptrs_8013B230[2] = {
    D_8013BAA8,
    D_8013BB2C,
};

void *ptrs_8013B238[1] = {
    D_8013BB34,
};

void *ptrs_8013B23C[2] = {
    D_8013B6E8,
    D_8013B6F0,
};

void *ptrs_8013B244[2] = {
    D_8013B5F8,
    D_8013B600,
};
