// DATA 8011fb98 368 X002
// Data of X002 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void *ptrs_8011FB98[];
extern void *ptrs_8011FBAC[];
extern void *ptrs_8011FBB0[];
extern void *ptrs_8011FBB4[];
extern void *ptrs_8011FBB8[];
extern void *ptrs_8011FBBC[];
extern void *ptrs_8011FBC0[];
extern void *ptrs_8011FBC4[];
extern AnimWord anim_8011FBC8[];
extern AnimWord anim_8011FBD0[];
extern AnimWord anim_8011FBD8[];
extern AnimWord anim_8011FBE0[];
extern AnimWord anim_8011FBE8[];
extern AnimWord anim_8011FBF0[];
extern AnimWord anim_8011FBF8[];
extern AnimWord anim_8011FC00[];
extern AnimWord anim_8011FC08[];
extern AnimWord anim_8011FC10[];
extern AnimWord anim_8011FC18[];
extern AnimWord anim_8011FC20[];
extern AnimWord anim_8011FC28[];
extern AnimWord anim_8011FC30[];
extern AnimWord anim_8011FC38[];
extern AnimWord anim_8011FC40[];
extern AnimWord anim_8011FC48[];
extern AnimWord anim_8011FC50[];
extern AnimWord anim_8011FC58[];
extern AnimWord anim_8011FC60[];
extern AnimWord anim_8011FC68[];
extern AnimWord anim_8011FC70[];
extern AnimWord anim_8011FC78[];
extern AnimWord anim_8011FC80[];
extern AnimWord anim_8011FCB4[];
extern AnimWord anim_8011FCE8[];

void *ptrs_8011FB98[5] = {
    anim_8011FC78,
    anim_8011FC80,
    anim_8011FCB4,
    anim_8011FCE8,
    anim_8011FBC8,
};

void *ptrs_8011FBAC[1] = {
    anim_8011FBD0,
};

void *ptrs_8011FBB0[1] = {
    anim_8011FBD8,
};

void *ptrs_8011FBB4[1] = {
    anim_8011FBE0,
};

void *ptrs_8011FBB8[1] = {
    anim_8011FBE8,
};

void *ptrs_8011FBBC[1] = {
    anim_8011FBF0,
};

void *ptrs_8011FBC0[1] = {
    anim_8011FBF8,
};

void *ptrs_8011FBC4[1] = {
    anim_8011FC00,
};

AnimWord anim_8011FBC8[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8011FBD0[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8011FBD8[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8011FBE0[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8011FBE8[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8011FBF0[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_8011FBF8[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_8011FC00[2] = {
    ANIM_STOP(7, 0, 0, 60),
};

AnimWord anim_8011FC08[2] = {
    ANIM_STOP(8, 0, 0, 60),
};

AnimWord anim_8011FC10[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_8011FC18[2] = {
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_8011FC20[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8011FC28[2] = {
    ANIM_STOP(12, 0, 0, 60),
};

AnimWord anim_8011FC30[2] = {
    ANIM_STOP(13, 0, 0, 60),
};

AnimWord anim_8011FC38[2] = {
    ANIM_STOP(14, 0, 0, 60),
};

AnimWord anim_8011FC40[2] = {
    ANIM_STOP(15, 0, 0, 60),
};

AnimWord anim_8011FC48[2] = {
    ANIM_STOP(16, 0, 0, 60),
};

AnimWord anim_8011FC50[2] = {
    ANIM_STOP(17, 0, 0, 60),
};

AnimWord anim_8011FC58[2] = {
    ANIM_STOP(18, 0, 0, 60),
};

AnimWord anim_8011FC60[2] = {
    ANIM_STOP(19, 0, 0, 60),
};

AnimWord anim_8011FC68[2] = {
    ANIM_STOP(20, 0, 0, 60),
};

AnimWord anim_8011FC70[2] = {
    ANIM_STOP(21, 0, 0, 60),
};

AnimWord anim_8011FC78[2] = {
    ANIM_STOP(22, 0, 0, 60),
};

AnimWord anim_8011FC80[13] = {
    ANIM(23, 0, 0, 8),
    ANIM(24, 0, 0, 8),
    ANIM(25, 0, 0, 8),
    ANIM(26, 0, 0, 8),
    ANIM(27, 0, 0, 8),
    ANIM_JUMP(28, 0, 0, 8, anim_8011FC80),
};

AnimWord anim_8011FCB4[13] = {
    ANIM(29, 0, 0, 8),
    ANIM(30, 0, 0, 12),
    ANIM(31, 0, 0, 12),
    ANIM(30, 0, 0, 12),
    ANIM(31, 0, 0, 12),
    ANIM_JUMP(30, 0, 0, 12, anim_8011FCB4),
};

AnimWord anim_8011FCE8[8] = {
    ANIM(22, 0, 0, 24),
    ANIM(32, 0, 0, 6),
    ANIM(33, 0, 0, 6),
    ANIM_STOP(34, 0, 0, 24),
};
