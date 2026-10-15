// DATA 80115a18 226 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80115A18[];
extern unsigned int data_80115A54[];
extern unsigned int data_80115A58[];
extern unsigned char data_80115AF8[];

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
