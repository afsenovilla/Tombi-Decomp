// DATA 801328bc 896 X010
// Data of X010 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_801328BC[];
extern AnimWord anim_801328C4[];
extern AnimWord anim_801328CC[];
extern AnimWord anim_801328DC[];
extern AnimWord anim_801328EC[];
extern AnimWord anim_801328F4[];
extern AnimWord anim_80132908[];
extern AnimWord anim_8013291C[];
extern AnimWord anim_80132934[];
extern AnimWord anim_80132944[];
extern void *ptrs_80132964[];
extern AnimWord anim_80132978[];
extern AnimWord anim_80132980[];
extern AnimWord anim_801329B4[];
extern AnimWord anim_801329E0[];
extern AnimWord anim_801329E8[];
extern void *ptrs_80132A00[];
extern void *ptrs_80132A20[];
extern void *ptrs_80132A34[];
extern AnimWord anim_80132A54[];
extern AnimWord anim_80132A5C[];
extern AnimWord anim_80132A88[];
extern AnimWord anim_80132AB4[];
extern AnimWord anim_80132AD8[];
extern AnimWord anim_80132AF0[];
extern AnimWord anim_80132AF8[];
extern AnimWord anim_80132B1C[];
extern AnimWord anim_80132B40[];
extern AnimWord anim_80132B60[];
extern AnimWord anim_80132B68[];
extern AnimWord anim_80132B70[];
extern void *ptrs_80132B90[];
extern AnimWord anim_80132B94[];
extern void *ptrs_80132BA4[];
extern AnimWord anim_80132BA8[];
extern AnimWord anim_80132BD0[];
extern AnimWord anim_80132BD8[];
extern AnimWord anim_80132BE0[];
extern void *ptrs_80132C08[];
extern AnimWord anim_80132C0C[];
extern AnimWord anim_80132C34[];

