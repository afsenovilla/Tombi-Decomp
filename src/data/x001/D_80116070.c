// DATA 80116070 28 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80116070[];

unsigned int data_80116070[7] = {
    0x10001, 0x20002, 0x30003, 0x30003,
    0x20002, 0x10001, 0x0,
};
