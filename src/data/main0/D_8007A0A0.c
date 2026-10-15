// DATA 8007a0a0 112 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void func_80033040();
extern unsigned int data_8007A0A0[];
extern unsigned int data_8007A108[];
extern unsigned int data_8007A10C[];

unsigned int data_8007A0A0[26] = {
    0x10101, 0x1000000, 0x10001, 0x0,
    0x1000100, 0x0, 0x0, 0x10000,
    0x100, 0x1000000, 0x1, 0x0,
    0x0, 0x0, 0x1010001, 0x1010101,
    0x10101, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0,
};

unsigned int data_8007A108[1] = {
    0x1,
};

unsigned int data_8007A10C[1] = {
    (unsigned int)func_80033040,
};
