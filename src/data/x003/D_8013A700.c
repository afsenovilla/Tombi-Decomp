// DATA 8013a700 880 X003
// Data of X003 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void *ptrs_8013A700[];
extern void *ptrs_8013A704[];
extern void *ptrs_8013A708[];
extern void *ptrs_8013A70C[];
extern AnimWord anim_8013A718[];
extern AnimWord anim_8013A72C[];
extern AnimWord anim_8013A740[];
extern AnimWord anim_8013A754[];
extern AnimWord anim_8013A774[];
extern AnimWord anim_8013A7C8[];
extern AnimWord anim_8013A82C[];
extern AnimWord anim_8013A844[];
extern AnimWord anim_8013A84C[];
extern AnimWord anim_8013A860[];
extern AnimWord anim_8013A884[];
extern AnimWord anim_8013A89C[];
extern AnimWord anim_8013A8A4[];
extern AnimWord anim_8013A8E4[];
extern AnimWord anim_8013A8F4[];
extern AnimWord anim_8013A904[];
extern AnimWord anim_8013A914[];
extern AnimWord anim_8013A924[];
extern AnimWord anim_8013A934[];
extern AnimWord anim_8013A93C[];
extern AnimWord anim_8013A944[];
extern AnimWord anim_8013A94C[];
extern AnimWord anim_8013A954[];
extern AnimWord anim_8013A95C[];
extern AnimWord anim_8013A964[];
extern AnimWord anim_8013A96C[];
extern AnimWord anim_8013A9A4[];
extern AnimWord anim_8013A9B8[];
extern AnimWord anim_8013A9F8[];
extern AnimWord anim_8013AA2C[];
extern AnimWord anim_8013AA34[];
extern AnimWord anim_8013AA3C[];

void *ptrs_8013A700[1] = {
    anim_8013A9A4,
};

void *ptrs_8013A704[1] = {
    anim_8013A9B8,
};

void *ptrs_8013A708[1] = {
    anim_8013A9F8,
};

void *ptrs_8013A70C[3] = {
    anim_8013AA2C,
    anim_8013AA34,
    anim_8013AA3C,
};

AnimWord anim_8013A718[5] = {
    ANIM(0, 0, 0, 12),
    ANIM_JUMP(1, 0, 0, 12, anim_8013A718),
};

AnimWord anim_8013A72C[5] = {
    ANIM(2, 0, 0, 12),
    ANIM_JUMP(3, 0, 0, 12, anim_8013A72C),
};

AnimWord anim_8013A740[5] = {
    ANIM(4, 0, 0, 12),
    ANIM_JUMP(5, 0, 0, 12, anim_8013A740),
};

AnimWord anim_8013A754[8] = {
    ANIM(6, 0, 0, 12),
    ANIM(7, 0, 0, 12),
    ANIM(6, 0, 0, 12),
    ANIM_STOP(7, 0, 0, 12),
};

AnimWord anim_8013A774[21] = {
    ANIM(8, 0, 0, 10),
    ANIM(9, 0, 0, 10),
    ANIM(8, 0, 0, 10),
    ANIM(9, 0, 0, 5),
    ANIM(10, 0, 0, 3),
    ANIM(11, 0, 0, 10),
    ANIM(12, 0, 0, 10),
    ANIM(11, 0, 0, 10),
    ANIM(12, 0, 0, 5),
    ANIM_JUMP_STOP(10, 0, 0, 3, anim_8013A774),
};

AnimWord anim_8013A7C8[25] = {
    ANIM(6, 0, 0, 10),
    ANIM(7, 0, 0, 10),
    ANIM(13, 0, 0, 10),
    ANIM(14, 0, 0, 10),
    ANIM(13, 0, 0, 10),
    ANIM(14, 0, 0, 10),
    ANIM(6, 0, 0, 10),
    ANIM(7, 0, 0, 10),
    ANIM(15, 0, 0, 10),
    ANIM(16, 0, 0, 10),
    ANIM(15, 0, 0, 10),
    ANIM_JUMP_STOP(16, 0, 0, 10, anim_8013A7C8),
};

AnimWord anim_8013A82C[6] = {
    ANIM(17, 0, 0, 6),
    ANIM(18, 0, 0, 6),
    ANIM_STOP(19, 0, 0, 6),
};

AnimWord anim_8013A844[2] = {
    ANIM_STOP(20, 0, 0, 60),
};

AnimWord anim_8013A84C[5] = {
    ANIM(21, 0, 0, 12),
    ANIM_JUMP(22, 0, 0, 12, anim_8013A84C),
};

