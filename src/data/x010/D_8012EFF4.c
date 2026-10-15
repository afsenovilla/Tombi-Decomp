// DATA 8012eff4 348 X010
// Data of X010 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_8012EFF4[];
extern unsigned int data_8012F004[];
extern unsigned int data_8012F01C[];
extern unsigned int data_8012F034[];
extern unsigned int data_8012F054[];
extern void *ptrs_8012F064[];
extern unsigned int data_8012F088[];

unsigned int data_8012EFF4[4] = {
    0x0, 0x4011202, 0xD02, 0x1010A00,
};

unsigned int data_8012F004[6] = {
    0x1FE0000, 0x3000A01, 0x366003C, 0x1020A00,
    0x0, 0x4030A02,
};

unsigned int data_8012F01C[6] = {
    0x0, 0x2010A02, 0x3C, 0x1000A00,
    0x366003C, 0x1020A00,
};

unsigned int data_8012F034[8] = {
    0x0, 0x1050002, 0x0, 0x1080A02,
    0x0, 0x3010A02, 0x0, 0x4010A02,
};

unsigned int data_8012F054[4] = {
    0x0, 0x2030A02, 0x0, 0x3030A02,
};

void *ptrs_8012F064[9] = {
    data_8012EFF4,
    data_8012F004,
    data_8012F01C,
    data_8012F034,
    data_8012EFF4,
    data_8012F004,
    data_8012F01C,
    data_8012F034,
    data_8012F054,
};

unsigned int data_8012F088[50] = {
    0xCEB0000, 0x152F114E, 0xAA1D90, 0x5440026,
    0x6D6475A5, 0x6E0A6A6E, 0x54E36544, 0x41693C00,
    0xCEB0000, 0x152F114E, 0xAA1D90, 0x5440026,
    0x6A6E6D64, 0x75A56E0A, 0x54E375A5, 0x41693C00,
    0xCEB0000, 0x152F114E, 0xAA1D90, 0x5440026,
    0x75A56E0A, 0x6A6E6D64, 0x54E375A5, 0x41693C00,
    0xCEB0000, 0x152F114E, 0xAA1D90, 0x5440026,
    0x6E0A6A6E, 0x6D6475A5, 0x54E36544, 0x41693C00,
    0x320000A, 0xFF9C, 0xC80000, 0xFF380000,
    0x40000, 0x0, 0xFE7005DC, 0x0,
    0x0, 0x2580004, 0x258FF6A, 0xFF6A,
    0x0, 0x40000, 0x96FDA8, 0x96FDA8,
    0x0, 0x0,
};
