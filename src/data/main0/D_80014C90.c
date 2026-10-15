// DATA 80014c90 1372 MAIN0
// Data of MAIN0 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80090000[];
extern unsigned int data_80014C90[];
extern void *ptrs_80015028[];
extern void *ptrs_80015040[];
extern void *ptrs_80015054[];
extern void *ptrs_8001506C[];
extern AnimWord anim_80015084[];
extern AnimWord anim_8001508C[];
extern AnimWord anim_800150B0[];
extern void *ptrs_800150D0[];
extern AnimWord anim_800150D4[];
extern AnimWord anim_800150DC[];
extern AnimWord anim_800150E4[];
extern AnimWord anim_80015104[];
extern AnimWord anim_8001510C[];
extern AnimWord anim_80015114[];
extern AnimWord anim_8001511C[];
extern AnimWord anim_80015124[];
extern AnimWord anim_8001512C[];
extern AnimWord anim_80015150[];
extern AnimWord anim_80015158[];
extern AnimWord anim_8001517C[];
extern AnimWord anim_800151A0[];
extern AnimWord anim_800151A8[];
extern AnimWord anim_800151B0[];
extern AnimWord anim_800151B8[];
extern AnimWord anim_800151C0[];
extern AnimWord anim_800151C8[];
extern AnimWord anim_800151D0[];
extern AnimWord anim_800151D8[];

unsigned int data_80014C90[230] = {
    0xA9, 0xE0000, 0xAA, 0x800E0000,
    0x94, 0x803C0000, 0x95, 0x803C0000,
    0x96, 0x803C0000, 0x97, 0x80000,
    0x98, 0x80000, 0x99, 0x80000,
    0x9A, 0x80000, 0x9B, 0x80000,
    0x9C, 0x80080000, 0x9D, 0x80000,
    0x9E, 0x80000, 0x9F, 0x80000,
    0xA0, 0x80000, 0xA1, 0x80000,
    0xA2, 0x80080000, 0xA3, 0x80000,
    0xA4, 0x80000, 0xA5, 0x80000,
    0xA6, 0x80000, 0xA7, 0x80000,
    0xA8, 0x80080000, 0xAB, 0xE0000,
    0xAC, 0x800E0000, 0xAD, 0xE0000,
    0xAE, 0x800E0000, 0xAF, 0x120000,
    0xB0, 0x120000, 0xAF, 0x80000,
    0xB0, 0x80000, 0xAF, 0x80000,
    0xB0, 0x80000, 0xAF, 0x120000,
    0xB0, 0x80120000, 0xB1, 0x120000,
    0xB2, 0x120000, 0xB3, 0x80000,
    0xB2, 0x80000, 0xB3, 0x80000,
    0xB2, 0x80000, 0xB3, 0x120000,
    0xB2, 0x80120000, 0xB4, 0x120000,
    0xB5, 0x120000, 0xB6, 0x80000,
    0xB5, 0x80000, 0xB6, 0x80000,
    0xB5, 0x80000, 0xB6, 0x120000,
    0xB5, 0x80120000, 0xB7, 0x120000,
    0xB8, 0x60000, 0xB7, 0x60000,
    0xB8, 0x60000, 0xB7, 0x60000,
    0xB8, 0x60000, 0xB7, 0x60000,
    0xB8, 0x60000, 0xB7, 0x60000,
    0xB8, 0x80000, 0xB7, 0x80000,
    0xB8, 0x800A0000, 0xB9, 0x120000,
    0xBA, 0x60000, 0xB9, 0x60000,
    0xBA, 0x60000, 0xB9, 0x60000,
    0xBA, 0x60000, 0xB9, 0x60000,
    0xBA, 0x60000, 0xB9, 0x60000,
    0xBA, 0x80000, 0xB9, 0x80000,
    0xBA, 0x800A0000, 0xBB, 0x120000,
    0xBC, 0x60000, 0xBB, 0x60000,
    0xBC, 0x60000, 0xBB, 0x60000,
    0xBC, 0x60000, 0xBB, 0x60000,
    0xBC, 0x60000, 0xBB, 0x60000,
    0xBC, 0x80000, 0xBB, 0x80000,
    0xBC, 0x800A0000, 0xBD, 0x803C0000,
    0xBE, 0x803C0000, 0xBF, 0x803C0000,
    0xC0, 0x70000, 0xC1, 0x100000,
    0xC0, 0x50000, 0x94, 0x80280000,
    0xC2, 0x70000, 0xC3, 0x100000,
    0xC2, 0x50000, 0xC4, 0x80280000,
    0xC5, 0x70000, 0xC6, 0x100000,
    0xC5, 0x50000, 0xC7, 0x80280000,
    0x95, 0x90000, 0xC2, 0x90000,
    0xC0, 0x90000, 0xCA, 0xB0000,
    0xC9, 0x80060000, 0xC8, 0xB0000,
    0xC9, 0x60000, 0xCA, 0xB0000,
    0xC9, 0x80060000, 0xAA, 0x70000,
    0xC0, 0x90000, 0xC3, 0x90000,
    0x95, (unsigned int)D_80090000,
};

