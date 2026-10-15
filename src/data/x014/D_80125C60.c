// DATA 80125c60 32 X014
// Data of X014 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80125C60[];
extern unsigned int data_80125C68[];
extern unsigned int data_80125C70[];

unsigned int data_80125C60[2] = {
    0x28272625, 0x2B2B2A29,
};

unsigned int data_80125C68[2] = {
    0x2060401, 0x7030500,
};

unsigned int data_80125C70[4] = {
    0x1E501E3, 0x1E201E4, 0x1E101E0, 0x1E701E6,
};
