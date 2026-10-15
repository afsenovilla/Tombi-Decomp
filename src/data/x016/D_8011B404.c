// DATA 8011b404 808 X016
// Data of X016 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void *ptrs_8011B404[];
extern void *ptrs_8011B410[];
extern void *ptrs_8011B41C[];
extern void *ptrs_8011B434[];
extern void *ptrs_8011B448[];
extern void *ptrs_8011B458[];
extern AnimWord anim_8011B468[];
extern AnimWord anim_8011B470[];
extern AnimWord anim_8011B478[];
extern AnimWord anim_8011B480[];
extern AnimWord anim_8011B488[];
extern AnimWord anim_8011B490[];
extern AnimWord anim_8011B498[];
extern AnimWord anim_8011B4A0[];
extern AnimWord anim_8011B4A8[];
extern AnimWord anim_8011B4B0[];
extern AnimWord anim_8011B4B8[];
extern AnimWord anim_8011B4EC[];
extern AnimWord anim_8011B518[];
extern AnimWord anim_8011B520[];
extern AnimWord anim_8011B538[];
extern AnimWord anim_8011B54C[];
extern AnimWord anim_8011B554[];
extern AnimWord anim_8011B584[];
extern AnimWord anim_8011B5AC[];
extern AnimWord anim_8011B5BC[];
extern AnimWord anim_8011B5D4[];
extern AnimWord anim_8011B5DC[];
extern AnimWord anim_8011B60C[];
extern AnimWord anim_8011B634[];
extern AnimWord anim_8011B684[];
extern AnimWord anim_8011B68C[];
extern AnimWord anim_8011B6BC[];
extern AnimWord anim_8011B6E4[];

void *ptrs_8011B404[3] = {
    anim_8011B488,
    anim_8011B490,
    anim_8011B4A8,
};

void *ptrs_8011B410[3] = {
    anim_8011B498,
    anim_8011B4A0,
    anim_8011B4A8,
};

void *ptrs_8011B41C[6] = {
    anim_8011B4B0,
    anim_8011B4B8,
    anim_8011B4EC,
    anim_8011B518,
    anim_8011B520,
    anim_8011B538,
};

void *ptrs_8011B434[5] = {
    anim_8011B54C,
    anim_8011B554,
    anim_8011B584,
    anim_8011B5AC,
    anim_8011B5BC,
};

void *ptrs_8011B448[4] = {
    anim_8011B5D4,
    anim_8011B5DC,
    anim_8011B60C,
    anim_8011B634,
};

void *ptrs_8011B458[4] = {
    anim_8011B684,
    anim_8011B68C,
    anim_8011B6BC,
    anim_8011B6E4,
};

AnimWord anim_8011B468[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8011B470[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8011B478[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8011B480[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8011B488[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8011B490[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_8011B498[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_8011B4A0[2] = {
    ANIM_STOP(7, 0, 0, 60),
};

AnimWord anim_8011B4A8[2] = {
    ANIM_STOP(8, 0, 0, 60),
};

AnimWord anim_8011B4B0[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_8011B4B8[13] = {
    ANIM(10, 0, 0, 8),
    ANIM(11, 0, 0, 8),
    ANIM(12, 0, 0, 8),
    ANIM(13, 0, 0, 8),
    ANIM(14, 0, 0, 8),
    ANIM_JUMP(15, 0, 0, 8, anim_8011B4B8),
};

AnimWord anim_8011B4EC[11] = {
    ANIM(16, 0, 0, 8),
    ANIM(17, 0, 0, 8),
    ANIM(18, 0, 0, 8),
    ANIM(17, 0, 0, 8),
    ANIM_JUMP(18, 0, 0, 8, anim_8011B4EC),
};

AnimWord anim_8011B518[2] = {
    ANIM_STOP(19, 0, 0, 60),
};

AnimWord anim_8011B520[6] = {
    ANIM(20, 0, 0, 5),
    ANIM(21, 0, 0, 3),
    ANIM_STOP(22, 0, 0, 60),
};

AnimWord anim_8011B538[5] = {
    ANIM(20, 0, 0, 5),
    ANIM_JUMP(21, 0, 0, 3, anim_8011B538),
};

AnimWord anim_8011B54C[2] = {
    ANIM_STOP(23, 0, 0, 60),
};

AnimWord anim_8011B554[12] = {
    ANIM(24, 0, 0, 8),
    ANIM(25, 0, 0, 8),
    ANIM(26, 0, 0, 8),
    ANIM(27, 0, 0, 8),
    ANIM(28, 0, 0, 8),
    ANIM_STOP(29, 0, 0, 8),
};

AnimWord anim_8011B584[10] = {
    ANIM(30, 0, 0, 8),
    ANIM(31, 0, 0, 8),
    ANIM(32, 0, 0, 8),
    ANIM(31, 0, 0, 8),
    ANIM_STOP(32, 0, 0, 8),
};

AnimWord anim_8011B5AC[4] = {
    ANIM(33, 0, 0, 4),
    ANIM_STOP(34, 0, 0, 60),
};

AnimWord anim_8011B5BC[6] = {
    ANIM(35, 0, 0, 5),
    ANIM(36, 0, 0, 3),
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_8011B5D4[2] = {
    ANIM_STOP(38, 0, 0, 60),
};

AnimWord anim_8011B5DC[12] = {
    ANIM(39, 0, 0, 8),
    ANIM(40, 0, 0, 8),
    ANIM(41, 0, 0, 8),
    ANIM(42, 0, 0, 8),
    ANIM(44, 0, 0, 8),
    ANIM_STOP(43, 0, 0, 8),
};

AnimWord anim_8011B60C[10] = {
    ANIM(45, 0, 0, 8),
    ANIM(46, 0, 0, 16),
    ANIM(47, 0, 0, 16),
    ANIM(46, 0, 0, 16),
    ANIM_STOP(47, 0, 0, 16),
};

AnimWord anim_8011B634[20] = {
    ANIM(42, 0, 0, 16),
    ANIM(49, 0, 0, 4),
    ANIM(50, 0, 0, 10),
    ANIM(51, 0, 0, 4),
    ANIM(52, 0, 0, 12),
    ANIM(53, 0, 0, 5),
    ANIM(54, 0, 0, 5),
    ANIM(55, 0, 0, 5),
    ANIM(56, 0, 0, 5),
    ANIM_STOP(52, 0, 0, 16),
};

AnimWord anim_8011B684[2] = {
    ANIM_STOP(57, 0, 0, 60),
};

AnimWord anim_8011B68C[12] = {
    ANIM(58, 0, 0, 8),
    ANIM(59, 0, 0, 8),
    ANIM(60, 0, 0, 8),
    ANIM(61, 0, 0, 8),
    ANIM(62, 0, 0, 8),
    ANIM_STOP(63, 0, 0, 8),
};

AnimWord anim_8011B6BC[10] = {
    ANIM(64, 0, 0, 8),
    ANIM(65, 0, 0, 16),
    ANIM(66, 0, 0, 16),
    ANIM(65, 0, 0, 16),
    ANIM_STOP(66, 0, 0, 16),
};

AnimWord anim_8011B6E4[18] = {
    ANIM(67, 0, 0, 4),
    ANIM(68, 0, 0, 8),
    ANIM(69, 0, 0, 4),
    ANIM(70, 0, 0, 16),
    ANIM(71, 0, 0, 5),
    ANIM(72, 0, 0, 5),
    ANIM(73, 0, 0, 5),
    ANIM(74, 0, 0, 5),
    ANIM_STOP(70, 0, 0, 5),
};