AnimWord anim_801328BC[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_801328C4[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_801328CC[4] = {
    ANIM(4, 0, 0, 4),
    ANIM_STOP(5, 0, 0, 10),
};

AnimWord anim_801328DC[4] = {
    ANIM(5, 0, 0, 4),
    ANIM_STOP(4, 0, 0, 10),
};

AnimWord anim_801328EC[2] = {
    ANIM_STOP(1, 0, 0, 8),
};

AnimWord anim_801328F4[5] = {
    ANIM(2, 0, 0, 16),
    ANIM_JUMP(3, 0, 0, 16, anim_801328F4),
};

AnimWord anim_80132908[5] = {
    ANIM(6, 0, 0, 16),
    ANIM_JUMP(7, 0, 0, 16, anim_80132908),
};

AnimWord anim_8013291C[6] = {
    ANIM(16, 0, 0, 4),
    ANIM(17, 0, 0, 4),
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_80132934[4] = {
    ANIM(11, 0, 0, 4),
    ANIM_STOP(12, 0, 0, 60),
};

AnimWord anim_80132944[8] = {
    ANIM(8, 0, 0, 4),
    ANIM(9, 0, 0, 16),
    ANIM(1, 0, 0, 4),
    ANIM_STOP(0, 0, 0, 56),
};

void *ptrs_80132964[5] = {
    anim_80132978,
    anim_80132980,
    anim_801329B4,
    anim_801329E0,
    anim_801329E8,
};

AnimWord anim_80132978[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80132980[13] = {
    ANIM(1, 0, 0, 8),
    ANIM(2, 0, 0, 8),
    ANIM(3, 0, 0, 8),
    ANIM(4, 0, 0, 8),
    ANIM(5, 0, 0, 8),
    ANIM_JUMP(6, 0, 0, 8, anim_80132980),
};

AnimWord anim_801329B4[11] = {
    ANIM(7, 0, 0, 8),
    ANIM(8, 0, 0, 8),
    ANIM(9, 0, 0, 8),
    ANIM(8, 0, 0, 8),
    ANIM_JUMP(9, 0, 0, 8, anim_801329B4),
};

AnimWord anim_801329E0[2] = {
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_801329E8[6] = {
    ANIM(11, 0, 0, 5),
    ANIM(12, 0, 0, 3),
    ANIM_STOP(13, 0, 0, 60),
};

void *ptrs_80132A00[8] = {
    anim_80132B68,
    anim_80132BD8,
    anim_80132A54,
    anim_80132A5C,
    anim_80132A88,
    anim_80132AB4,
    anim_80132AD8,
    anim_80132AF0,
};

void *ptrs_80132A20[5] = {
    anim_80132AF8,
    anim_80132B1C,
    anim_80132B40,
    anim_80132B60,
    anim_80132B68,
};

void *ptrs_80132A34[8] = {
    anim_80132B70,
    anim_80132B94,
    anim_80132BA8,
    anim_80132BD0,
    anim_80132BD8,
    anim_80132BE0,
    anim_80132C0C,
    anim_80132C34,
};

AnimWord anim_80132A54[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80132A5C[11] = {
    ANIM(0, 0, 0, 8),
    ANIM(1, 0, 0, 8),
    ANIM(2, 0, 0, 8),
    ANIM(3, 0, 0, 8),
    ANIM_JUMP(4, 0, 0, 8, anim_80132A5C),
};

AnimWord anim_80132A88[11] = {
    ANIM(5, 0, 0, 12),
    ANIM(6, 0, 0, 8),
    ANIM(7, 0, 0, 4),
    ANIM(8, 0, 0, 8),
    ANIM_JUMP(6, 0, 0, 2, anim_80132A88),
};

AnimWord anim_80132AB4[9] = {
    ANIM(9, 0, 0, 12),
    ANIM(10, 0, 0, 12),
    ANIM(11, 0, 0, 12),
    ANIM_JUMP(10, 0, 0, 12, anim_80132AB4),
};

AnimWord anim_80132AD8[6] = {
    ANIM(10, 0, 0, 6),
    ANIM(12, 0, 0, 6),
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80132AF0[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_80132AF8[9] = {
    ANIM(13, 0, 0, 10),
    ANIM(14, 0, 0, 10),
    ANIM(15, 0, 0, 10),
    ANIM_JUMP(16, 0, 0, 10, anim_80132AF8),
};

AnimWord anim_80132B1C[9] = {
    ANIM(17, 0, 0, 10),
    ANIM(18, 0, 0, 10),
    ANIM(19, 0, 0, 10),
    ANIM_JUMP(20, 0, 0, 10, anim_80132B1C),
};

AnimWord anim_80132B40[8] = {
    ANIM(21, 0, 0, 4),
    ANIM(22, 0, 0, 3),
    ANIM(23, 0, 0, 2),
    ANIM_STOP(24, 0, 0, 4),
};

AnimWord anim_80132B60[2] = {
    ANIM_STOP(44, 0, 0, 60),
};

AnimWord anim_80132B68[2] = {
    ANIM_STOP(45, 0, 0, 60),
};

AnimWord anim_80132B70[8] = {
    ANIM(25, 0, 0, 8),
    ANIM(26, 0, 0, 8),
    ANIM(27, 0, 0, 8),
    ANIM_STOP(26, 0, 0, 8),
};

void *ptrs_80132B90[1] = {
    anim_80132B70,
};

AnimWord anim_80132B94[4] = {
    ANIM(26, 0, 0, 8),
    ANIM_STOP(28, 0, 0, 8),
};

void *ptrs_80132BA4[1] = {
    anim_80132B94,
};

AnimWord anim_80132BA8[10] = {
    ANIM(29, 0, 0, 4),
    ANIM(30, 0, 0, 3),
    ANIM(31, 0, 0, 2),
    ANIM(32, 0, 0, 3),
    ANIM_STOP(33, 0, 0, 4),
};

AnimWord anim_80132BD0[2] = {
    ANIM_STOP(46, 0, 0, 60),
};

AnimWord anim_80132BD8[2] = {
    ANIM_STOP(47, 0, 0, 60),
};

AnimWord anim_80132BE0[10] = {
    ANIM(34, 0, 0, 8),
    ANIM(35, 0, 0, 8),
    ANIM(36, 0, 0, 8),
    ANIM(37, 0, 0, 8),
    ANIM_STOP(38, 0, 0, 8),
};

void *ptrs_80132C08[1] = {
    anim_80132BE0,
};

AnimWord anim_80132C0C[10] = {
    ANIM(39, 0, 0, 4),
    ANIM(40, 0, 0, 3),
    ANIM(41, 0, 0, 2),
    ANIM(42, 0, 0, 3),
    ANIM_STOP(43, 0, 0, 4),
};

AnimWord anim_80132C34[2] = {
    ANIM_STOP(48, 0, 0, 60),
};
