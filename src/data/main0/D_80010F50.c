// DATA 80010f50 72 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_80010F50[];
extern AnimWord anim_80010F70[];
extern AnimWord anim_80010F78[];
extern AnimWord anim_80010F80[];
extern AnimWord anim_80010F88[];
extern AnimWord anim_80010F90[];

AnimWord anim_80010F50[8] = {
    ANIM(160, 0, 0, 6),
    ANIM(161, 0, 0, 6),
    ANIM(162, 0, 0, 6),
    ANIM_STOP(161, 0, 0, 6),
};

AnimWord anim_80010F70[2] = {
    ANIM_STOP(163, 0, 0, 60),
};

AnimWord anim_80010F78[2] = {
    ANIM_STOP(164, 0, 0, 60),
};

AnimWord anim_80010F80[2] = {
    ANIM_STOP(165, 0, 0, 60),
};

AnimWord anim_80010F88[2] = {
    ANIM_STOP(166, 0, 0, 60),
};

AnimWord anim_80010F90[2] = {
    ANIM_STOP(167, 0, 0, 60),
};
