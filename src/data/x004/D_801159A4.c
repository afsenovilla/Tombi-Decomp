// DATA 801159a4 60 X004
// Data of X004 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void func_8011A0B4();
extern void func_8012DFAC();
extern unsigned int data_801159A4[];

unsigned int data_801159A4[15] = {
    (unsigned int)func_8011A0B4, 0x8013B2F4, 0x8013B310, 0x8013B2F4,
    0x8013B348, 0x8013E600, 0x8013E61C, 0x8013E638,
    0x8013E654, 0x8013B2F4, 0x8013B310, 0x8013B2F4,
    0x8013B348, 0x8012DF90, (unsigned int)func_8012DFAC,
};
