// DATA 80114a48 80 X006
// Data of X006 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80114A48[];
extern unsigned int data_80114A68[];
extern unsigned int data_80114A80[];

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
