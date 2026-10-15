// DATA 8013fab0 496 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013FCD0[];
extern char D_8013FCD8[];
extern char D_8013FD1C[];
extern char D_8013FDC4[];
extern char D_8013FDDC[];
extern char D_8013FDE4[];
extern char D_8013FE04[];
extern char D_8013FE28[];
extern char D_8013FE40[];
extern char D_8013FE48[];
extern char D_8013FE50[];
extern char D_8013FE58[];
extern void *ptrs_8013FAB0[];
extern void *ptrs_8013FAB4[];
extern void *ptrs_8013FAB8[];
extern void *ptrs_8013FABC[];
extern void *ptrs_8013FAC0[];
extern void *ptrs_8013FAC4[];
extern void *ptrs_8013FAC8[];
extern void *ptrs_8013FACC[];
extern void *ptrs_8013FAD0[];
extern AnimWord anim_8013FAD4[];
extern AnimWord anim_8013FADC[];
extern AnimWord anim_8013FAE4[];
extern AnimWord anim_8013FAEC[];
extern AnimWord anim_8013FAF4[];
extern AnimWord anim_8013FAFC[];
extern AnimWord anim_8013FB04[];
extern AnimWord anim_8013FB0C[];
extern AnimWord anim_8013FB14[];
extern AnimWord anim_8013FB54[];
extern AnimWord anim_8013FB6C[];
extern AnimWord anim_8013FB90[];
extern AnimWord anim_8013FBB4[];
extern AnimWord anim_8013FBD4[];
extern AnimWord anim_8013FBDC[];
extern AnimWord anim_8013FC4C[];
extern AnimWord anim_8013FC60[];
extern void *ptrs_8013FC70[];
extern void *ptrs_8013FC74[];
extern void *ptrs_8013FC78[];
extern void *ptrs_8013FC7C[];
extern void *ptrs_8013FC84[];
extern void *ptrs_8013FC88[];
extern void *ptrs_8013FC8C[];
extern void *ptrs_8013FC90[];
extern void *ptrs_8013FC94[];

void *ptrs_8013FAB0[1] = {
    anim_8013FB14,
};

void *ptrs_8013FAB4[1] = {
    anim_8013FB54,
};

void *ptrs_8013FAB8[1] = {
    anim_8013FB6C,
};

void *ptrs_8013FABC[1] = {
    anim_8013FB90,
};

void *ptrs_8013FAC0[1] = {
    anim_8013FBB4,
};

void *ptrs_8013FAC4[1] = {
    anim_8013FBD4,
};

void *ptrs_8013FAC8[1] = {
    anim_8013FBDC,
};

void *ptrs_8013FACC[1] = {
    anim_8013FC4C,
};

void *ptrs_8013FAD0[1] = {
    anim_8013FC60,
};

AnimWord anim_8013FAD4[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8013FADC[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8013FAE4[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8013FAEC[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8013FAF4[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8013FAFC[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_8013FB04[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_8013FB0C[2] = {
    ANIM_STOP(7, 0, 0, 60),
};

AnimWord anim_8013FB14[16] = {
    ANIM(8, 0, 0, 9),
    ANIM(9, 0, 0, 5),
    ANIM(10, 0, 0, 7),
    ANIM(11, 0, 0, 5),
    ANIM(12, 0, 0, 9),
    ANIM(13, 0, 0, 5),
    ANIM(14, 0, 0, 7),
    ANIM_STOP(15, 0, 0, 5),
};

AnimWord anim_8013FB54[6] = {
    ANIM(16, 0, 0, 5),
    ANIM(17, 0, 0, 5),
    ANIM_STOP(18, 0, 0, 5),
};

AnimWord anim_8013FB6C[9] = {
    ANIM(19, 0, 0, 8),
    ANIM(20, 0, 0, 8),
    ANIM(21, 0, 0, 8),
    ANIM_JUMP(20, 0, 0, 8, anim_8013FB6C),
};

AnimWord anim_8013FB90[9] = {
    ANIM(22, 0, 0, 8),
    ANIM(23, 0, 0, 8),
    ANIM(24, 0, 0, 8),
    ANIM_JUMP(23, 0, 0, 8, anim_8013FB90),
};

AnimWord anim_8013FBB4[8] = {
    ANIM(25, 0, 0, 9),
    ANIM(26, 0, 0, 7),
    ANIM(27, 0, 0, 9),
    ANIM_STOP(26, 0, 0, 7),
};

AnimWord anim_8013FBD4[2] = {
    ANIM_STOP(28, 0, 0, 60),
};

AnimWord anim_8013FBDC[28] = {
    ANIM(29, 0, 0, 3),
    ANIM(30, 0, 0, 4),
    ANIM(31, 0, 0, 5),
    ANIM(32, 0, 0, 4),
    ANIM(33, 0, 0, 3),
    ANIM(34, 0, 0, 4),
    ANIM(35, 0, 0, 6),
    ANIM(36, 0, 0, 3),
    ANIM(37, 0, 0, 4),
    ANIM(38, 0, 0, 5),
    ANIM(39, 0, 0, 4),
    ANIM(40, 0, 0, 3),
    ANIM(41, 0, 0, 4),
    ANIM_STOP(42, 0, 0, 6),
};

AnimWord anim_8013FC4C[5] = {
    ANIM(43, 0, 0, 12),
    ANIM_JUMP(44, 0, 0, 12, anim_8013FC4C),
};

AnimWord anim_8013FC60[4] = {
    ANIM(45, 0, 0, 16),
    ANIM_STOP(46, 0, 0, 16),
};

void *ptrs_8013FC70[1] = {
    D_8013FCD0,
};

void *ptrs_8013FC74[1] = {
    D_8013FCD8,
};

void *ptrs_8013FC78[1] = {
    D_8013FD1C,
};

void *ptrs_8013FC7C[2] = {
    D_8013FDC4,
    D_8013FDDC,
};

void *ptrs_8013FC84[1] = {
    D_8013FDE4,
};

void *ptrs_8013FC88[1] = {
    D_8013FE04,
};

void *ptrs_8013FC8C[1] = {
    D_8013FE28,
};

void *ptrs_8013FC90[1] = {
    D_8013FE40,
};

void *ptrs_8013FC94[3] = {
    D_8013FE48,
    D_8013FE50,
    D_8013FE58,
};
