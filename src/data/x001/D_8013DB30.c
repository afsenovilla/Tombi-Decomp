// DATA 8013db30 780 X001
// Data of X001 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8013DE5C[];
extern char D_8013DE80[];
extern char D_8013DEC4[];
extern char D_8013DEE4[];
extern char D_8013DF04[];
extern char D_8013DF0C[];
extern char D_8013DF30[];
extern char D_8013DF48[];
extern char D_8013DF7C[];
extern char D_8013DF84[];
extern char D_8013DFE4[];
extern char D_8013DFF4[];
extern char D_8013DFFC[];
extern char D_8013E074[];
extern char D_8013E07C[];
extern char D_8013E084[];
extern char D_8013E08C[];
extern char D_8013E0D0[];
extern char D_8013E0F0[];
extern char D_8013E0F8[];
extern char D_8013E118[];
extern char D_8013E138[];
extern char D_8013E158[];
extern char D_8013E168[];
extern char D_8013E188[];
extern char D_8013E190[];
extern char D_8013E198[];
extern char D_8013E1A0[];
extern char D_8013E1E4[];
extern char D_8013E204[];
extern char D_8013E20C[];
extern char D_8013E22C[];
extern char D_8013E250[];
extern char D_8013E270[];
extern char D_8013E278[];
extern char D_8013E280[];
extern char D_8013E288[];
extern char D_8013E290[];
extern char D_8013E2D4[];
extern char D_8013E2F4[];
extern char D_8013E30C[];
extern char D_8013E33C[];
extern char D_8013E35C[];
extern char D_8013E37C[];
extern char D_8013E3DC[];
extern char D_8013E3E4[];
extern char D_8013E3F8[];
extern char D_8013E418[];
extern char D_8013E420[];
extern char D_8013E428[];
extern char D_8013E458[];
extern char D_8013E460[];
extern char D_8013E468[];
extern char D_8013E470[];
extern char D_8013E494[];
extern char D_8013E4F4[];
extern char D_8013E4FC[];
extern char D_8013E504[];
extern char D_8013E50C[];
extern AnimWord anim_8013DB30[];
extern AnimWord anim_8013DB74[];
extern AnimWord anim_8013DB98[];
extern AnimWord anim_8013DBA8[];
extern AnimWord anim_8013DBB0[];
extern AnimWord anim_8013DBD0[];
extern AnimWord anim_8013DBD8[];
extern AnimWord anim_8013DBE0[];
extern AnimWord anim_8013DBE8[];
extern AnimWord anim_8013DBF0[];
extern AnimWord anim_8013DC34[];
extern AnimWord anim_8013DC5C[];
extern AnimWord anim_8013DC80[];
extern AnimWord anim_8013DC88[];
extern AnimWord anim_8013DCA0[];
extern AnimWord anim_8013DCC4[];
extern AnimWord anim_8013DCE8[];
extern AnimWord anim_8013DCF0[];
extern AnimWord anim_8013DD18[];
extern void *ptrs_8013DD50[];
extern void *ptrs_8013DDB0[];
extern void *ptrs_8013DDC8[];
extern void *ptrs_8013DDD8[];
extern void *ptrs_8013DDDC[];
extern void *ptrs_8013DDE4[];
extern void *ptrs_8013DDF0[];
extern void *ptrs_8013DDF4[];
extern void *ptrs_8013DDF8[];
extern void *ptrs_8013DE08[];
extern void *ptrs_8013DE0C[];
extern void *ptrs_8013DE2C[];
extern void *ptrs_8013DE34[];
extern void *ptrs_8013DE38[];

AnimWord anim_8013DB30[17] = {
    ANIM(10, 1, 0, 2),
    ANIM(11, 1, 0, 2),
    ANIM(12, 1, 0, 2),
    ANIM(13, 1, 0, 2),
    ANIM(14, 1, 0, 2),
    ANIM(15, 1, 0, 2),
    ANIM(16, 1, 0, 2),
    ANIM_JUMP(17, 1, 0, 2, anim_8013DB30),
};

AnimWord anim_8013DB74[9] = {
    ANIM(32, 3, 0, 10),
    ANIM(33, 3, 0, 4),
    ANIM(34, 2, 0, 10),
    ANIM_JUMP(33, 3, 0, 4, anim_8013DB74),
};

AnimWord anim_8013DB98[4] = {
    ANIM(46, 29, 0, 8),
    ANIM_STOP(47, 29, 0, 60),
};

AnimWord anim_8013DBA8[2] = {
    ANIM_STOP(82, 26, 0, 5),
};

AnimWord anim_8013DBB0[8] = {
    ANIM(89, 6, 0, 15),
    ANIM(90, 6, 0, 15),
    ANIM(91, 6, 0, 15),
    ANIM_STOP(92, 6, 0, 1),
};

AnimWord anim_8013DBD0[2] = {
    ANIM_STOP(93, 23, 0, 60),
};

AnimWord anim_8013DBD8[2] = {
    ANIM_STOP(94, 24, 0, 60),
};

