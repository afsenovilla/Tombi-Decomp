// DATA 80114ab8 336 X018
// Data of X018 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80114AB8[];
extern unsigned int data_80114AC0[];
extern unsigned int data_80114AC8[];
extern unsigned int data_80114AE4[];
extern unsigned int data_80114AE8[];
extern unsigned int data_80114BDC[];

unsigned int data_80114AB8[2] = {
    0x2A101041, 0x2A101041,
};

unsigned int data_80114AC0[2] = {
    0x2D111144, 0x2D111144,
};

unsigned int data_80114AC8[7] = {
    0x8013A160, 0x8013DAE8, 0x8013DAE8, 0x801386F8,
    0x80133B5C, 0x801386F8, 0x10081008,
};

unsigned int data_80114AE4[1] = {
    0x30008,
};

unsigned int data_80114AE8[61] = {
    0x8013B120, 0x10081008, 0x30008, 0x8013E558,
    0x10081008, 0x30008, 0x8013E558, 0x10081008,
    0x30008, 0x8013E558, 0x10081008, 0x30008,
    0x8013E558, 0x10081008, 0x30008, 0x8013E558,
    0x10081008, 0x30008, 0x8013E558, 0x10081008,
    0x30008, 0x8013E558, 0x10081008, 0x30008,
    0x8013E558, 0x10081008, 0x30008, 0x8013E558,
    0x10081008, 0x30008, 0x8013E558, 0x10081008,
    0x30008, 0x8013E558, 0x10081008, 0x30008,
    0x8013E558, 0x10081008, 0x30008, 0x8013E558,
    0x10081008, 0x30008, 0x8013E558, 0x10081008,
    0x30008, 0x8013E558, 0x10081008, 0x30008,
    0x8013E558, 0x10081008, 0x30008, 0x8013E558,
    0x10081008, 0x30008, 0x8013E558, 0x10081008,
    0x30008, 0x8013E558, 0x10081008, 0x30008,
    0x8013E558,
};

unsigned int data_80114BDC[11] = {
    0x20001, 0xFE00, 0x30000, 0xFD00,
    0x40001, 0xFDB0, 0x50000, 0xFF00,
    0x60001, 0xFF00, 0x40206,
};
