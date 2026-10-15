// DATA 80115c34 244 X011
// Data of X011 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80115C34[];
extern unsigned int data_80115C64[];
extern unsigned int data_80115C70[];
extern unsigned int data_80115C90[];
extern unsigned int data_80115CA4[];
extern unsigned int data_80115CB8[];
extern unsigned int data_80115CCC[];
extern unsigned int data_80115CE4[];
extern unsigned int data_80115CF0[];
extern unsigned int data_80115CFC[];
extern unsigned int data_80115D08[];
extern unsigned int data_80115D14[];

unsigned int data_80115C34[12] = {
    0x1003703, 0x37030000, 0x200, 0x3000103,
    0x1030000, 0x400, 0x5000103, 0x1030000,
    0x600, 0x7000103, 0xFF0000, 0x0,
};

unsigned int data_80115C64[3] = {
    0x1003403, 0xFF0000, 0x0,
};

unsigned int data_80115C70[8] = {
    0x103, 0x1030000, 0x100, 0x2000103,
    0x1030000, 0x300, 0xFF, 0x0,
};

unsigned int data_80115C90[5] = {
    0x4003703, 0x37030000, 0x500, 0xFF,
    0x0,
};

unsigned int data_80115CA4[5] = {
    0x4000703, 0x7030000, 0x500, 0xFF,
    0x0,
};

unsigned int data_80115CB8[5] = {
    0x1003, 0x10030000, 0x0, 0xFF,
    0x0,
};

unsigned int data_80115CCC[6] = {
    0x1003, 0x10030000, 0x0, 0x1003,
    0xFF0000, 0x0,
};

unsigned int data_80115CE4[3] = {
    0x1203, 0xFF0000, 0x0,
};

unsigned int data_80115CF0[3] = {
    0x50203, 0xFF0000, 0x0,
};

unsigned int data_80115CFC[3] = {
    0x1001403, 0xFF0000, 0x0,
};

unsigned int data_80115D08[3] = {
    0x1060203, 0xFF0000, 0x0,
};

unsigned int data_80115D14[5] = {
    0x1060203, 0x2030000, 0x106, 0xFF,
    0x0,
};
