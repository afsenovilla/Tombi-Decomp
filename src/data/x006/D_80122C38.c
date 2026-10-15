// DATA 80122c38 396 X006
// Data of X006 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_80122C38[];
extern AnimWord anim_80122C40[];
extern AnimWord anim_80122C48[];
extern AnimWord anim_80122C7C[];
extern AnimWord anim_80122CB0[];
extern AnimWord anim_80122CE4[];
extern AnimWord anim_80122CF4[];
extern AnimWord anim_80122D04[];
extern AnimWord anim_80122D28[];
extern AnimWord anim_80122D30[];
extern AnimWord anim_80122D44[];
extern AnimWord anim_80122D58[];
extern AnimWord anim_80122D6C[];
extern AnimWord anim_80122D7C[];
extern AnimWord anim_80122D94[];
extern AnimWord anim_80122D9C[];

AnimWord anim_80122C38[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80122C40[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_80122C48[13] = {
    ANIM(8, 0, 0, 30),
    ANIM(2, 0, 0, 20),
    ANIM(3, 0, 0, 15),
    ANIM(4, 0, 0, 35),
    ANIM(3, 0, 0, 15),
    ANIM_JUMP(2, 0, 0, 20, anim_80122C48),
};

AnimWord anim_80122C7C[13] = {
    ANIM(8, 0, 0, 30),
    ANIM(5, 0, 0, 20),
    ANIM(6, 0, 0, 15),
    ANIM(7, 0, 0, 35),
    ANIM(6, 0, 0, 15),
    ANIM_JUMP(5, 0, 0, 20, anim_80122C7C),
};

AnimWord anim_80122CB0[13] = {
    ANIM(12, 0, 0, 30),
    ANIM(9, 0, 0, 20),
    ANIM(10, 0, 0, 15),
    ANIM(11, 0, 0, 35),
    ANIM(10, 0, 0, 15),
    ANIM_JUMP(9, 0, 0, 20, anim_80122CB0),
};

AnimWord anim_80122CE4[4] = {
    ANIM(13, 0, 0, 15),
    ANIM_STOP(15, 0, 0, 15),
};

AnimWord anim_80122CF4[4] = {
    ANIM(14, 0, 0, 15),
    ANIM_STOP(16, 0, 0, 15),
};

AnimWord anim_80122D04[9] = {
    ANIM(18, 0, 0, 72),
    ANIM(19, 0, 0, 9),
    ANIM(20, 0, 0, 9),
    ANIM_JUMP(19, 0, 0, 9, anim_80122D04),
};

AnimWord anim_80122D28[2] = {
    ANIM_STOP(21, 0, 0, 8),
};

AnimWord anim_80122D30[5] = {
    ANIM(22, 0, 0, 16),
    ANIM_JUMP(23, 0, 0, 16, anim_80122D30),
};

AnimWord anim_80122D44[5] = {
    ANIM(24, 0, 0, 6),
    ANIM_JUMP(25, 0, 0, 6, anim_80122D44),
};

AnimWord anim_80122D58[5] = {
    ANIM(26, 0, 0, 16),
    ANIM_JUMP(27, 0, 0, 16, anim_80122D58),
};

AnimWord anim_80122D6C[4] = {
    ANIM(28, 0, 0, 16),
    ANIM_STOP(29, 0, 0, 5),
};

AnimWord anim_80122D7C[6] = {
    ANIM(29, 0, 0, 5),
    ANIM(30, 0, 0, 4),
    ANIM_STOP(31, 0, 0, 2),
};

AnimWord anim_80122D94[2] = {
    ANIM_STOP(31, 0, 0, 60),
};

AnimWord anim_80122D9C[10] = {
    ANIM(31, 0, 0, 2),
    ANIM(32, 0, 0, 4),
    ANIM(33, 0, 0, 4),
    ANIM(34, 0, 0, 4),
    ANIM_STOP(35, 0, 0, 60),
};