AnimWord anim_8013DBE0[2] = {
    ANIM_STOP(95, 24, 0, 8),
};

AnimWord anim_8013DBE8[2] = {
    ANIM_STOP(96, 24, 0, 8),
};

AnimWord anim_8013DBF0[17] = {
    ANIM(99, 35, 0, 48),
    ANIM(100, 35, 0, 8),
    ANIM(101, 35, 0, 8),
    ANIM(100, 35, 0, 8),
    ANIM(99, 35, 0, 16),
    ANIM(100, 35, 0, 8),
    ANIM(101, 35, 0, 8),
    ANIM_JUMP(100, 35, 0, 8, anim_8013DBF0),
};

AnimWord anim_8013DC34[10] = {
    ANIM(124, 6, 0, 8),
    ANIM(125, 6, 0, 8),
    ANIM(126, 7, 0, 8),
    ANIM(127, 8, 0, 8),
    ANIM(128, 8, 0, 8),
};

AnimWord anim_8013DC5C[9] = {
    ANIM(129, 9, 0, 8),
    ANIM(130, 9, 0, 8),
    ANIM(129, 9, 0, 8),
    ANIM_JUMP(131, 9, 0, 8, anim_8013DC5C),
};

AnimWord anim_8013DC80[2] = {
    ANIM_STOP(134, 22, 0, 60),
};

AnimWord anim_8013DC88[6] = {
    ANIM(135, 30, 0, 6),
    ANIM(136, 30, 0, 6),
    ANIM_STOP(137, 31, 0, 30),
};

AnimWord anim_8013DCA0[9] = {
    ANIM(138, 32, 0, 12),
    ANIM(139, 33, 0, 12),
    ANIM(140, 33, 0, 12),
    ANIM_JUMP(139, 33, 0, 12, anim_8013DCA0),
};

AnimWord anim_8013DCC4[9] = {
    ANIM(143, 34, 0, 3),
    ANIM(144, 34, 0, 6),
    ANIM(143, 34, 0, 3),
    ANIM_JUMP(142, 34, 0, 6, anim_8013DCC4),
};

AnimWord anim_8013DCE8[2] = {
    ANIM_STOP(181, 39, 0, 60),
};

AnimWord anim_8013DCF0[10] = {
    ANIM(119, 0, 0, 6),
    ANIM(120, 0, 0, 8),
    ANIM(121, 0, 0, 5),
    ANIM(122, 0, 0, 5),
    ANIM_STOP(123, 0, 0, 5),
};

AnimWord anim_8013DD18[14] = {
    ANIM(149, 0, 0, 4),
    ANIM(150, 0, 0, 4),
    ANIM(151, 0, 0, 4),
    ANIM(152, 0, 0, 4),
    ANIM(153, 0, 0, 4),
    ANIM(154, 0, 0, 4),
    ANIM_STOP(155, 0, 0, 4),
};

void *ptrs_8013DD50[24] = {
    D_8013E074,
    D_8013E07C,
    D_8013E084,
    D_8013E08C,
    D_8013E0D0,
    D_8013E0F0,
    D_8013E0F8,
    D_8013E118,
    D_8013E138,
    D_8013E158,
    D_8013E168,
    D_8013E188,
    D_8013E190,
    D_8013E198,
    D_8013E1A0,
    D_8013E1E4,
    D_8013E204,
    D_8013E20C,
    D_8013E22C,
    D_8013E250,
    D_8013E270,
    D_8013E278,
    D_8013E280,
    D_8013E288,
};

void *ptrs_8013DDB0[6] = {
    D_8013E290,
    D_8013E2D4,
    D_8013E2F4,
    D_8013E30C,
    D_8013E33C,
    D_8013E35C,
};

void *ptrs_8013DDC8[4] = {
    D_8013E37C,
    D_8013E3DC,
    D_8013E3E4,
    D_8013E3F8,
};

void *ptrs_8013DDD8[1] = {
    D_8013E418,
};

void *ptrs_8013DDDC[2] = {
    D_8013E420,
    D_8013E428,
};

void *ptrs_8013DDE4[3] = {
    D_8013E458,
    D_8013E460,
    D_8013E468,
};

void *ptrs_8013DDF0[1] = {
    D_8013E470,
};

void *ptrs_8013DDF4[1] = {
    D_8013E494,
};

void *ptrs_8013DDF8[4] = {
    D_8013E4F4,
    D_8013E4FC,
    D_8013E504,
    D_8013E50C,
};

void *ptrs_8013DE08[1] = {
    D_8013DE5C,
};

void *ptrs_8013DE0C[8] = {
    D_8013DE80,
    D_8013DEC4,
    D_8013DEE4,
    D_8013DF04,
    D_8013DF0C,
    D_8013DF30,
    D_8013DF48,
    D_8013DF7C,
};

void *ptrs_8013DE2C[2] = {
    D_8013DF84,
    D_8013DFE4,
};

void *ptrs_8013DE34[1] = {
    D_8013DFF4,
};

void *ptrs_8013DE38[1] = {
    D_8013DFFC,
};
