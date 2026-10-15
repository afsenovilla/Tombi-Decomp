// DATA 80115ff4 68 X002
// Data of X002 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80115FF4[];
extern unsigned int data_8011600C[];

unsigned int data_80115FF4[6] = {
    0xFFFA0000, 0xFFFA, 0x60000, 0x6,
    0xFFFCFFFC, 0x4FFFC,
};

unsigned int data_8011600C[11] = {
    0x40004, 0xFFFC0004, 0xFFFD0000, 0xFFFD,
    0x30000, 0x3, 0xFFFD0000, 0xFFFD,
    0x30000, 0x3, 0x0,
};
