// DATA 80077cdc 120 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80077CDC[];
extern unsigned int data_80077D0C[];

unsigned int data_80077CDC[12] = {
    0x3000080, 0x300FF80, 0x0, 0x30000C0,
    0x300FF40, 0x0, 0x3000100, 0x300FF00,
    0x0, 0x3000130, 0x300FED0, 0x0,
};

unsigned int data_80077D0C[18] = {
    0x4000180, 0x400FE80, 0x0, 0x40001C0,
    0x400FE40, 0x0, 0x5000200, 0x500FE00,
    0x0, 0x5000280, 0x500FD80, 0x0,
    0x6000300, 0x600FD00, 0x0, 0x6000380,
    0x600FC80, 0x0,
};
