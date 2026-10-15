// DATA 8012dba4 1120 X009
// Data of X009 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern char D_8012E748[];
extern char D_8012E750[];
extern char D_8012E758[];
extern char D_8012E760[];
extern char D_8012E768[];
extern char D_8012E770[];
extern char D_8012E778[];
extern char D_8012E7F4[];
extern char D_8012E7FC[];
extern char D_8012E804[];
extern char D_8012E80C[];
extern char D_8012E814[];
extern char D_8012E81C[];
extern char D_8012E824[];
extern char D_8012E82C[];
extern char D_8012E94C[];
extern char D_8012E954[];
extern char D_8012E95C[];
extern char D_8012E964[];
extern char D_8012E96C[];
extern char D_8012E974[];
extern char D_8012E97C[];
extern char D_8012E984[];
extern char D_8012E98C[];
extern char D_8012E994[];
extern char D_8012E99C[];
extern char D_8012E9A4[];
extern char D_8012E9AC[];
extern char D_8012E9B4[];
extern AnimWord anim_8012DBA4[];
extern AnimWord anim_8012DBAC[];
extern AnimWord anim_8012DBB4[];
extern AnimWord anim_8012DC28[];
extern AnimWord anim_8012DC9C[];
extern AnimWord anim_8012DD10[];
extern AnimWord anim_8012DD84[];
extern AnimWord anim_8012DD8C[];
extern AnimWord anim_8012DD94[];
extern AnimWord anim_8012DD9C[];
extern AnimWord anim_8012DDA4[];
extern AnimWord anim_8012DDAC[];
extern AnimWord anim_8012DDB4[];
extern AnimWord anim_8012DDBC[];
extern AnimWord anim_8012DDC4[];
extern AnimWord anim_8012DDCC[];
extern AnimWord anim_8012DDD4[];
extern AnimWord anim_8012DDDC[];
extern AnimWord anim_8012DDE4[];
extern AnimWord anim_8012DDEC[];
extern AnimWord anim_8012DDF4[];
extern AnimWord anim_8012DDFC[];
extern AnimWord anim_8012DE04[];
extern AnimWord anim_8012DE0C[];
extern AnimWord anim_8012DE14[];
extern AnimWord anim_8012DE1C[];
extern AnimWord anim_8012DE24[];
extern AnimWord anim_8012DE2C[];
extern AnimWord anim_8012DE34[];
extern AnimWord anim_8012DE3C[];
extern AnimWord anim_8012DE44[];
extern AnimWord anim_8012DE4C[];
extern AnimWord anim_8012DE54[];
extern AnimWord anim_8012DE5C[];
extern AnimWord anim_8012DE80[];
extern AnimWord anim_8012DEC4[];
extern AnimWord anim_8012DEDC[];
extern AnimWord anim_8012DF08[];
extern AnimWord anim_8012DF34[];
extern AnimWord anim_8012DF60[];
extern AnimWord anim_8012DF68[];
extern AnimWord anim_8012DF70[];
extern AnimWord anim_8012DF78[];
extern AnimWord anim_8012DF80[];
extern AnimWord anim_8012DF88[];
extern void *ptrs_8012DF90[];
extern void *ptrs_8012DFAC[];
extern void *ptrs_8012DFC8[];
extern void *ptrs_8012DFE4[];
extern void *ptrs_8012E000[];

