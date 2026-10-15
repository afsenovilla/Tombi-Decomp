// DATA 8011c46c 704 X011
// Data of X011 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_8011C46C[];
extern AnimWord anim_8011C490[];
extern AnimWord anim_8011C4B0[];
extern AnimWord anim_8011C4D0[];
extern AnimWord anim_8011C4F4[];
extern AnimWord anim_8011C518[];
extern AnimWord anim_8011C540[];
extern AnimWord anim_8011C568[];
extern AnimWord anim_8011C590[];
extern AnimWord anim_8011C5A8[];
extern AnimWord anim_8011C5C0[];
extern AnimWord anim_8011C5E0[];
extern AnimWord anim_8011C600[];
extern AnimWord anim_8011C628[];
extern AnimWord anim_8011C630[];
extern AnimWord anim_8011C654[];
extern AnimWord anim_8011C678[];
extern AnimWord anim_8011C680[];
extern AnimWord anim_8011C6A0[];
extern AnimWord anim_8011C6B0[];
extern AnimWord anim_8011C6D0[];

AnimWord anim_8011C46C[9] = {
    ANIM(0, 0, 0, 8),
    ANIM(1, 0, 0, 9),
    ANIM(2, 0, 0, 8),
    ANIM_JUMP(3, 0, 0, 7, anim_8011C46C),
};

AnimWord anim_8011C490[8] = {
    ANIM(51, 0, 0, 8),
    ANIM(52, 0, 0, 9),
    ANIM(53, 0, 0, 8),
    ANIM_STOP(54, 0, 0, 7),
};

AnimWord anim_8011C4B0[8] = {
    ANIM(55, 0, 0, 8),
    ANIM(56, 0, 0, 9),
    ANIM(57, 0, 0, 8),
    ANIM_STOP(58, 0, 0, 7),
};

AnimWord anim_8011C4D0[9] = {
    ANIM(59, 0, 0, 8),
    ANIM(60, 0, 0, 9),
    ANIM(61, 0, 0, 8),
    ANIM_JUMP(62, 0, 0, 7, anim_8011C4D0),
};

AnimWord anim_8011C4F4[9] = {
    ANIM(59, 0, 0, 8),
    ANIM(63, 0, 0, 9),
    ANIM(64, 0, 0, 8),
    ANIM_JUMP(65, 0, 0, 7, anim_8011C4F4),
};

AnimWord anim_8011C518[10] = {
    ANIM(4, 0, 0, 5),
    ANIM(5, 0, 0, 6),
    ANIM(6, 0, 0, 6),
    ANIM(7, 0, 0, 6),
    ANIM_STOP(8, 0, 0, 7),
};

AnimWord anim_8011C540[10] = {
    ANIM(9, 0, 0, 7),
    ANIM(10, 0, 0, 5),
    ANIM(11, 0, 0, 2),
    ANIM(12, 0, 0, 10),
    ANIM_STOP(11, 0, 0, 5),
};

AnimWord anim_8011C568[10] = {
    ANIM(66, 0, 0, 7),
    ANIM(67, 0, 0, 5),
    ANIM(68, 0, 0, 2),
    ANIM(69, 0, 0, 10),
    ANIM_STOP(68, 0, 0, 5),
};

AnimWord anim_8011C590[6] = {
    ANIM(13, 0, 0, 6),
    ANIM(14, 0, 0, 8),
    ANIM_STOP(15, 0, 0, 5),
};

AnimWord anim_8011C5A8[6] = {
    ANIM(70, 0, 0, 6),
    ANIM(71, 0, 0, 8),
    ANIM_STOP(72, 0, 0, 5),
};

AnimWord anim_8011C5C0[8] = {
    ANIM(16, 0, 0, 8),
    ANIM(17, 0, 0, 9),
    ANIM(18, 0, 0, 8),
    ANIM_STOP(19, 0, 0, 7),
};

AnimWord anim_8011C5E0[8] = {
    ANIM(73, 0, 0, 8),
    ANIM(74, 0, 0, 9),
    ANIM(75, 0, 0, 8),
    ANIM_STOP(76, 0, 0, 7),
};

AnimWord anim_8011C600[10] = {
    ANIM(20, 0, 0, 8),
    ANIM(21, 0, 0, 7),
    ANIM(22, 0, 0, 5),
    ANIM(6, 0, 0, 6),
    ANIM(7, 0, 0, 7),
};

AnimWord anim_8011C628[2] = {
    ANIM_STOP(8, 0, 0, 8),
};

AnimWord anim_8011C630[9] = {
    ANIM(23, 0, 0, 6),
    ANIM(24, 0, 0, 8),
    ANIM(25, 0, 0, 6),
    ANIM_JUMP(26, 0, 0, 8, anim_8011C630),
};

AnimWord anim_8011C654[9] = {
    ANIM(27, 0, 0, 7),
    ANIM(28, 0, 0, 7),
    ANIM(29, 0, 0, 7),
    ANIM_JUMP(30, 0, 0, 7, anim_8011C654),
};

AnimWord anim_8011C678[2] = {
    ANIM_STOP(31, 0, 0, 60),
};

AnimWord anim_8011C680[8] = {
    ANIM(32, 0, 0, 5),
    ANIM(33, 0, 0, 6),
    ANIM(34, 0, 0, 7),
    ANIM_STOP(35, 0, 0, 9),
};

AnimWord anim_8011C6A0[4] = {
    ANIM(6, 0, 0, 6),
    ANIM_STOP(36, 0, 0, 7),
};

AnimWord anim_8011C6B0[8] = {
    ANIM(37, 0, 0, 6),
    ANIM(38, 0, 0, 6),
    ANIM(39, 0, 0, 6),
    ANIM(40, 0, 0, 6),
};

AnimWord anim_8011C6D0[23] = {
    ANIM_JUMP(41, 0, 0, 6, anim_8011C6B0),
    ANIM_STOP(42, 0, 0, 6),
    ANIM(43, 0, 0, 8),
    ANIM(44, 0, 0, 6),
    ANIM(45, 0, 0, 8),
    ANIM_STOP(44, 0, 0, 7),
    ANIM(46, 0, 0, 6),
    ANIM(47, 0, 0, 8),
    ANIM_STOP(48, 0, 0, 11),
    ANIM(49, 0, 0, 6),
    ANIM_STOP(50, 0, 0, 255),
};
