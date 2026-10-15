// DATA 801255e4 168 X014
// Data of X014 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_801255E4[];
extern unsigned int data_801255F4[];
extern unsigned int data_80125604[];
extern unsigned int data_80125614[];
extern unsigned int data_80125624[];
extern unsigned int data_80125634[];
extern unsigned int data_80125644[];
extern unsigned int data_80125654[];
extern void *ptrs_80125664[];
extern unsigned int data_80125684[];

unsigned int data_801255E4[4] = {
    0x0, 0x3, 0x0, 0x5040103,
};

unsigned int data_801255F4[4] = {
    0x0, 0x3, 0x0, 0x4000A03,
};

unsigned int data_80125604[4] = {
    0x0, 0x3, 0x0, 0x2051003,
};

unsigned int data_80125614[4] = {
    0x0, 0x3, 0x0, 0x10A0403,
};

unsigned int data_80125624[4] = {
    0x0, 0x3, 0x0, 0x1040203,
};

unsigned int data_80125634[4] = {
    0x0, 0x3, 0x0, 0x1020903,
};

unsigned int data_80125644[4] = {
    0x0, 0x3, 0x0, 0x4020303,
};

unsigned int data_80125654[4] = {
    0x0, 0x10D03, 0x0, 0x50203,
};

void *ptrs_80125664[8] = {
    data_801255E4,
    data_801255F4,
    data_80125604,
    data_80125614,
    data_80125624,
    data_80125634,
    data_80125644,
    data_80125654,
};

unsigned int data_80125684[2] = {
    0xA12100E, 0x130F110D,
};
