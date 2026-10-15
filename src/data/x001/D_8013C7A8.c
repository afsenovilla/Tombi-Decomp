// DATA 8013c7a8 340 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_8013C7A8[];
extern unsigned int data_8013C7B0[];
extern unsigned int data_8013C7B8[];
extern unsigned int data_8013C7C8[];
extern unsigned int data_8013C7D8[];
extern unsigned int data_8013C7E0[];
extern unsigned int data_8013C7F0[];
extern unsigned int data_8013C800[];
extern unsigned int data_8013C8E8[];

unsigned int data_8013C7A8[2] = {
    0x0, 0x0,
};

unsigned int data_8013C7B0[2] = {
    0x80008000, 0x40406020,
};

unsigned int data_8013C7B8[4] = {
    0x1000001, 0x100, 0x100, 0x1000100,
};

unsigned int data_8013C7C8[4] = {
    0x1010001, 0x100, 0x100, 0x1000101,
};

unsigned int data_8013C7D8[2] = {
    0x1000100, 0x10100,
};

unsigned int data_8013C7E0[4] = {
    0x3010001, 0x101, 0x10301, 0x1010301,
};

unsigned int data_8013C7F0[4] = {
    0x1010001, 0x1030100, 0x10103, 0x3010303,
};

unsigned int data_8013C800[58] = {
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
    0x26161B08, 0x1B0B1E09, 0xFFF0FFE8, 0xFFFAFFF8,
    0xFFFAFFFA, 0xFFFCFFFC,
};

unsigned int data_8013C8E8[5] = {
    0x4FFFC, 0x60006, 0x80008, 0x180010,
    0x20,
};
