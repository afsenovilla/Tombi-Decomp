// DATA 80122e50 372 X006
// Data of X006 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_80122FE8[];
extern char D_80122FF0[];
extern char D_80123004[];
extern char D_8012301C[];
extern char D_80123024[];
extern AnimWord anim_80122E50[];
extern AnimWord anim_80122E68[];
extern AnimWord anim_80122E80[];
extern AnimWord anim_80122E88[];
extern AnimWord anim_80122EA0[];
extern AnimWord anim_80122EA8[];
extern AnimWord anim_80122EB0[];
extern AnimWord anim_80122EB8[];
extern AnimWord anim_80122EC8[];
extern AnimWord anim_80122ED0[];
extern AnimWord anim_80122ED8[];
extern AnimWord anim_80122EE0[];
extern AnimWord anim_80122EE8[];
extern AnimWord anim_80122EF0[];
extern AnimWord anim_80122EF8[];
extern AnimWord anim_80122F00[];
extern AnimWord anim_80122F08[];
extern AnimWord anim_80122F10[];
extern AnimWord anim_80122F18[];
extern AnimWord anim_80122F20[];
extern AnimWord anim_80122F28[];
extern AnimWord anim_80122F30[];
extern AnimWord anim_80122F38[];
extern AnimWord anim_80122F40[];
extern AnimWord anim_80122F48[];
extern AnimWord anim_80122F50[];
extern AnimWord anim_80122F68[];
extern AnimWord anim_80122F70[];
extern AnimWord anim_80122F78[];
extern AnimWord anim_80122F80[];
extern AnimWord anim_80122F88[];
extern AnimWord anim_80122F90[];
extern AnimWord anim_80122F98[];
extern AnimWord anim_80122FA0[];
extern AnimWord anim_80122FA8[];
extern void *ptrs_80122FB0[];
extern void *ptrs_80122FB4[];
extern void *ptrs_80122FB8[];
extern void *ptrs_80122FBC[];

AnimWord anim_80122E50[6] = {
    ANIM(2, 0, 0, 3),
    ANIM(1, 0, 0, 3),
    ANIM_STOP(0, 0, 0, 3),
};

AnimWord anim_80122E68[6] = {
    ANIM(3, 0, 0, 3),
    ANIM(4, 0, 0, 3),
    ANIM_STOP(5, 0, 0, 3),
};

AnimWord anim_80122E80[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_80122E88[6] = {
    ANIM(6, 0, 0, 10),
    ANIM(7, 0, 0, 10),
    ANIM_STOP(8, 0, 0, 10),
};

AnimWord anim_80122EA0[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_80122EA8[2] = {
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_80122EB0[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_80122EB8[4] = {
    ANIM_STOP(12, 0, 0, 60),
    ANIM_STOP(13, 0, 0, 60),
};

AnimWord anim_80122EC8[2] = {
    ANIM_STOP(14, 0, 0, 60),
};

AnimWord anim_80122ED0[2] = {
    ANIM_STOP(15, 0, 0, 60),
};

AnimWord anim_80122ED8[2] = {
    ANIM_STOP(16, 0, 0, 60),
};

AnimWord anim_80122EE0[2] = {
    ANIM_STOP(17, 0, 0, 60),
};

AnimWord anim_80122EE8[2] = {
    ANIM_STOP(18, 0, 0, 60),
};

AnimWord anim_80122EF0[2] = {
    ANIM_STOP(19, 0, 0, 60),
};

AnimWord anim_80122EF8[2] = {
    ANIM_STOP(20, 0, 0, 60),
};

AnimWord anim_80122F00[2] = {
    ANIM_STOP(21, 0, 0, 60),
};

AnimWord anim_80122F08[2] = {
    ANIM_STOP(22, 0, 0, 60),
};

AnimWord anim_80122F10[2] = {
    ANIM_STOP(23, 0, 0, 60),
};

AnimWord anim_80122F18[2] = {
    ANIM_STOP(24, 0, 0, 60),
};

AnimWord anim_80122F20[2] = {
    ANIM_STOP(25, 0, 0, 60),
};

AnimWord anim_80122F28[2] = {
    ANIM_STOP(26, 0, 0, 60),
};

AnimWord anim_80122F30[2] = {
    ANIM_STOP(27, 0, 0, 60),
};

AnimWord anim_80122F38[2] = {
    ANIM_STOP(31, 0, 0, 60),
};

AnimWord anim_80122F40[2] = {
    ANIM_STOP(32, 0, 0, 60),
};

AnimWord anim_80122F48[2] = {
    ANIM_STOP(33, 0, 0, 60),
};

AnimWord anim_80122F50[6] = {
    ANIM(28, 0, 0, 3),
    ANIM(29, 0, 0, 3),
    ANIM_STOP(30, 0, 0, 3),
};

AnimWord anim_80122F68[2] = {
    ANIM_STOP(34, 0, 0, 60),
};

AnimWord anim_80122F70[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_80122F78[2] = {
    ANIM_STOP(36, 0, 0, 60),
};

AnimWord anim_80122F80[2] = {
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_80122F88[2] = {
    ANIM_STOP(38, 0, 0, 60),
};

AnimWord anim_80122F90[2] = {
    ANIM_STOP(39, 0, 0, 60),
};

AnimWord anim_80122F98[2] = {
    ANIM_STOP(40, 0, 0, 60),
};

AnimWord anim_80122FA0[2] = {
    ANIM_STOP(41, 0, 0, 60),
};

AnimWord anim_80122FA8[2] = {
    ANIM_STOP(42, 0, 0, 60),
};

void *ptrs_80122FB0[1] = {
    D_80122FE8,
};

void *ptrs_80122FB4[1] = {
    D_80122FF0,
};

void *ptrs_80122FB8[1] = {
    D_80123004,
};

void *ptrs_80122FBC[2] = {
    D_8012301C,
    D_80123024,
};
