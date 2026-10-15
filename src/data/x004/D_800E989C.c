// DATA 800e989c 80 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_800E989C[];
extern unsigned int data_800E98B0[];
extern unsigned int data_800E98C4[];
extern unsigned int data_800E98D8[];

unsigned int data_800E989C[5] = {
    0x8000800, 0x800, 0x2000000, 0x6000400,
    0x400,
};

unsigned int data_800E98B0[5] = {
    0x0, 0x0, 0x8C0000, 0x8C0118,
    0x118,
};

unsigned int data_800E98C4[5] = {
    0x0, 0x0, 0x2000000, 0x5500400,
    0x400,
};

unsigned int data_800E98D8[5] = {
    0x0, 0x0, 0x2000000, 0x7300400,
    0x400,
};
