// DATA 801159a8 48 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013B2F4[];
extern char D_8013B310[];
extern char D_8013B348[];
extern unsigned int data_801159A8[];

unsigned int data_801159A8[12] = {
    (unsigned int)D_8013B2F4, (unsigned int)D_8013B310, (unsigned int)D_8013B2F4, (unsigned int)D_8013B348,
    0x8013E600, 0x8013E61C, 0x8013E638, 0x8013E654,
    (unsigned int)D_8013B2F4, (unsigned int)D_8013B310, (unsigned int)D_8013B2F4, (unsigned int)D_8013B348,
};
