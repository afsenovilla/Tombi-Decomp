// DATA 8013b1cc 72 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013B364[];
extern char D_8013B39C[];
extern char D_8013B468[];
extern char D_8013B4BC[];
extern char D_8013B530[];
extern char D_8013B550[];
extern char D_8013B570[];
extern char D_8013B590[];
extern char D_8013B5B0[];
extern char D_8013B5D0[];
extern char D_8013B5F0[];
extern char D_8013B608[];
extern char D_8013B64C[];
extern char D_8013B838[];
extern char D_8013B840[];
extern char D_8013B848[];
extern char D_8013BAB8[];
extern char D_8013BAC0[];
extern void *ptrs_8013B1CC[];
extern void *ptrs_8013B1D0[];
extern void *ptrs_8013B1D4[];
extern void *ptrs_8013B1F0[];
extern void *ptrs_8013B1F4[];
extern void *ptrs_8013B1FC[];
extern void *ptrs_8013B208[];
extern void *ptrs_8013B20C[];

void *ptrs_8013B1CC[1] = {
    D_8013B468,
};

void *ptrs_8013B1D0[1] = {
    D_8013B4BC,
};

void *ptrs_8013B1D4[7] = {
    D_8013B530,
    D_8013B550,
    D_8013B570,
    D_8013B590,
    D_8013B5B0,
    D_8013B5D0,
    D_8013B5F0,
};

void *ptrs_8013B1F0[1] = {
    D_8013B364,
};

void *ptrs_8013B1F4[2] = {
    D_8013B608,
    D_8013B64C,
};

void *ptrs_8013B1FC[3] = {
    D_8013B838,
    D_8013B840,
    D_8013B848,
};

void *ptrs_8013B208[1] = {
    D_8013B39C,
};

void *ptrs_8013B20C[2] = {
    D_8013BAB8,
    D_8013BAC0,
};
