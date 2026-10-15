// DATA 8013a378 844 X003
// Data of X003 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013A718[];
extern char D_8013A72C[];
extern char D_8013A740[];
extern char D_8013A754[];
extern char D_8013A774[];
extern char D_8013A7C8[];
extern char D_8013A82C[];
extern char D_8013A844[];
extern char D_8013A84C[];
extern char D_8013A860[];
extern char D_8013A884[];
extern void *ptrs_8013A378[];
extern void *ptrs_8013A37C[];
extern void *ptrs_8013A38C[];
extern void *ptrs_8013A3A8[];
extern void *ptrs_8013A3B4[];
extern AnimWord anim_8013A3BC[];
extern AnimWord anim_8013A3E0[];
extern AnimWord anim_8013A404[];
extern AnimWord anim_8013A440[];
extern AnimWord anim_8013A47C[];
extern AnimWord anim_8013A490[];
extern AnimWord anim_8013A4B0[];
extern AnimWord anim_8013A4B8[];
extern AnimWord anim_8013A4C0[];
extern AnimWord anim_8013A4D4[];
extern AnimWord anim_8013A4DC[];
extern AnimWord anim_8013A4EC[];
extern AnimWord anim_8013A4F4[];
extern AnimWord anim_8013A4FC[];
extern AnimWord anim_8013A524[];
extern AnimWord anim_8013A548[];
extern AnimWord anim_8013A550[];
extern AnimWord anim_8013A558[];
extern AnimWord anim_8013A560[];
extern AnimWord anim_8013A594[];
extern AnimWord anim_8013A5C8[];
extern AnimWord anim_8013A60C[];
extern AnimWord anim_8013A640[];
extern AnimWord anim_8013A648[];
extern AnimWord anim_8013A678[];
extern AnimWord anim_8013A680[];
extern AnimWord anim_8013A690[];
extern void *ptrs_8013A698[];
extern void *ptrs_8013A69C[];
extern void *ptrs_8013A6A4[];
extern void *ptrs_8013A6A8[];
extern void *ptrs_8013A6B0[];
extern void *ptrs_8013A6B4[];
extern void *ptrs_8013A6B8[];
extern void *ptrs_8013A6BC[];
extern void *ptrs_8013A6C0[];

void *ptrs_8013A378[1] = {
    anim_8013A60C,
};

void *ptrs_8013A37C[4] = {
    anim_8013A4B8,
    anim_8013A4C0,
    anim_8013A4D4,
    anim_8013A4DC,
};

void *ptrs_8013A38C[7] = {
    anim_8013A4B0,
    anim_8013A47C,
    anim_8013A3BC,
    anim_8013A3E0,
    anim_8013A404,
    anim_8013A440,
    anim_8013A490,
};

void *ptrs_8013A3A8[3] = {
    anim_8013A640,
    anim_8013A648,
    anim_8013A678,
};

void *ptrs_8013A3B4[2] = {
    anim_8013A680,
    anim_8013A690,
};

AnimWord anim_8013A3BC[9] = {
    ANIM(0, 0, 0, 6),
    ANIM(1, 0, 0, 4),
    ANIM(2, 0, 0, 6),
    ANIM_JUMP(1, 0, 0, 4, anim_8013A3BC),
};

AnimWord anim_8013A3E0[9] = {
    ANIM(3, 0, 0, 7),
    ANIM(4, 0, 0, 7),
    ANIM(5, 0, 0, 7),
    ANIM_JUMP(4, 0, 0, 7, anim_8013A3E0),
};

AnimWord anim_8013A404[15] = {
    ANIM(6, 0, 0, 12),
    ANIM(7, 0, 0, 5),
    ANIM(8, 0, 0, 3),
    ANIM(9, 0, 0, 3),
    ANIM(10, 0, 0, 3),
    ANIM(11, 0, 0, 3),
    ANIM_JUMP(12, 0, 0, 8, anim_8013A404),
};

AnimWord anim_8013A440[15] = {
    ANIM(13, 0, 0, 12),
    ANIM(14, 0, 0, 5),
    ANIM(15, 0, 0, 3),
    ANIM(16, 0, 0, 3),
    ANIM(17, 0, 0, 3),
    ANIM(18, 0, 0, 3),
    ANIM_JUMP(19, 0, 0, 8, anim_8013A440),
};

AnimWord anim_8013A47C[5] = {
    ANIM(20, 0, 0, 16),
    ANIM_JUMP(21, 0, 0, 16, anim_8013A47C),
};

AnimWord anim_8013A490[8] = {
    ANIM(22, 0, 0, 16),
    ANIM(23, 0, 0, 4),
    ANIM(24, 0, 0, 6),
    ANIM_STOP(23, 0, 0, 4),
};

