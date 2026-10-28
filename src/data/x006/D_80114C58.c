// DATA 80114c58 36 X006
// Data of X006 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void villagerNpcWalkPath();
extern void villagerNpcSpringMotion();
extern void stubNop800EDC0C();
extern void *ptrs_80114C58[];
extern unsigned int data_80114C64[];
extern unsigned int data_80114C70[];

void *ptrs_80114C58[3] = {
    stubNop800EDC0C,
    villagerNpcWalkPath,
    villagerNpcSpringMotion,
};

unsigned int data_80114C64[3] = {
    0x1F800310, 0x1F800314, 0x1F800318,
};

unsigned int data_80114C70[3] = {
    0x1F80031C, 0x1F800320, 0x1F800324,
};
