// DATA 801159a8 96 X009
// Data of X009 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8012DF90[];
extern char D_8012DFAC[];
extern char D_8012DFC8[];
extern char D_8012DFE4[];
extern unsigned int data_801159A8[];

unsigned int data_801159A8[24] = {
    0x8013B2F4, 0x8013B310, 0x8013B2F4, 0x8013B348,
    0x8013E600, 0x8013E61C, 0x8013E638, 0x8013E654,
    0x8013B2F4, 0x8013B310, 0x8013B2F4, 0x8013B348,
    (unsigned int)D_8012DF90, (unsigned int)D_8012DFAC, (unsigned int)D_8012DFC8, (unsigned int)D_8012DFE4,
    0x80131D4C, 0x80131D84, 0x80131D68, 0x80131DA0,
    0x80131D4C, 0x80131D84, 0x80131D68, 0x80131DA0,
};
