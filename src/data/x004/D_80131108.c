// DATA 80131108 64 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80131108[];
extern unsigned int data_80131128[];

unsigned int data_80131108[8] = {
    0x18000C, 0x800000, 0x18000C, 0x200020,
    0x0, 0x0, 0x18000C, 0x800000,
};

unsigned int data_80131128[8] = {
    0x0, 0x0, 0x10000, 0x10001,
    0x20002, 0x30002, 0x30003, 0x30003,
};
