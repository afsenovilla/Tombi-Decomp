// DATA 80122fe8 340 X006
// Data of X006 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_80122FE8[];
extern AnimWord anim_80122FF0[];
extern AnimWord anim_80123004[];
extern AnimWord anim_8012301C[];
extern AnimWord anim_80123024[];
extern AnimWord anim_8012302C[];
extern AnimWord anim_8012303C[];
extern AnimWord anim_80123070[];
extern AnimWord anim_80123084[];
extern AnimWord anim_801230A8[];
extern AnimWord anim_801230CC[];
extern AnimWord anim_801230F0[];
extern AnimWord anim_80123118[];
extern AnimWord anim_80123134[];

AnimWord anim_80122FE8[2] = {
    ANIM_STOP(29, 0, 0, 60),
};

AnimWord anim_80122FF0[5] = {
    ANIM(30, 0, 0, 16),
    ANIM_JUMP(31, 0, 0, 16, anim_80122FF0),
};

AnimWord anim_80123004[6] = {
    ANIM(20, 0, 0, 15),
    ANIM(21, 0, 0, 15),
    ANIM_STOP(22, 0, 0, 60),
};

AnimWord anim_8012301C[2] = {
    ANIM_STOP(2, 0, 0, 8),
};

AnimWord anim_80123024[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_8012302C[4] = {
    ANIM(0, 0, 0, 8),
    ANIM_STOP(1, 0, 0, 8),
};

AnimWord anim_8012303C[13] = {
    ANIM(4, 0, 0, 45),
    ANIM(5, 0, 0, 8),
    ANIM(4, 0, 0, 20),
    ANIM(5, 0, 0, 8),
    ANIM(4, 0, 0, 6),
    ANIM_JUMP(5, 0, 0, 8, anim_8012303C),
};

AnimWord anim_80123070[5] = {
    ANIM(18, 0, 0, 32),
    ANIM_JUMP(19, 0, 0, 8, anim_80123070),
};

AnimWord anim_80123084[9] = {
    ANIM(12, 0, 0, 40),
    ANIM(13, 0, 0, 6),
    ANIM(14, 0, 0, 32),
    ANIM_JUMP(13, 0, 0, 6, anim_80123084),
};

AnimWord anim_801230A8[9] = {
    ANIM(9, 0, 0, 40),
    ANIM(10, 0, 0, 6),
    ANIM(11, 0, 0, 32),
    ANIM_JUMP(10, 0, 0, 6, anim_801230A8),
};

AnimWord anim_801230CC[9] = {
    ANIM(15, 0, 0, 32),
    ANIM(16, 0, 0, 10),
    ANIM(17, 0, 0, 56),
    ANIM_JUMP(16, 0, 0, 10, anim_801230CC),
};

AnimWord anim_801230F0[10] = {
    ANIM(24, 0, 0, 6),
    ANIM(25, 0, 0, 8),
    ANIM(26, 0, 0, 5),
    ANIM(27, 0, 0, 5),
    ANIM_STOP(28, 0, 0, 5),
};

AnimWord anim_80123118[7] = {
    ANIM(6, 0, 0, 12),
    ANIM(7, 0, 0, 6),
    ANIM_JUMP(8, 0, 0, 12, anim_80123118),
};

AnimWord anim_80123134[2] = {
    ANIM_STOP(23, 0, 0, 60),
};
