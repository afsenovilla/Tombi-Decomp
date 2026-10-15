// DATA 8011cacc 648 X018
// Data of X018 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_8011CACC[];
extern AnimWord anim_8011CAD4[];
extern AnimWord anim_8011CB08[];
extern AnimWord anim_8011CB34[];
extern AnimWord anim_8011CB3C[];
extern AnimWord anim_8011CB54[];
extern AnimWord anim_8011CB68[];
extern AnimWord anim_8011CB70[];
extern AnimWord anim_8011CBA0[];
extern AnimWord anim_8011CBC8[];
extern AnimWord anim_8011CBD8[];
extern AnimWord anim_8011CBF0[];
extern AnimWord anim_8011CC10[];
extern AnimWord anim_8011CC18[];
extern AnimWord anim_8011CC4C[];
extern AnimWord anim_8011CC54[];
extern AnimWord anim_8011CC68[];
extern AnimWord anim_8011CC98[];
extern AnimWord anim_8011CCA0[];
extern AnimWord anim_8011CCB0[];
extern AnimWord anim_8011CCD8[];
extern AnimWord anim_8011CCE0[];
extern AnimWord anim_8011CCFC[];
extern AnimWord anim_8011CD34[];
extern AnimWord anim_8011CD3C[];
extern AnimWord anim_8011CD44[];
extern AnimWord anim_8011CD4C[];

AnimWord anim_8011CACC[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_8011CAD4[13] = {
    ANIM(10, 0, 0, 8),
    ANIM(11, 0, 0, 8),
    ANIM(12, 0, 0, 8),
    ANIM(13, 0, 0, 8),
    ANIM(14, 0, 0, 8),
    ANIM_JUMP(15, 0, 0, 8, anim_8011CAD4),
};

AnimWord anim_8011CB08[11] = {
    ANIM(16, 0, 0, 8),
    ANIM(17, 0, 0, 8),
    ANIM(18, 0, 0, 8),
    ANIM(17, 0, 0, 8),
    ANIM_JUMP(18, 0, 0, 8, anim_8011CB08),
};

AnimWord anim_8011CB34[2] = {
    ANIM_STOP(19, 0, 0, 60),
};

AnimWord anim_8011CB3C[6] = {
    ANIM(20, 0, 0, 5),
    ANIM(21, 0, 0, 3),
    ANIM_STOP(22, 0, 0, 60),
};

AnimWord anim_8011CB54[5] = {
    ANIM(20, 0, 0, 5),
    ANIM_JUMP(21, 0, 0, 3, anim_8011CB54),
};

AnimWord anim_8011CB68[2] = {
    ANIM_STOP(23, 0, 0, 60),
};

AnimWord anim_8011CB70[12] = {
    ANIM(24, 0, 0, 8),
    ANIM(25, 0, 0, 8),
    ANIM(26, 0, 0, 8),
    ANIM(27, 0, 0, 8),
    ANIM(28, 0, 0, 8),
    ANIM_STOP(29, 0, 0, 8),
};

AnimWord anim_8011CBA0[10] = {
    ANIM(30, 0, 0, 8),
    ANIM(31, 0, 0, 8),
    ANIM(32, 0, 0, 8),
    ANIM(31, 0, 0, 8),
    ANIM_STOP(32, 0, 0, 8),
};

AnimWord anim_8011CBC8[4] = {
    ANIM(33, 0, 0, 4),
    ANIM_STOP(34, 0, 0, 60),
};

AnimWord anim_8011CBD8[6] = {
    ANIM(35, 0, 0, 5),
    ANIM(36, 0, 0, 3),
    ANIM_STOP(37, 0, 0, 60),
};

AnimWord anim_8011CBF0[8] = {
    ANIM(7, 0, 0, 5),
    ANIM(8, 0, 0, 5),
    ANIM(18, 0, 0, 12),
    ANIM_STOP(19, 0, 0, 60),
};

AnimWord anim_8011CC10[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8011CC18[13] = {
    ANIM(1, 0, 0, 8),
    ANIM(2, 0, 0, 8),
    ANIM(3, 0, 0, 8),
    ANIM(4, 0, 0, 8),
    ANIM(5, 0, 0, 8),
    ANIM_JUMP(6, 0, 0, 8, anim_8011CC18),
};

AnimWord anim_8011CC4C[2] = {
    ANIM_STOP(12, 0, 0, 4),
};

AnimWord anim_8011CC54[5] = {
    ANIM(13, 0, 0, 12),
    ANIM_JUMP(14, 0, 0, 12, anim_8011CC54),
};

AnimWord anim_8011CC68[12] = {
    ANIM(0, 0, 0, 24),
    ANIM(7, 0, 0, 4),
    ANIM(8, 0, 0, 4),
    ANIM(9, 0, 0, 4),
    ANIM(10, 0, 0, 16),
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_8011CC98[2] = {
    ANIM_STOP(15, 0, 0, 4),
};

AnimWord anim_8011CCA0[4] = {
    ANIM(16, 0, 0, 8),
    ANIM_STOP(17, 0, 0, 8),
};

AnimWord anim_8011CCB0[10] = {
    ANIM(20, 0, 0, 8),
    ANIM(21, 0, 0, 6),
    ANIM(22, 0, 0, 8),
    ANIM(21, 0, 0, 2),
    ANIM_STOP(23, 0, 0, 4),
};

AnimWord anim_8011CCD8[2] = {
    ANIM_STOP(24, 0, 0, 60),
};

AnimWord anim_8011CCE0[7] = {
    ANIM(25, 0, 0, 10),
    ANIM(26, 0, 0, 10),
    ANIM_JUMP(27, 0, 0, 10, anim_8011CCE0),
};

AnimWord anim_8011CCFC[14] = {
    ANIM(57, 0, 0, 5),
    ANIM(58, 0, 0, 5),
    ANIM(57, 0, 0, 5),
    ANIM(58, 0, 0, 5),
    ANIM(59, 0, 0, 5),
    ANIM(60, 0, 0, 5),
    ANIM_STOP(61, 0, 0, 5),
};

AnimWord anim_8011CD34[2] = {
    ANIM_STOP(30, 0, 0, 60),
};

AnimWord anim_8011CD3C[2] = {
    ANIM_STOP(28, 0, 0, 60),
};

AnimWord anim_8011CD44[2] = {
    ANIM_STOP(52, 0, 0, 60),
};

AnimWord anim_8011CD4C[2] = {
    ANIM_STOP(53, 0, 0, 60),
};
