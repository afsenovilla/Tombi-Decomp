// DATA 8013fcb0 756 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void *ptrs_8013FCB0[];
extern void *ptrs_8013FCB8[];
extern void *ptrs_8013FCBC[];
extern void *ptrs_8013FCC0[];
extern void *ptrs_8013FCC4[];
extern void *ptrs_8013FCC8[];
extern void *ptrs_8013FCCC[];
extern unsigned int data_8013FCD0[];
extern AnimWord anim_8013FCD8[];
extern AnimWord anim_8013FD1C[];
extern AnimWord anim_8013FD54[];
extern AnimWord anim_8013FD8C[];
extern AnimWord anim_8013FDC4[];
extern AnimWord anim_8013FDDC[];
extern AnimWord anim_8013FDE4[];
extern AnimWord anim_8013FE04[];
extern AnimWord anim_8013FE28[];
extern AnimWord anim_8013FE40[];
extern AnimWord anim_8013FE48[];
extern AnimWord anim_8013FE50[];
extern AnimWord anim_8013FE58[];
extern AnimWord anim_8013FE68[];
extern AnimWord anim_8013FE78[];
extern AnimWord anim_8013FE94[];
extern AnimWord anim_8013FEAC[];
extern AnimWord anim_8013FEB4[];
extern AnimWord anim_8013FEDC[];
extern AnimWord anim_8013FF00[];
extern AnimWord anim_8013FF08[];
extern AnimWord anim_8013FF48[];
extern AnimWord anim_8013FF74[];

void *ptrs_8013FCB0[2] = {
    anim_8013FEDC,
    anim_8013FF00,
};

void *ptrs_8013FCB8[1] = {
    anim_8013FF08,
};

void *ptrs_8013FCBC[1] = {
    anim_8013FF48,
};

void *ptrs_8013FCC0[1] = {
    anim_8013FF74,
};

void *ptrs_8013FCC4[1] = {
    anim_8013FD54,
};

void *ptrs_8013FCC8[1] = {
    anim_8013FD8C,
};

void *ptrs_8013FCCC[1] = {
    anim_8013FE78,
};

unsigned int data_8013FCD0[2] = {
    0x0, 0x80000000,
};

AnimWord anim_8013FCD8[17] = {
    ANIM(1, 0, 0, 4),
    ANIM(2, 0, 0, 4),
    ANIM(3, 0, 0, 4),
    ANIM(4, 0, 0, 4),
    ANIM(5, 0, 0, 4),
    ANIM(6, 0, 0, 4),
    ANIM(7, 0, 0, 4),
    ANIM_JUMP(8, 0, 0, 4, anim_8013FCD8),
};

AnimWord anim_8013FD1C[14] = {
    ANIM(9, 0, 0, 4),
    ANIM(10, 0, 0, 4),
    ANIM(11, 0, 0, 4),
    ANIM(12, 0, 0, 4),
    ANIM(13, 0, 0, 4),
    ANIM(14, 0, 0, 4),
    ANIM_STOP(15, 0, 0, 4),
};

AnimWord anim_8013FD54[14] = {
    ANIM(15, 0, 0, 4),
    ANIM(14, 0, 0, 4),
    ANIM(13, 0, 0, 4),
    ANIM(12, 0, 0, 4),
    ANIM(11, 0, 0, 4),
    ANIM(10, 0, 0, 4),
    ANIM_STOP(9, 0, 0, 4),
};

AnimWord anim_8013FD8C[14] = {
    ANIM(44, 0, 1, 4),
    ANIM(43, 0, 1, 4),
    ANIM(42, 0, 1, 4),
    ANIM(41, 0, 1, 4),
    ANIM(40, 0, 1, 4),
    ANIM(39, 0, 1, 4),
    ANIM_STOP(38, 0, 1, 4),
};

AnimWord anim_8013FDC4[6] = {
    ANIM(16, 0, 0, 4),
    ANIM(17, 0, 0, 4),
    ANIM_STOP(18, 0, 0, 16),
};

AnimWord anim_8013FDDC[2] = {
    ANIM_STOP(21, 0, 0, 10),
};

AnimWord anim_8013FDE4[8] = {
    ANIM(22, 0, 0, 4),
    ANIM(23, 0, 0, 4),
    ANIM(24, 0, 0, 4),
    ANIM_STOP(25, 0, 0, 4),
};

AnimWord anim_8013FE04[9] = {
    ANIM(26, 0, 0, 4),
    ANIM(27, 0, 0, 4),
    ANIM(28, 0, 0, 4),
    ANIM_JUMP(29, 0, 0, 4, anim_8013FE04),
};

AnimWord anim_8013FE28[6] = {
    ANIM(30, 0, 0, 4),
    ANIM(31, 0, 0, 4),
    ANIM_STOP(32, 0, 0, 4),
};

AnimWord anim_8013FE40[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_8013FE48[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_8013FE50[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_8013FE58[4] = {
    ANIM(33, 0, 0, 4),
    ANIM_STOP(34, 0, 0, 16),
};

AnimWord anim_8013FE68[4] = {
    ANIM(19, 0, 0, 4),
    ANIM(20, 0, 0, 4),
};

AnimWord anim_8013FE78[7] = {
    ANIM(54, 0, 0, 4),
    ANIM(55, 0, 0, 4),
    ANIM_JUMP(56, 0, 0, 4, anim_8013FE78),
};

AnimWord anim_8013FE94[6] = {
    ANIM(45, 0, 0, 6),
    ANIM(46, 0, 0, 6),
    ANIM(47, 0, 0, 6),
};

AnimWord anim_8013FEAC[2] = {
    ANIM_STOP(48, 0, 0, 60),
};

AnimWord anim_8013FEB4[10] = {
    ANIM(49, 0, 0, 4),
    ANIM(50, 0, 0, 4),
    ANIM(51, 0, 0, 4),
    ANIM(52, 0, 0, 4),
    ANIM_STOP(53, 0, 0, 4),
};

AnimWord anim_8013FEDC[9] = {
    ANIM(57, 0, 0, 32),
    ANIM(58, 0, 0, 8),
    ANIM(59, 0, 0, 48),
    ANIM_JUMP(58, 0, 0, 12, anim_8013FEDC),
};

AnimWord anim_8013FF00[2] = {
    ANIM_STOP(60, 0, 0, 60),
};

AnimWord anim_8013FF08[16] = {
    ANIM(61, 0, 0, 4),
    ANIM(62, 0, 0, 16),
    ANIM(63, 0, 0, 4),
    ANIM(64, 0, 0, 4),
    ANIM(65, 0, 0, 4),
    ANIM(63, 0, 0, 4),
    ANIM(64, 0, 0, 4),
    ANIM_STOP(65, 0, 0, 4),
};

AnimWord anim_8013FF48[11] = {
    ANIM(66, 0, 0, 4),
    ANIM(67, 0, 0, 4),
    ANIM(68, 0, 0, 4),
    ANIM(69, 0, 0, 4),
    ANIM_JUMP(70, 0, 0, 4, anim_8013FF48),
};

AnimWord anim_8013FF74[12] = {
    ANIM(71, 0, 0, 4),
    ANIM(72, 0, 0, 4),
    ANIM(73, 0, 0, 4),
    ANIM(74, 0, 0, 4),
    ANIM(75, 0, 0, 4),
    ANIM_STOP(76, 0, 0, 32),
};
