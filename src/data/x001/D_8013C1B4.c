// DATA 8013c1b4 220 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_8013C1B4[];
extern unsigned int data_8013C1C4[];
extern unsigned int data_8013C1DC[];
extern unsigned int data_8013C1F4[];
extern unsigned int data_8013C214[];
extern unsigned int data_8013C234[];

unsigned int data_8013C1B4[4] = {
    0x0, 0x1030002, 0xA00B6F, 0x2020100,
};

unsigned int data_8013C1C4[6] = {
    0x0, 0x102, 0xA0105B, 0x3030100,
    0x0, 0x2000202,
};

unsigned int data_8013C1DC[6] = {
    0xB4A, 0x2000101, 0x7580B6F, 0x1040101,
    0x0, 0x1060402,
};

unsigned int data_8013C1F4[8] = {
    0x5A105C, 0x1010101, 0x7F8102C, 0x3040101,
    0xBB8107C, 0x1000901, 0x0, 0x1030202,
};

unsigned int data_8013C214[8] = {
    0x8800FB4, 0x1030101, 0x8340B68, 0x1020100,
    0x0, 0x60902, 0x0, 0xD02,
};

unsigned int data_8013C234[23] = {
    (unsigned int)data_8013C1B4, (unsigned int)data_8013C1C4, (unsigned int)data_8013C1DC, (unsigned int)data_8013C1F4,
    (unsigned int)data_8013C214, 0x320000A, 0xFF9C, 0xC80000,
    0xFF380000, 0x40000, 0x0, 0xFE7005DC,
    0x0, 0x0, 0x2580004, 0x258FF6A,
    0xFF6A, 0x0, 0x40000, 0x96FDA8,
    0x96FDA8, 0x0, 0x0,
};