AnimWord anim_8013A4B0[2] = {
    ANIM_STOP(25, 0, 0, 60),
};

AnimWord anim_8013A4B8[2] = {
    ANIM_STOP(26, 0, 0, 60),
};

AnimWord anim_8013A4C0[5] = {
    ANIM(27, 0, 0, 8),
    ANIM_JUMP(28, 0, 0, 16, anim_8013A4C0),
};

AnimWord anim_8013A4D4[2] = {
    ANIM_STOP(29, 0, 0, 60),
};

AnimWord anim_8013A4DC[4] = {
    ANIM(30, 0, 0, 6),
    ANIM_STOP(31, 0, 0, 60),
};

AnimWord anim_8013A4EC[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_8013A4F4[2] = {
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_8013A4FC[10] = {
    ANIM(32, 0, 0, 8),
    ANIM(33, 0, 0, 7),
    ANIM(34, 0, 0, 7),
    ANIM(35, 0, 0, 7),
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_8013A524[9] = {
    ANIM(32, 0, 0, 8),
    ANIM(33, 0, 0, 8),
    ANIM(34, 0, 0, 8),
    ANIM_JUMP(35, 0, 0, 8, anim_8013A524),
};

AnimWord anim_8013A548[2] = {
    ANIM_STOP(49, 0, 0, 60),
};

AnimWord anim_8013A550[2] = {
    ANIM_STOP(48, 0, 0, 60),
};

AnimWord anim_8013A558[2] = {
    ANIM_STOP(38, 0, 0, 60),
};

AnimWord anim_8013A560[13] = {
    ANIM(46, 0, 0, 6),
    ANIM(40, 0, 0, 3),
    ANIM(41, 0, 0, 3),
    ANIM(47, 0, 0, 6),
    ANIM(41, 0, 0, 3),
    ANIM_JUMP(40, 0, 0, 3, anim_8013A560),
};

AnimWord anim_8013A594[13] = {
    ANIM(39, 0, 0, 10),
    ANIM(40, 0, 0, 7),
    ANIM(41, 0, 0, 7),
    ANIM(42, 0, 0, 10),
    ANIM(41, 0, 0, 7),
    ANIM_JUMP(40, 0, 0, 7, anim_8013A594),
};

AnimWord anim_8013A5C8[17] = {
    ANIM(38, 0, 0, 6),
    ANIM(43, 0, 0, 8),
    ANIM(44, 0, 0, 8),
    ANIM(45, 0, 0, 8),
    ANIM(44, 0, 0, 8),
    ANIM(45, 0, 0, 8),
    ANIM(44, 0, 0, 6),
    ANIM_JUMP(43, 0, 0, 6, anim_8013A5C8),
};

AnimWord anim_8013A60C[13] = {
    ANIM(38, 0, 0, 6),
    ANIM(43, 0, 0, 8),
    ANIM(44, 0, 0, 8),
    ANIM(45, 0, 0, 8),
    ANIM(44, 0, 0, 6),
    ANIM_JUMP(43, 0, 0, 20, anim_8013A60C),
};

AnimWord anim_8013A640[2] = {
    ANIM_STOP(77, 0, 0, 60),
};

AnimWord anim_8013A648[12] = {
    ANIM(78, 0, 0, 8),
    ANIM(79, 0, 0, 8),
    ANIM(80, 0, 0, 8),
    ANIM(81, 0, 0, 8),
    ANIM(82, 0, 0, 8),
    ANIM_STOP(83, 0, 0, 8),
};

AnimWord anim_8013A678[2] = {
    ANIM_STOP(84, 0, 0, 8),
};

AnimWord anim_8013A680[4] = {
    ANIM(85, 0, 0, 16),
    ANIM_STOP(86, 0, 0, 16),
};

AnimWord anim_8013A690[2] = {
    ANIM_STOP(87, 0, 0, 60),
};

void *ptrs_8013A698[1] = {
    D_8013A718,
};

void *ptrs_8013A69C[2] = {
    D_8013A72C,
    D_8013A740,
};

void *ptrs_8013A6A4[1] = {
    D_8013A754,
};

void *ptrs_8013A6A8[2] = {
    D_8013A774,
    D_8013A7C8,
};

void *ptrs_8013A6B0[1] = {
    D_8013A82C,
};

void *ptrs_8013A6B4[1] = {
    D_8013A844,
};

void *ptrs_8013A6B8[1] = {
    D_8013A84C,
};

void *ptrs_8013A6BC[1] = {
    D_8013A860,
};

void *ptrs_8013A6C0[1] = {
    D_8013A884,
};
