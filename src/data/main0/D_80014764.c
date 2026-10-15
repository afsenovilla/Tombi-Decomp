// DATA 80014764 36 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_80014764[];
extern AnimWord anim_8001476C[];
extern AnimWord anim_80014774[];
extern unsigned int data_8001477C[];
extern AnimWord anim_80014780[];

AnimWord anim_80014764[2] = {
    ANIM_STOP(4, 0, 0, 2),
};

AnimWord anim_8001476C[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_80014774[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

unsigned int data_8001477C[1] = {
    (unsigned int)anim_80014780,
};

AnimWord anim_80014780[2] = {
    ANIM_STOP(82, 26, 0, 5),
};
