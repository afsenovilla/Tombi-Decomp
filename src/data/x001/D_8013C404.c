// DATA 8013c404 92 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void springVineStep2();
extern void springVineStep3();
extern void springVineStep4();
extern void springVineGrabbed();
extern void springVineIdle();
extern unsigned int data_8013C404[];
extern unsigned int data_8013C428[];
extern void *ptrs_8013C44C[];

unsigned int data_8013C404[9] = {
    0xD000D0, 0x14000D0, 0x1400140, 0xD000D0,
    0xD000D0, 0xD000D0, 0xD000D0, 0xD000D0,
    0x1400140,
};

unsigned int data_8013C428[9] = {
    0x1F901F6, 0x1E601F9, 0x1EB01E9, 0x1E601E6,
    0x1EA01E6, 0x1E601E6, 0x1EC01E6, 0x1EC01EC,
    0x1ED01ED,
};

void *ptrs_8013C44C[5] = {
    springVineIdle,
    springVineGrabbed,
    springVineStep2,
    springVineStep3,
    springVineStep4,
};
