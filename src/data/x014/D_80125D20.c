// DATA 80125d20 256 X014
// Data of X014 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80125E70[];
extern char D_80125ED0[];
extern char D_80125F50[];
extern char D_80125FB0[];
extern char D_80126030[];
extern char D_80126090[];
extern char D_80126120[];
extern char D_80126180[];
extern char D_80126200[];
extern char D_80126260[];
extern char D_801262F0[];
extern char D_80126350[];
extern char D_801263D0[];
extern char D_80126430[];
extern char D_801264B0[];
extern char D_80126510[];
extern char D_80128E9C[];
extern char D_801290A0[];
extern char D_801292C4[];
extern char D_801294F0[];
extern char D_8012970C[];
extern char D_801298F4[];
extern char D_80129B00[];
extern char D_80129D34[];
extern unsigned int data_80125D20[];
extern unsigned int data_80125D30[];
extern unsigned int data_80125D40[];
extern unsigned int data_80125D50[];
extern unsigned int data_80125D60[];
extern unsigned int data_80125D70[];
extern unsigned int data_80125D80[];
extern unsigned int data_80125E10[];

unsigned int data_80125D20[4] = {
    0x0, 0x0, 0x1000000, 0x10101,
};

unsigned int data_80125D30[4] = {
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_80125D40[4] = {
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_80125D50[4] = {
    0x1010100, 0x1010101, 0x1, 0x0,
};

unsigned int data_80125D60[4] = {
    0x1000000, 0x10101, 0x0, 0x0,
};

unsigned int data_80125D70[4] = {
    0x0, 0x0, 0x0, 0x0,
};

unsigned int data_80125D80[36] = {
    0x1000000, 0x10101, 0x0, 0x0,
    (unsigned int)data_80125D20, (unsigned int)data_80125D30, (unsigned int)data_80125D40, (unsigned int)data_80125D50,
    (unsigned int)data_80125D60, (unsigned int)data_80125D70, (unsigned int)data_80125D80, (unsigned int)data_80125D80,
    (unsigned int)D_80128E9C, (unsigned int)D_801290A0, (unsigned int)D_801292C4, (unsigned int)D_801294F0,
    (unsigned int)D_8012970C, (unsigned int)D_801298F4, (unsigned int)D_80129B00, (unsigned int)D_80129D34,
    (unsigned int)D_80125E70, (unsigned int)D_80125F50, (unsigned int)D_80126030, (unsigned int)D_80126120,
    (unsigned int)D_80126200, (unsigned int)D_801262F0, (unsigned int)D_801263D0, (unsigned int)D_801264B0,
    (unsigned int)D_80125ED0, (unsigned int)D_80125FB0, (unsigned int)D_80126090, (unsigned int)D_80126180,
    (unsigned int)D_80126260, (unsigned int)D_80126350, (unsigned int)D_80126430, (unsigned int)D_80126510,
};

unsigned int data_80125E10[4] = {
    0x60003, 0x10004, 0x20005, 0x0,
};
