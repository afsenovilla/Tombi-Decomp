// DATA 8011a470 720 X005
// Data of X005 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_8011A470[];
extern AnimWord anim_8011A478[];
extern AnimWord anim_8011A480[];
extern AnimWord anim_8011A488[];
extern AnimWord anim_8011A490[];
extern AnimWord anim_8011A498[];
extern AnimWord anim_8011A4A0[];
extern AnimWord anim_8011A4A8[];
extern AnimWord anim_8011A4B0[];
extern AnimWord anim_8011A4F0[];
extern AnimWord anim_8011A508[];
extern AnimWord anim_8011A52C[];
extern AnimWord anim_8011A550[];
extern AnimWord anim_8011A570[];
extern AnimWord anim_8011A578[];
extern AnimWord anim_8011A5E8[];
extern AnimWord anim_8011A5F8[];
extern void *ptrs_8011A608[];
extern void *ptrs_8011A63C[];
extern AnimWord anim_8011A658[];
extern AnimWord anim_8011A660[];
extern AnimWord anim_8011A668[];
extern AnimWord anim_8011A670[];
extern AnimWord anim_8011A678[];
extern AnimWord anim_8011A680[];
extern AnimWord anim_8011A688[];
extern AnimWord anim_8011A690[];
extern AnimWord anim_8011A6C8[];
extern AnimWord anim_8011A6E8[];
extern AnimWord anim_8011A6F0[];
extern AnimWord anim_8011A6F8[];
extern AnimWord anim_8011A700[];
extern AnimWord anim_8011A708[];
extern AnimWord anim_8011A710[];
extern AnimWord anim_8011A718[];
extern AnimWord anim_8011A720[];
extern AnimWord anim_8011A728[];
extern AnimWord anim_8011A730[];
extern AnimWord anim_8011A738[];

AnimWord anim_8011A470[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8011A478[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8011A480[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8011A488[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8011A490[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8011A498[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_8011A4A0[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_8011A4A8[2] = {
    ANIM_STOP(7, 0, 0, 60),
};

AnimWord anim_8011A4B0[16] = {
    ANIM(8, 0, 0, 9),
    ANIM(9, 0, 0, 5),
    ANIM(10, 0, 0, 7),
    ANIM(11, 0, 0, 5),
    ANIM(12, 0, 0, 9),
    ANIM(13, 0, 0, 5),
    ANIM(14, 0, 0, 7),
    ANIM_STOP(15, 0, 0, 5),
};

AnimWord anim_8011A4F0[6] = {
    ANIM(16, 0, 0, 5),
    ANIM(17, 0, 0, 5),
    ANIM_STOP(18, 0, 0, 5),
};

AnimWord anim_8011A508[9] = {
    ANIM(19, 0, 0, 8),
    ANIM(20, 0, 0, 8),
    ANIM(21, 0, 0, 8),
    ANIM_JUMP(20, 0, 0, 8, anim_8011A508),
};

AnimWord anim_8011A52C[9] = {
    ANIM(22, 0, 0, 8),
    ANIM(23, 0, 0, 8),
    ANIM(24, 0, 0, 8),
    ANIM_JUMP(23, 0, 0, 8, anim_8011A52C),
};

AnimWord anim_8011A550[8] = {
    ANIM(25, 0, 0, 9),
    ANIM(26, 0, 0, 7),
    ANIM(27, 0, 0, 9),
    ANIM_STOP(26, 0, 0, 7),
};

AnimWord anim_8011A570[2] = {
    ANIM_STOP(28, 0, 0, 60),
};

AnimWord anim_8011A578[28] = {
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

AnimWord anim_8011A5E8[4] = {
    ANIM(43, 0, 0, 12),
    ANIM_STOP(44, 0, 0, 12),
};

AnimWord anim_8011A5F8[4] = {
    ANIM(45, 0, 0, 16),
    ANIM_STOP(46, 0, 0, 16),
};

void *ptrs_8011A608[13] = {
    anim_8011A658,
    anim_8011A660,
    anim_8011A668,
    anim_8011A670,
    anim_8011A678,
    anim_8011A680,
    anim_8011A688,
    anim_8011A690,
    anim_8011A6C8,
    anim_8011A6E8,
    anim_8011A6F0,
    anim_8011A6F8,
    anim_8011A700,
};

void *ptrs_8011A63C[7] = {
    anim_8011A708,
    anim_8011A710,
    anim_8011A718,
    anim_8011A720,
    anim_8011A728,
    anim_8011A730,
    anim_8011A738,
};

AnimWord anim_8011A658[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8011A660[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8011A668[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8011A670[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8011A678[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8011A680[2] = {
    ANIM_STOP(24, 0, 0, 60),
};

AnimWord anim_8011A688[2] = {
    ANIM_STOP(25, 0, 0, 60),
};

AnimWord anim_8011A690[14] = {
    ANIM(26, 0, 0, 3),
    ANIM(27, 0, 0, 3),
    ANIM(28, 0, 0, 3),
    ANIM(29, 0, 0, 3),
    ANIM(28, 0, 0, 3),
    ANIM(27, 0, 0, 3),
    ANIM_STOP(26, 0, 0, 3),
};

AnimWord anim_8011A6C8[8] = {
    ANIM(30, 0, 0, 60),
    ANIM(31, 0, 0, 8),
    ANIM(32, 0, 0, 8),
    ANIM_STOP(33, 0, 0, 8),
};

AnimWord anim_8011A6E8[2] = {
    ANIM_STOP(34, 0, 0, 60),
};

AnimWord anim_8011A6F0[2] = {
    ANIM_STOP(35, 0, 0, 60),
};

AnimWord anim_8011A6F8[2] = {
    ANIM_STOP(13, 0, 0, 60),
};

AnimWord anim_8011A700[2] = {
    ANIM_STOP(14, 0, 0, 60),
};

AnimWord anim_8011A708[2] = {
    ANIM_STOP(15, 0, 0, 60),
};

AnimWord anim_8011A710[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_8011A718[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_8011A720[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_8011A728[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8011A730[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_8011A738[2] = {
    ANIM_STOP(5, 0, 0, 60),
};
