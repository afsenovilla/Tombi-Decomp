// DATA 80115de4 196 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80115DE4[];
extern unsigned int data_80115DE8[];
extern unsigned int data_80115DEC[];
extern unsigned int data_80115DF0[];

unsigned int data_80115DE4[1] = {
    0x14000A,
};

unsigned int data_80115DE8[1] = {
    0x200010,
};

unsigned int data_80115DEC[1] = {
    0x30008,
};

unsigned int data_80115DF0[46] = {
    0x8013AC14, 0x200010, 0x200010, 0x20001,
    0x8013DE08, 0x200010, 0x200010, 0x70006,
    0x8011F1A8, 0x200010, 0x200010, 0x70001,
    0x8013DD50, 0x200010, 0x200010, 0x70002,
    0x8013DD50, 0x14000A, 0x200010, 0x20001,
    0x8013AC48, 0x14000A, 0x100008, 0x3000A,
    0x8013E568, 0x300018, 0x100018, 0x30012,
    0x8013E740, 0x200010, 0x200010, 0x30012,
    0x8011F190, 0x0, 0x0, 0x30004,
    0x8011F180, 0x20001, 0x20001, 0xC000F,
    0x8011F148, 0x200010, 0x200010, 0x80002,
    0x8013E794, 0x1E,
};
