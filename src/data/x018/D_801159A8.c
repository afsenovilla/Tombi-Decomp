// DATA 801159a8 338 X018
// Data of X018 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_801159A8[];
extern unsigned int data_80115A08[];
extern unsigned int data_80115A18[];
extern unsigned int data_80115A54[];
extern unsigned int data_80115A58[];
extern unsigned char data_80115AF8[];

unsigned int data_801159A8[24] = {
    0x8013B2F4, 0x8013B310, 0x8013B2F4, 0x8013B348,
    0x8013E600, 0x8013E61C, 0x8013E638, 0x8013E654,
    0x8013B2F4, 0x8013B310, 0x8013B2F4, 0x8013B348,
    0x8012DF90, 0x8012DFAC, 0x8012DFC8, 0x8012DFE4,
    0x80131D4C, 0x80131D84, 0x80131D68, 0x80131DA0,
    0x80131D4C, 0x80131D84, 0x80131D68, 0x80131DA0,
};

unsigned int data_80115A08[4] = {
    0x8012E084, 0x8012E0BC, 0x8012E0A0, 0x8012E0D8,
};

unsigned int data_80115A18[15] = {
    0xB0B080B, 0x6070608, 0x6060608, 0x7070707,
    0xB0B0B0B, 0xA0A0A0A, 0x6060708, 0x6060708,
    0x6060708, 0xB0B0B09, 0x8080808, 0x6060708,
    0x6060708, 0x6060708, 0x6060708,
};

unsigned int data_80115A54[1] = {
    0x6070B08,
};

unsigned int data_80115A58[40] = {
    0x20000503, 0x1E030000, 0x2000, 0x20004D03,
    0x7030000, 0x2100, 0x20009703, 0x98030000,
    0x2000, 0x21004D03, 0x15030000, 0x2000,
    0x20000203, 0x1F030000, 0x2100, 0x220005FF,
    0x2030000, 0x2006, 0x210005FF, 0x2030000,
    0x2204, 0x20007C03, 0x82030000, 0x2100,
    0x22050203, 0x7030000, 0x2300, 0x20001F03,
    0x5FF0000, 0x2100, 0x22001703, 0x1E030000,
    0x2300, 0x20004D03, 0x1D030000, 0x2100,
    0x22002103, 0x4FF0000, 0x2300, 0x240005FF,
};

unsigned char data_80115AF8[2] = { 0x0, 0x0 };
