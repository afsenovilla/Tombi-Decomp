// DATA 8012a004 428 X014
// Data of X014 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8012A53C[];
extern char D_8012A558[];
extern char D_8012A574[];
extern char D_8012A57C[];
extern char D_8012A584[];
extern char D_8012A60C[];
extern void *ptrs_8012A004[];
extern void *ptrs_8012A010[];
extern void *ptrs_8012A014[];
extern void *ptrs_8012A018[];
extern void *ptrs_8012A01C[];
extern void *ptrs_8012A028[];
extern AnimWord anim_8012A030[];
extern AnimWord anim_8012A0D8[];
extern AnimWord anim_8012A0E0[];
extern AnimWord anim_8012A110[];
extern AnimWord anim_8012A164[];

void *ptrs_8012A004[3] = {
    anim_8012A030,
    anim_8012A0D8,
    anim_8012A0E0,
};

void *ptrs_8012A010[1] = {
    anim_8012A110,
};

void *ptrs_8012A014[1] = {
    anim_8012A164,
};

void *ptrs_8012A018[1] = {
    D_8012A60C,
};

void *ptrs_8012A01C[3] = {
    D_8012A574,
    D_8012A57C,
    D_8012A584,
};

void *ptrs_8012A028[2] = {
    D_8012A53C,
    D_8012A558,
};

AnimWord anim_8012A030[42] = {
    ANIM(0, 0, 0, 2),
    ANIM(1, 0, 0, 2),
    ANIM(2, 0, 0, 3),
    ANIM(3, 0, 0, 3),
    ANIM(4, 0, 0, 2),
    ANIM(5, 0, 0, 3),
    ANIM(6, 0, 0, 3),
    ANIM(7, 0, 0, 3),
    ANIM(8, 0, 0, 3),
    ANIM(9, 0, 0, 3),
    ANIM(10, 0, 0, 3),
    ANIM_STOP(11, 0, 0, 4),
    ANIM(12, 0, 0, 4),
    ANIM(13, 0, 0, 3),
    ANIM(14, 0, 0, 3),
    ANIM(15, 0, 0, 3),
    ANIM(16, 0, 0, 3),
    ANIM(17, 0, 0, 3),
    ANIM(18, 0, 0, 4),
    ANIM(19, 0, 0, 5),
    ANIM_STOP(20, 0, 0, 6),
};

AnimWord anim_8012A0D8[2] = {
    ANIM_STOP(27, 0, 0, 60),
};

AnimWord anim_8012A0E0[12] = {
    ANIM(28, 0, 0, 3),
    ANIM(29, 1, 0, 6),
    ANIM(30, 2, 0, 6),
    ANIM(31, 3, 0, 6),
    ANIM(32, 4, 0, 6),
    ANIM_STOP(34, 5, 0, 6),
};

AnimWord anim_8012A110[21] = {
    ANIM(35, 0, 0, 2),
    ANIM(40, 0, 0, 1),
    ANIM(36, 0, 0, 2),
    ANIM(40, 0, 0, 1),
    ANIM(37, 0, 0, 2),
    ANIM(40, 0, 0, 1),
    ANIM(38, 0, 0, 2),
    ANIM(40, 0, 0, 1),
    ANIM(39, 0, 0, 2),
    ANIM_JUMP(40, 0, 0, 1, anim_8012A110),
};

AnimWord anim_8012A164[19] = {
    ANIM(41, 0, 0, 2),
    ANIM(42, 0, 0, 2),
    ANIM(43, 0, 0, 2),
    ANIM(42, 0, 0, 1),
    ANIM(44, 0, 0, 2),
    ANIM(45, 0, 0, 2),
    ANIM(46, 0, 0, 2),
    ANIM(47, 0, 0, 2),
    ANIM_JUMP(48, 0, 0, 3, anim_8012A164),
};
