// DATA 801265b0 360 X014
// Data of X014 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_801265B0[];
extern unsigned int data_801265BC[];
extern unsigned int data_801265C4[];
extern unsigned int data_801265D4[];
extern unsigned int data_80126654[];
extern unsigned int data_801266D4[];
extern unsigned int data_801266E0[];
extern unsigned int data_801266F0[];
extern unsigned int data_801266F8[];
extern unsigned int data_80126708[];

unsigned int data_801265B0[3] = {
    0xFFE00020, 0xFF000080, 0xFF600040,
};

unsigned int data_801265BC[2] = {
    0x30501010, 0x40201040,
};

unsigned int data_801265C4[4] = {
    0x400000, 0xF000A0, 0x700018, 0xD0,
};

unsigned int data_801265D4[32] = {
    0xFE8C00C0, 0x0, 0xFEFC00E2, 0x2,
    0xFEBC00F2, 0x0, 0xFF1C013E, 0x2,
    0xFE8C0154, 0x1, 0xFECC0172, 0x0,
    0xFF2C0182, 0x1, 0xFE9401B4, 0x2,
    0xFF1C01C4, 0x0, 0xFEB40202, 0x1,
    0xFEC80267, 0x3, 0xFF5D0244, 0x3,
    0xFFA901C2, 0x3, 0xFFC10136, 0x3,
    0xFF7300C3, 0x3, 0xFED3006B, 0x3,
};

unsigned int data_80126654[32] = {
    0xFE0C0190, 0x0, 0xFE0C022D, 0x0,
    0xFE6401E1, 0x0, 0xFE640280, 0x0,
    0xFE640140, 0x0, 0xFEBC0190, 0x0,
    0xFEBC0230, 0x0, 0xFF1F00D2, 0x0,
    0xFF5C014E, 0x0, 0xFF5B01DC, 0x0,
    0xFF570280, 0x0, 0xFF4402E0, 0x0,
    0xFF4D00FC, 0x0, 0xFF4D019C, 0x0,
    0xFF410218, 0x0, 0xFF5602BC, 0x0,
};

unsigned int data_801266D4[3] = {
    0x200028, 0x280040, 0x400020,
};

unsigned int data_801266E0[4] = {
    0x780050, 0xC800A0, 0x7800F0, 0xC800A0,
};

unsigned int data_801266F0[2] = {
    0x10100808, 0x6060606,
};

unsigned int data_801266F8[4] = {
    0x1000100, 0x1010101, 0x10101, 0x1000101,
};

unsigned int data_80126708[4] = {
    0x100, 0x1, 0x1, 0x100,
};
