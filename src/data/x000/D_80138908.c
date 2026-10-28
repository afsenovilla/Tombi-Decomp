// DATA 80138908 676 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void updateChainedPartsObj();
extern void chainedPartPlaceChild();
extern void func_8011E028();
extern void func_8011E0EC();
extern unsigned int data_80138908[];
extern unsigned int data_80138948[];
extern unsigned int data_80138988[];
extern unsigned int data_8013898C[];
extern unsigned int data_801389B8[];
extern unsigned int data_801389BC[];
extern unsigned int data_801389DC[];
extern unsigned int data_80138A10[];
extern unsigned int data_80138A30[];
extern unsigned int data_80138A64[];
extern unsigned int data_80138A84[];
extern unsigned int data_80138AB0[];
extern unsigned int data_80138AE0[];
extern unsigned int data_80138B20[];
extern void *ptrs_80138B70[];
extern void *ptrs_80138B7C[];
extern void *ptrs_80138B88[];
extern unsigned int data_80138B98[];

unsigned int data_80138908[16] = {
    0xFFF80002, 0x10001, 0xFFF80002, 0x10001,
    0x20006, 0x1FFF8, 0x20006, 0x1FFF8,
    0x20006, 0x1FFF8, 0x20006, 0x1FFF8,
    0x20006, 0x1FFF8, 0xFFF80002, 0x10001,
};

unsigned int data_80138948[16] = {
    0x20002, 0x10001, 0x20002, 0x10001,
    0x2000A, 0x10002, 0x2000A, 0x10002,
    0x2000A, 0x10002, 0x2000A, 0x10002,
    0x2000A, 0x10002, 0x20002, 0x10001,
};

unsigned int data_80138988[1] = {
    0x10641000,
};

unsigned int data_8013898C[11] = {
    0x10641000, 0x10641000, 0xFB00F9C, 0x10001050,
    0x10001050, 0xF9C0FB0, 0x10640F4C, 0x10501000,
    0x10640F9C, 0x10501000, 0x108C1050,
};

unsigned int data_801389B8[1] = {
    0x80000000,
};

unsigned int data_801389BC[8] = {
    0x500000, 0xF000A0, 0xF00140, 0x5000A0,
    0xFFD80000, 0xFF88FFB0, 0xFF88FF60, 0xFFD8FFB0,
};

unsigned int data_801389DC[13] = {
    0x140000, 0x3C0028, 0x3C0050, 0x140028,
    0xFFF60000, 0xFFE2FFEC, 0xFFE2FFD8, 0xFFF6FFEC,
    0x50000, 0xF000A, 0xF0014, 0x5000A,
    0x80000000,
};

unsigned int data_80138A10[8] = {
    0xFEE8FEC0, 0xFF38FF10, 0xFF88FF60, 0xFFB0,
    0x280014, 0x50003C, 0x780064, 0xA0008C,
};

unsigned int data_80138A30[13] = {
    0x78008C, 0x500064, 0x28003C, 0x14,
    0xFFD8FFEC, 0xFFB0FFC4, 0xFFB0FF9C, 0xFFD8FFC4,
    0xFFEC, 0x280014, 0x28003C, 0x14,
    0x8000,
};

unsigned int data_80138A64[8] = {
    0xFF60FF38, 0xFFB0FF88, 0x140000, 0x3C0028,
    0x640050, 0x8C0078, 0x8C00A0, 0x640078,
};

unsigned int data_80138A84[11] = {
    0x3C0050, 0x140028, 0xFFEC0000, 0xFFC4FFD8,
    0xFF9CFFB0, 0xFFC4FFB0, 0xFFECFFD8, 0x140000,
    0x3C0028, 0x140028, 0x80000000,
};

unsigned int data_80138AB0[12] = {
    0x160016, 0xA0005, 0xA0016, 0x0,
    0x120012, 0xA0005, 0xA0012, 0x2,
    0xE000E, 0xA0005, 0xA000E, 0x2,
};

unsigned int data_80138AE0[16] = {
    0x220022, 0xA0005, 0xA0022, 0x0,
    0x200020, 0xA0005, 0xA0020, 0x20000,
    0x210021, 0xA0005, 0xA0021, 0x40000,
    0x1E001E, 0xA0005, 0xA001E, 0x50002,
};

unsigned int data_80138B20[20] = {
    0x1C001C, 0xA0005, 0xA001C, 0x0,
    0x1A001A, 0xA0005, 0xA001A, 0x30000,
    0x1B001B, 0xA0005, 0xA001B, 0xFFFF0000,
    0x1A001A, 0xA0005, 0xA001A, 0x20012,
    0x1A001A, 0xA0005, 0xA001A, 0x20012,
};

void *ptrs_80138B70[3] = {
    data_80138AB0,
    data_80138AE0,
    data_80138B20,
};

void *ptrs_80138B7C[3] = {
    func_8011E028,
    chainedPartPlaceChild,
    updateChainedPartsObj,
};

void *ptrs_80138B88[4] = {
    func_8011E0EC,
    chainedPartPlaceChild,
    chainedPartPlaceChild,
    updateChainedPartsObj,
};

unsigned int data_80138B98[5] = {
    0x8011DF60, (unsigned int)chainedPartPlaceChild, (unsigned int)chainedPartPlaceChild, (unsigned int)chainedPartPlaceChild,
    (unsigned int)updateChainedPartsObj,
};
