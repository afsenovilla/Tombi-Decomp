// DATA 80138644 60 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80138644[];
extern unsigned int data_80138648[];
extern unsigned int data_8013864C[];
extern unsigned int data_80138650[];
extern unsigned int data_80138664[];
extern unsigned int data_8013867C[];

unsigned int data_80138644[1] = {
    0xFFB5,
};

unsigned int data_80138648[1] = {
    0x4B,
};

unsigned int data_8013864C[1] = {
    0x18FFB5,
};

unsigned int data_80138650[5] = {
    0x18004B, 0xFFC8, 0x38, 0x48FFC8,
    0x480038,
};

unsigned int data_80138664[6] = {
    0x0, 0x10000, 0x20000, 0x20001,
    0x10001, 0x1,
};

unsigned int data_8013867C[1] = {
    0x1EF01EE,
};
