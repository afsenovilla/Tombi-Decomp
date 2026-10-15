// DATA 801229cc 556 X006
// Data of X006 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void *ptrs_801229CC[];
extern void *ptrs_801229D0[];
extern void *ptrs_801229D4[];
extern void *ptrs_801229D8[];
extern void *ptrs_801229E0[];
extern AnimWord anim_80122A00[];
extern AnimWord anim_80122A08[];
extern AnimWord anim_80122A1C[];
extern AnimWord anim_80122A30[];
extern AnimWord anim_80122A44[];
extern AnimWord anim_80122A4C[];
extern AnimWord anim_80122A98[];
extern AnimWord anim_80122AA0[];
extern AnimWord anim_80122AA8[];
extern AnimWord anim_80122AB0[];
extern AnimWord anim_80122AB8[];
extern AnimWord anim_80122ADC[];
extern AnimWord anim_80122B00[];
extern AnimWord anim_80122B24[];
extern AnimWord anim_80122B48[];
extern AnimWord anim_80122B6C[];
extern AnimWord anim_80122B7C[];
extern unsigned int data_80122B8C[];
extern AnimWord anim_80122B90[];
extern AnimWord anim_80122B98[];
extern AnimWord anim_80122BA0[];
extern AnimWord anim_80122BA8[];
extern AnimWord anim_80122BCC[];
extern AnimWord anim_80122BD4[];

void *ptrs_801229CC[1] = {
    anim_80122B90,
};

void *ptrs_801229D0[1] = {
    anim_80122B98,
};

void *ptrs_801229D4[1] = {
    anim_80122BA0,
};

void *ptrs_801229D8[2] = {
    anim_80122BA8,
    anim_80122BCC,
};

void *ptrs_801229E0[8] = {
    anim_80122BD4,
    anim_80122AB8,
    anim_80122ADC,
    anim_80122B00,
    anim_80122B24,
    anim_80122B48,
    anim_80122B6C,
    anim_80122B7C,
};

AnimWord anim_80122A00[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80122A08[5] = {
    ANIM(25, 0, 0, 4),
    ANIM_JUMP(26, 0, 0, 4, anim_80122A08),
};

AnimWord anim_80122A1C[5] = {
    ANIM(26, 0, 0, 4),
    ANIM_JUMP(27, 0, 0, 4, anim_80122A1C),
};

AnimWord anim_80122A30[5] = {
    ANIM(27, 0, 0, 4),
    ANIM_JUMP(28, 0, 0, 4, anim_80122A30),
};

AnimWord anim_80122A44[2] = {
    ANIM_STOP(28, 0, 0, 60),
};

AnimWord anim_80122A4C[19] = {
    ANIM(31, 0, 0, 4),
    ANIM(30, 0, 0, 4),
    ANIM(29, 0, 0, 4),
    ANIM(30, 0, 0, 4),
    ANIM(31, 0, 0, 4),
    ANIM(32, 0, 0, 4),
    ANIM(33, 0, 0, 4),
    ANIM(32, 0, 0, 4),
    ANIM_JUMP(31, 0, 0, 4, anim_80122A08),
};

AnimWord anim_80122A98[2] = {
    ANIM_STOP(29, 0, 0, 60),
};

AnimWord anim_80122AA0[2] = {
    ANIM_STOP(30, 0, 0, 60),
};

AnimWord anim_80122AA8[2] = {
    ANIM_STOP(32, 0, 0, 60),
};

AnimWord anim_80122AB0[2] = {
    ANIM_STOP(33, 0, 0, 60),
};

AnimWord anim_80122AB8[9] = {
    ANIM(1, 0, 0, 6),
    ANIM(2, 0, 0, 6),
    ANIM(3, 0, 0, 6),
    ANIM_JUMP(4, 0, 0, 6, anim_80122AB8),
};

AnimWord anim_80122ADC[9] = {
    ANIM(5, 0, 0, 6),
    ANIM(6, 0, 0, 6),
    ANIM(7, 0, 0, 6),
    ANIM_JUMP(8, 0, 0, 6, anim_80122ADC),
};

AnimWord anim_80122B00[9] = {
    ANIM(9, 0, 0, 6),
    ANIM(10, 0, 0, 6),
    ANIM(11, 0, 0, 6),
    ANIM_JUMP(12, 0, 0, 6, anim_80122B00),
};

AnimWord anim_80122B24[9] = {
    ANIM(13, 0, 0, 6),
    ANIM(14, 0, 0, 6),
    ANIM(15, 0, 0, 6),
    ANIM_JUMP(16, 0, 0, 6, anim_80122B24),
};

AnimWord anim_80122B48[9] = {
    ANIM(17, 0, 0, 6),
    ANIM(18, 0, 0, 6),
    ANIM(19, 0, 0, 6),
    ANIM_JUMP(20, 0, 0, 6, anim_80122B48),
};

AnimWord anim_80122B6C[4] = {
    ANIM(21, 0, 0, 5),
    ANIM_STOP(22, 0, 0, 60),
};

AnimWord anim_80122B7C[4] = {
    ANIM(23, 0, 0, 5),
    ANIM_STOP(24, 0, 0, 60),
};

unsigned int data_80122B8C[1] = {
    (unsigned int)anim_80122B7C,
};

AnimWord anim_80122B90[2] = {
    ANIM_STOP(34, 0, 0, 60),
};

AnimWord anim_80122B98[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_80122BA0[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_80122BA8[9] = {
    ANIM(37, 0, 0, 3),
    ANIM(38, 0, 0, 3),
    ANIM(37, 0, 0, 3),
    ANIM_JUMP(39, 0, 0, 3, anim_80122BA8),
};

AnimWord anim_80122BCC[2] = {
    ANIM_STOP(40, 0, 0, 60),
};

AnimWord anim_80122BD4[9] = {
    ANIM(41, 0, 0, 4),
    ANIM(42, 0, 0, 4),
    ANIM(43, 0, 0, 4),
    ANIM_JUMP(44, 0, 0, 4, anim_80122BD4),
};
