// DATA 80138fa8 336 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80138FA8[];
extern unsigned int data_80138FB8[];
extern unsigned int data_80138FC8[];
extern unsigned int data_80138FD8[];
extern unsigned int data_801390B0[];
extern unsigned int data_801390C0[];
extern unsigned int data_801390D0[];
extern void *ptrs_801390E0[];
extern unsigned int data_801390EC[];
extern unsigned int data_801390F4[];

unsigned int data_80138FA8[4] = {
    0x10000, 0x100, 0x100, 0x1,
};

unsigned int data_80138FB8[4] = {
    0x10001, 0x10001, 0x1000100, 0x10001,
};

unsigned int data_80138FC8[4] = {
    0x1020100, 0x10002, 0x20100, 0x2000102,
};

unsigned int data_80138FD8[54] = {
    0x25151707, 0x2617140B, 0x26171908, 0x25151905,
    0x27171A0B, 0x2717180E, 0x1606220C, 0x2111220C,
    0x2515150B, 0x24141609, 0x2515150A, 0x23151907,
    0x25141507, 0x2515130A, 0x25151608, 0x23131B08,
    0x2515180B, 0x2111180B, 0x17072817, 0x26161608,
    0x26162212, 0x1D122212, 0x1A0C2010, 0x1303250F,
    0x1606250F, 0x1505280F, 0x1D0E240E, 0x2415190C,
    0x26151B06, 0x24151909, 0x17061F0C, 0x20102009,
    0x24141809, 0x2414140B, 0x24141809, 0x1505240C,
    0x1505220E, 0x2515170A, 0x1504200D, 0x12042315,
    0x2313150B, 0x2313130B, 0x25151309, 0x1506200B,
    0x1F101D09, 0x2213160A, 0x2515170C, 0x1B0B1E08,
    0x1B0B1B09, 0x1B0B1C0D, 0x22121C08, 0x26161608,
    0x26161B08, 0x1B0B1E09,
};

unsigned int data_801390B0[4] = {
    0xFF05FF05, 0xFFFF05FF, 0xFFFF05FF, 0xFFFFFF05,
};

unsigned int data_801390C0[4] = {
    0x2010105, 0x1010201, 0x2010201, 0x2010101,
};

unsigned int data_801390D0[4] = {
    0x2010202, 0x2010201, 0x1020101, 0x2010202,
};

void *ptrs_801390E0[3] = {
    data_801390D0,
    data_801390C0,
    data_801390B0,
};

unsigned int data_801390EC[2] = {
    0x2E35342E, 0x33182E,
};

unsigned int data_801390F4[1] = {
    0xC31,
};
