// DATA 8011980c 72 X017
// Data of X017 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_8011980C[];
extern unsigned int data_80119838[];

unsigned int data_8011980C[11] = {
    0x320000A, 0xFF9C, 0xC80000, 0xFF380000,
    0x40000, 0x0, 0xFE7005DC, 0x0,
    0x0, 0x2580004, 0x258FF6A,
};

unsigned int data_80119838[7] = {
    0xFF6A, 0x0, 0x40000, 0x96FDA8,
    0x96FDA8, 0x0, 0x0,
};
