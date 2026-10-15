// DATA 80134f64 124 X003
// Data of X003 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80134F64[];
extern unsigned int data_80134F7C[];
extern unsigned int data_80134F9C[];
extern unsigned int data_80134FB4[];
extern void *ptrs_80134FC4[];

unsigned int data_80134F64[6] = {
    0x0, 0x1000502, 0xFF7400A0, 0x3000900,
    0x0, 0x10302,
};

unsigned int data_80134F7C[8] = {
    0x0, 0x3000302, 0x0, 0x1020302,
    0x0, 0x4000302, 0x0, 0x503,
};

unsigned int data_80134F9C[6] = {
    0x0, 0x1010302, 0x0, 0x1030302,
    0x0, 0x2021302,
};

unsigned int data_80134FB4[4] = {
    0x0, 0x2020302, 0x0, 0x10302,
};

void *ptrs_80134FC4[7] = {
    data_80134F64,
    data_80134F7C,
    data_80134F9C,
    data_80134FB4,
    data_80134F64,
    data_80134F7C,
    data_80134FB4,
};
