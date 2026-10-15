// DATA 80114a48 448 X009
// Data of X009 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80114A48[];
extern unsigned int data_80114A68[];
extern unsigned int data_80114A80[];
extern unsigned int data_80114A98[];
extern unsigned int data_80114AA8[];
extern unsigned int data_80114AB8[];
extern unsigned int data_80114AC0[];
extern unsigned int data_80114AC8[];
extern unsigned int data_80114AE4[];
extern unsigned int data_80114AE8[];
extern unsigned int data_80114BDC[];

unsigned int data_80114A48[8] = {
    0x1400040, 0x780140, 0x2000F0, 0x200020,
    0x200020, 0x200020, 0x200020, 0x9A990020,
};

unsigned int data_80114A68[6] = {
    0xFE000200, 0xFE00FE00, 0xFD000100, 0xFD00FF00,
    0xFC800080, 0xFC000000,
};

unsigned int data_80114A80[6] = {
    0x300F0FC, 0xFC10, 0x2000400, 0x4001408,
    0x30014F6, 0x10000F0,
};

unsigned int data_80114A98[4] = {
    0xA0FF40, 0x60FF80, 0x40FFC0, 0xFFE0,
};

unsigned int data_80114AA8[4] = {
    0xFE80FEA0, 0xFE40FE60, 0xFE00FE20, 0xFDC0FDE0,
};

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
