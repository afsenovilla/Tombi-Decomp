// DATA 80115d3c 84 X008
// Data of X008 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80115C34[];
extern char D_80115C64[];
extern char D_80115C70[];
extern char D_80115C90[];
extern char D_80115CA4[];
extern char D_80115CB8[];
extern char D_80115CCC[];
extern char D_80115CE4[];
extern char D_80115CF0[];
extern char D_80115CFC[];
extern char D_80115D08[];
extern char D_80115D14[];
extern char D_80115D28[];
extern void *ptrs_80115D3C[];
extern void *ptrs_80115D40[];
extern void *ptrs_80115D58[];
extern void *ptrs_80115D5C[];
extern void *ptrs_80115D60[];
extern void *ptrs_80115D64[];
extern unsigned int data_80115D70[];

void *ptrs_80115D3C[1] = {
    D_80115C34,
};

void *ptrs_80115D40[6] = {
    D_80115C64,
    D_80115C70,
    D_80115C90,
    D_80115CA4,
    D_80115CB8,
    D_80115CCC,
};

void *ptrs_80115D58[1] = {
    D_80115CE4,
};

void *ptrs_80115D5C[1] = {
    D_80115CF0,
};

void *ptrs_80115D60[1] = {
    D_80115CFC,
};

void *ptrs_80115D64[3] = {
    D_80115D08,
    D_80115D14,
    D_80115D28,
};

unsigned int data_80115D70[8] = {
    0xFC00FFC0, 0xFC000040, 0xFBC0FFA0, 0xFBC00060,
    0xFB80FF80, 0xFB800080, 0xFB40FF60, 0xFB4000A0,
};
