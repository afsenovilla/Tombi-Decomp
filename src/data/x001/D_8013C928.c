// DATA 8013c928 34 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_8013C928[];
extern unsigned int data_8013C92C[];
extern unsigned int data_8013C930[];
extern unsigned int data_8013C934[];
extern unsigned int data_8013C938[];
extern unsigned char data_8013C948[];

unsigned int data_8013C928[1] = {
    0x1C000E,
};

unsigned int data_8013C92C[1] = {
    0x140006,
};

unsigned int data_8013C930[1] = {
    0xF0001,
};

unsigned int data_8013C934[1] = {
    0x2,
};

unsigned int data_8013C938[4] = {
    0x8013DAE8, 0x200010, 0x1C000E, 0x8,
};

unsigned char data_8013C948[2] = { 0x3, 0x0 };
