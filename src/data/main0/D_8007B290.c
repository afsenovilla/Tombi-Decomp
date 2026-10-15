// DATA 8007b290 128 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8007AF44[];
extern char D_8007AF58[];
extern char D_8007AF6C[];
extern char D_8007AF80[];
extern char D_8007AF94[];
extern char D_8007AFA8[];
extern char D_8007AFBC[];
extern char D_8007AFD0[];
extern char D_8007AFE4[];
extern char D_8007AFF8[];
extern char D_8007B00C[];
extern char D_8007B020[];
extern char D_8007B034[];
extern char D_8007B048[];
extern char D_8007B05C[];
extern char D_8007B070[];
extern void *ptrs_8007B290[];
extern unsigned int data_8007B2D0[];
extern unsigned int data_8007B2D4[];

void *ptrs_8007B290[16] = {
    D_8007AF44,
    D_8007AF58,
    D_8007AF6C,
    D_8007AF80,
    D_8007AF94,
    D_8007AFA8,
    D_8007AFBC,
    D_8007AFD0,
    D_8007AFE4,
    D_8007AFF8,
    D_8007B00C,
    D_8007B020,
    D_8007B034,
    D_8007B048,
    D_8007B05C,
    D_8007B070,
};

unsigned int data_8007B2D0[1] = {
    0x1F4,
};

unsigned int data_8007B2D4[15] = {
    0x1E30150, 0x3E8, 0x1E00150, 0x7D0,
    0x1E10150, 0x1388, 0x1E20150, 0x2710,
    0x1E30150, 0x4E20, 0x1E00150, 0x186A0,
    0x1E10150, 0x7A120, 0x1E20150,
};
