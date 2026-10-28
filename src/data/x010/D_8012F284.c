// DATA 8012f284 36 X010
// Data of X010 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void followParentWithOffset();
extern void stubNop8011A1D8();
extern void wobbleWhenStoodOn();
extern void ziplineCableInit();
extern unsigned int data_8012F284[];
extern void *ptrs_8012F298[];

unsigned int data_8012F284[5] = {
    0xCC0066, 0xAC0056, 0xE40072, 0x98004C,
    0x13C009E,
};

void *ptrs_8012F298[4] = {
    wobbleWhenStoodOn,
    stubNop8011A1D8,
    followParentWithOffset,
    ziplineCableInit,
};