AnimWord anim_8013A860[9] = {
    ANIM(23, 0, 0, 5),
    ANIM(24, 0, 0, 5),
    ANIM(23, 0, 0, 5),
    ANIM_JUMP(25, 0, 0, 5, anim_8013A860),
};

AnimWord anim_8013A884[6] = {
    ANIM(26, 0, 0, 6),
    ANIM(27, 0, 0, 6),
    ANIM_STOP(28, 0, 0, 60),
};

AnimWord anim_8013A89C[2] = {
    ANIM_STOP(29, 0, 0, 60),
};

AnimWord anim_8013A8A4[16] = {
    ANIM(71, 0, 0, 6),
    ANIM(72, 0, 0, 6),
    ANIM(73, 0, 0, 4),
    ANIM(74, 0, 0, 4),
    ANIM(73, 0, 0, 4),
    ANIM(74, 0, 0, 4),
    ANIM(73, 0, 0, 4),
    ANIM_STOP(74, 0, 0, 4),
};

AnimWord anim_8013A8E4[4] = {
    ANIM(30, 0, 0, 5),
    ANIM_STOP(31, 0, 0, 5),
};

AnimWord anim_8013A8F4[4] = {
    ANIM(32, 0, 0, 5),
    ANIM_STOP(33, 0, 0, 5),
};

AnimWord anim_8013A904[4] = {
    ANIM(34, 0, 0, 5),
    ANIM_STOP(35, 0, 0, 5),
};

AnimWord anim_8013A914[4] = {
    ANIM(36, 0, 0, 5),
    ANIM_STOP(37, 0, 0, 5),
};

AnimWord anim_8013A924[4] = {
    ANIM(38, 0, 0, 5),
    ANIM_STOP(39, 0, 0, 5),
};

AnimWord anim_8013A934[2] = {
    ANIM(57, 0, 0, 2),
};

AnimWord anim_8013A93C[2] = {
    ANIM(58, 0, 0, 2),
};

AnimWord anim_8013A944[2] = {
    ANIM(59, 0, 0, 2),
};

AnimWord anim_8013A94C[2] = {
    ANIM(60, 0, 0, 2),
};

AnimWord anim_8013A954[2] = {
    ANIM(61, 0, 0, 2),
};

AnimWord anim_8013A95C[2] = {
    ANIM(62, 0, 0, 2),
};

AnimWord anim_8013A964[2] = {
    ANIM_STOP(63, 0, 0, 2),
};

AnimWord anim_8013A96C[14] = {
    ANIM(64, 0, 0, 2),
    ANIM(65, 0, 0, 2),
    ANIM(66, 0, 0, 2),
    ANIM(67, 0, 0, 2),
    ANIM(68, 0, 0, 2),
    ANIM(69, 0, 0, 2),
    ANIM_STOP(70, 0, 0, 2),
};

AnimWord anim_8013A9A4[5] = {
    ANIM(40, 0, 0, 12),
    ANIM_JUMP(41, 0, 0, 12, anim_8013A9A4),
};

AnimWord anim_8013A9B8[16] = {
    ANIM(49, 0, 0, 7),
    ANIM(50, 0, 0, 8),
    ANIM(51, 0, 0, 9),
    ANIM(52, 0, 0, 10),
    ANIM(53, 0, 0, 8),
    ANIM(54, 0, 0, 8),
    ANIM(55, 0, 0, 8),
    ANIM_STOP(56, 0, 0, 8),
};

AnimWord anim_8013A9F8[13] = {
    ANIM(42, 0, 0, 4),
    ANIM(43, 0, 0, 4),
    ANIM(44, 0, 0, 4),
    ANIM(45, 0, 0, 4),
    ANIM(46, 0, 0, 4),
    ANIM_JUMP(47, 0, 0, 4, anim_8013A9F8),
};

AnimWord anim_8013AA2C[2] = {
    ANIM_STOP(46, 0, 0, 60),
};

AnimWord anim_8013AA34[2] = {
    ANIM_STOP(48, 0, 0, 60),
};

AnimWord anim_8013AA3C[13] = {
    ANIM(42, 0, 0, 4),
    ANIM(43, 0, 0, 4),
    ANIM(44, 0, 0, 4),
    ANIM(45, 0, 0, 4),
    ANIM(46, 0, 0, 4),
    ANIM_JUMP(47, 0, 0, 4, anim_8013AA3C),
};
