// DATA 8013c728 64 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_8013C728[];
extern unsigned int data_8013C730[];
extern unsigned int data_8013C734[];
extern unsigned int data_8013C738[];
extern unsigned int data_8013C73C[];
extern unsigned int data_8013C760[];

unsigned int data_8013C728[2] = {
    0x18000F00, 0x0,
};

unsigned int data_8013C730[1] = {
    0x100008,
};

unsigned int data_8013C734[1] = {
    0x100008,
};

unsigned int data_8013C738[1] = {
    0x3000A,
};

unsigned int data_8013C73C[9] = {
    0x8013E700, 0x100008, 0x100008, 0x3000A,
    0x8013E700, 0x100008, 0x100008, 0x3000A,
    0x8013E700,
};

unsigned int data_8013C760[2] = {
    0x19001306, 0x0,
};