void *ptrs_80015028[6] = {
    anim_800150D4,
    anim_800150DC,
    anim_80015084,
    anim_8001508C,
    anim_800150B0,
    anim_800150E4,
};

void *ptrs_80015040[5] = {
    anim_800151A0,
    anim_800151A8,
    anim_80015150,
    anim_80015158,
    anim_8001517C,
};

void *ptrs_80015054[6] = {
    anim_80015114,
    anim_8001511C,
    anim_8001510C,
    anim_8001512C,
    anim_80015104,
    anim_80015124,
};

void *ptrs_8001506C[6] = {
    anim_800151C0,
    anim_800151C8,
    anim_800151B8,
    anim_800151D8,
    anim_800151B0,
    anim_800151D0,
};

AnimWord anim_80015084[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8001508C[9] = {
    ANIM(1, 0, 0, 3),
    ANIM(2, 0, 0, 3),
    ANIM(3, 0, 0, 3),
    ANIM_JUMP(4, 0, 0, 3, anim_8001508C),
};

AnimWord anim_800150B0[8] = {
    ANIM(5, 0, 0, 8),
    ANIM(6, 0, 0, 8),
    ANIM(7, 0, 0, 8),
    ANIM_STOP(8, 0, 0, 8),
};

void *ptrs_800150D0[1] = {
    anim_800150B0,
};

AnimWord anim_800150D4[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_800150DC[2] = {
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_800150E4[8] = {
    ANIM(20, 0, 0, 16),
    ANIM(21, 0, 0, 5),
    ANIM(22, 0, 0, 4),
    ANIM_STOP(21, 0, 0, 3),
};

AnimWord anim_80015104[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8001510C[2] = {
    ANIM_STOP(12, 0, 0, 60),
};

AnimWord anim_80015114[2] = {
    ANIM_STOP(13, 0, 0, 60),
};

AnimWord anim_8001511C[2] = {
    ANIM_STOP(14, 0, 0, 60),
};

AnimWord anim_80015124[2] = {
    ANIM_STOP(15, 0, 0, 60),
};

AnimWord anim_8001512C[9] = {
    ANIM(16, 0, 0, 3),
    ANIM(17, 0, 0, 3),
    ANIM(18, 0, 0, 3),
    ANIM_JUMP(19, 0, 0, 3, anim_8001512C),
};

AnimWord anim_80015150[2] = {
    ANIM_STOP(23, 0, 0, 60),
};

AnimWord anim_80015158[9] = {
    ANIM(24, 0, 0, 8),
    ANIM(25, 0, 0, 8),
    ANIM(26, 0, 0, 8),
    ANIM_JUMP(27, 0, 0, 8, anim_80015158),
};

AnimWord anim_8001517C[9] = {
    ANIM(28, 0, 0, 8),
    ANIM(29, 0, 0, 8),
    ANIM(30, 0, 0, 8),
    ANIM_JUMP(31, 0, 0, 8, anim_8001517C),
};

AnimWord anim_800151A0[2] = {
    ANIM_STOP(32, 0, 0, 60),
};

AnimWord anim_800151A8[2] = {
    ANIM_STOP(33, 0, 0, 60),
};

AnimWord anim_800151B0[2] = {
    ANIM_STOP(34, 0, 0, 60),
};

AnimWord anim_800151B8[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_800151C0[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_800151C8[2] = {
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_800151D0[2] = {
    ANIM_STOP(38, 0, 0, 60),
};

AnimWord anim_800151D8[5] = {
    ANIM(35, 0, 0, 3),
    ANIM_JUMP(39, 0, 0, 4, anim_800151D8),
};
