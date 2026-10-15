// DATA 80135d7c 18 X003
// Data of X003 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80135D7C[];
extern unsigned int data_80135D80[];
extern unsigned int data_80135D84[];
extern unsigned char data_80135D8C[];

unsigned int data_80135D7C[1] = {
    0x2010180C,
};

unsigned int data_80135D80[1] = {
    0xB000A,
};

unsigned int data_80135D84[2] = {
    0x8013A368, 0x2010180C,
};

unsigned char data_80135D8C[2] = { 0x1, 0x0 };
