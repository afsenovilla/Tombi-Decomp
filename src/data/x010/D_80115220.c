// DATA 80115220 200 X010
// Data of X010 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80115220[];
extern unsigned int data_80115228[];
extern unsigned int data_80115248[];
extern unsigned int data_80115268[];

unsigned int data_80115220[2] = {
    0x8100000, 0x888,
};

unsigned int data_80115228[8] = {
    0x8380000, 0x8A00888, 0x2920290, 0x2960294,
    0x29C0298, 0x41C0000, 0x4480424, 0x448,
};

unsigned int data_80115248[8] = {
    0xFC000480, 0xFC000480, 0xFC000480, 0xFC000480,
    0xFD000200, 0xFD000200, 0xFD000200, 0xFD000200,
};

unsigned int data_80115268[32] = {
    0xC, 0x18, 0x28, 0x30,
    0x40, 0x100000, 0x300020, 0x100,
    0x0, 0x0, 0x10001, 0x10001,
    0x20002, 0x30002, 0x30003, 0x40003,
    0x40004, 0x50004, 0x50005, 0x60005,
    0x60006, 0x70006, 0x70007, 0x80008,
    0x80008, 0x80008, 0x90009, 0x9,
    0x24282C30, 0x14181C20, 0x6080C10, 0x204,
};
