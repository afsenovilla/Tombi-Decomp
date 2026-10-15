// DATA 80137f48 12 X000
// Data of X000 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern unsigned int data_80137F48[];

unsigned int data_80137F48[3] = {
    0x24020003, 0x3E00008, 0xA0820004,
};