AnimWord anim_8012DBA4[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8012DBAC[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8012DBB4[29] = {
    ANIM(3, 0, 0, 14),
    ANIM(3, 0, 0, 15),
    ANIM(4, 0, 0, 16),
    ANIM(5, 0, 0, 17),
    ANIM(6, 0, 0, 19),
    ANIM(7, 0, 0, 22),
    ANIM(8, 0, 0, 25),
    ANIM(9, 0, 0, 28),
    ANIM(8, 0, 0, 10),
    ANIM(7, 0, 0, 10),
    ANIM(6, 0, 0, 10),
    ANIM(5, 0, 0, 10),
    ANIM(4, 0, 0, 10),
    ANIM_JUMP(3, 0, 0, 20, anim_8012DBB4),
};

AnimWord anim_8012DC28[29] = {
    ANIM(11, 0, 0, 14),
    ANIM(11, 0, 0, 15),
    ANIM(12, 0, 0, 16),
    ANIM(13, 0, 0, 17),
    ANIM(14, 0, 0, 19),
    ANIM(15, 0, 0, 22),
    ANIM(16, 0, 0, 25),
    ANIM(17, 0, 0, 28),
    ANIM(16, 0, 0, 10),
    ANIM(15, 0, 0, 10),
    ANIM(14, 0, 0, 10),
    ANIM(13, 0, 0, 10),
    ANIM(12, 0, 0, 10),
    ANIM_JUMP(11, 0, 0, 20, anim_8012DC28),
};

AnimWord anim_8012DC9C[29] = {
    ANIM(19, 0, 0, 14),
    ANIM(19, 0, 0, 15),
    ANIM(20, 0, 0, 16),
    ANIM(21, 0, 0, 17),
    ANIM(22, 0, 0, 19),
    ANIM(23, 0, 0, 22),
    ANIM(24, 0, 0, 25),
    ANIM(25, 0, 0, 28),
    ANIM(24, 0, 0, 10),
    ANIM(23, 0, 0, 10),
    ANIM(22, 0, 0, 10),
    ANIM(21, 0, 0, 10),
    ANIM(20, 0, 0, 10),
    ANIM_JUMP(19, 0, 0, 20, anim_8012DC9C),
};

AnimWord anim_8012DD10[29] = {
    ANIM(27, 0, 0, 14),
    ANIM(27, 0, 0, 15),
    ANIM(27, 0, 0, 16),
    ANIM(29, 0, 0, 17),
    ANIM(30, 0, 0, 19),
    ANIM(31, 0, 0, 22),
    ANIM(32, 0, 0, 25),
    ANIM(33, 0, 0, 28),
    ANIM(32, 0, 0, 10),
    ANIM(31, 0, 0, 10),
    ANIM(30, 0, 0, 10),
    ANIM(29, 0, 0, 10),
    ANIM(28, 0, 0, 10),
    ANIM_JUMP(27, 0, 0, 20, anim_8012DD10),
};

AnimWord anim_8012DD84[2] = {
    ANIM_STOP(40, 0, 0, 60),
};

AnimWord anim_8012DD8C[2] = {
    ANIM_STOP(41, 0, 0, 60),
};

AnimWord anim_8012DD94[2] = {
    ANIM_STOP(42, 0, 0, 60),
};

AnimWord anim_8012DD9C[2] = {
    ANIM_STOP(43, 0, 0, 60),
};

AnimWord anim_8012DDA4[2] = {
    ANIM_STOP(44, 0, 0, 60),
};

AnimWord anim_8012DDAC[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8012DDB4[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8012DDBC[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8012DDC4[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8012DDCC[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8012DDD4[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_8012DDDC[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_8012DDE4[2] = {
    ANIM_STOP(7, 0, 0, 60),
};

AnimWord anim_8012DDEC[2] = {
    ANIM_STOP(8, 0, 0, 60),
};

AnimWord anim_8012DDF4[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_8012DDFC[2] = {
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_8012DE04[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8012DE0C[2] = {
    ANIM_STOP(12, 0, 0, 60),
};

AnimWord anim_8012DE14[2] = {
    ANIM_STOP(13, 0, 0, 60),
};

AnimWord anim_8012DE1C[2] = {
    ANIM_STOP(14, 0, 0, 60),
};

AnimWord anim_8012DE24[2] = {
    ANIM_STOP(15, 0, 0, 60),
};

AnimWord anim_8012DE2C[2] = {
    ANIM_STOP(20, 0, 0, 60),
};

AnimWord anim_8012DE34[2] = {
    ANIM_STOP(21, 0, 0, 60),
};

AnimWord anim_8012DE3C[2] = {
    ANIM_STOP(22, 0, 0, 60),
};

AnimWord anim_8012DE44[2] = {
    ANIM_STOP(39, 0, 0, 16),
};

AnimWord anim_8012DE4C[2] = {
    ANIM_STOP(40, 0, 0, 60),
};

AnimWord anim_8012DE54[2] = {
    ANIM_STOP(41, 0, 0, 60),
};

AnimWord anim_8012DE5C[9] = {
    ANIM(16, 0, 0, 60),
    ANIM(17, 0, 0, 10),
    ANIM(18, 0, 0, 10),
    ANIM_JUMP(19, 0, 0, 60, anim_8012DE5C),
};

AnimWord anim_8012DE80[17] = {
    ANIM(23, 0, 0, 8),
    ANIM(24, 0, 0, 8),
    ANIM(25, 0, 0, 8),
    ANIM(26, 0, 0, 8),
    ANIM(27, 0, 0, 8),
    ANIM(28, 0, 0, 8),
    ANIM(29, 0, 0, 8),
    ANIM_JUMP(30, 0, 0, 8, anim_8012DE80),
};

AnimWord anim_8012DEC4[6] = {
    ANIM(31, 0, 0, 16),
    ANIM(32, 0, 0, 16),
    ANIM(33, 0, 0, 16),
};

AnimWord anim_8012DEDC[11] = {
    ANIM(34, 0, 0, 16),
    ANIM(35, 0, 0, 16),
    ANIM(36, 0, 0, 16),
    ANIM(37, 0, 0, 16),
    ANIM_JUMP(38, 0, 0, 16, anim_8012DEC4),
};

AnimWord anim_8012DF08[11] = {
    ANIM(42, 0, 0, 4),
    ANIM(43, 0, 0, 4),
    ANIM(44, 0, 0, 4),
    ANIM(45, 0, 0, 4),
    ANIM_JUMP(46, 0, 0, 4, anim_8012DF08),
};

AnimWord anim_8012DF34[11] = {
    ANIM(47, 0, 0, 4),
    ANIM(48, 0, 0, 4),
    ANIM(49, 0, 0, 4),
    ANIM(50, 0, 0, 4),
    ANIM_JUMP(51, 0, 0, 4, anim_8012DF34),
};

AnimWord anim_8012DF60[2] = {
    ANIM_STOP(52, 0, 0, 60),
};

AnimWord anim_8012DF68[2] = {
    ANIM_STOP(53, 0, 0, 60),
};

AnimWord anim_8012DF70[2] = {
    ANIM_STOP(54, 0, 0, 60),
};

AnimWord anim_8012DF78[2] = {
    ANIM_STOP(55, 0, 0, 60),
};

AnimWord anim_8012DF80[2] = {
    ANIM_STOP(56, 0, 0, 60),
};

AnimWord anim_8012DF88[2] = {
    ANIM_STOP(57, 0, 0, 60),
};

void *ptrs_8012DF90[7] = {
    D_8012E748,
    D_8012E750,
    D_8012E758,
    D_8012E760,
    D_8012E768,
    D_8012E770,
    D_8012E778,
};

void *ptrs_8012DFAC[7] = {
    D_8012E7F4,
    D_8012E7FC,
    D_8012E804,
    D_8012E80C,
    D_8012E814,
    D_8012E81C,
    D_8012E824,
};

void *ptrs_8012DFC8[7] = {
    D_8012E94C,
    D_8012E954,
    D_8012E95C,
    D_8012E964,
    D_8012E96C,
    D_8012E974,
    D_8012E97C,
};

void *ptrs_8012DFE4[7] = {
    D_8012E984,
    D_8012E98C,
    D_8012E994,
    D_8012E99C,
    D_8012E9A4,
    D_8012E9AC,
    D_8012E9B4,
};

void *ptrs_8012E000[1] = {
    D_8012E82C,
};
