// DATA 80132c84 504 X010
// Data of X010 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern void *ptrs_80132C84[];
extern AnimWord anim_80132C94[];
extern AnimWord anim_80132C9C[];
extern AnimWord anim_80132CA4[];
extern AnimWord anim_80132CAC[];
extern AnimWord anim_80132CB4[];
extern AnimWord anim_80132CBC[];
extern AnimWord anim_80132CC4[];
extern AnimWord anim_80132CCC[];
extern AnimWord anim_80132CD4[];
extern AnimWord anim_80132CDC[];
extern AnimWord anim_80132CE4[];
extern AnimWord anim_80132CEC[];
extern AnimWord anim_80132CF4[];
extern AnimWord anim_80132D18[];
extern AnimWord anim_80132D3C[];
extern AnimWord anim_80132D60[];
extern AnimWord anim_80132D84[];
extern AnimWord anim_80132DA8[];
extern AnimWord anim_80132DCC[];
extern AnimWord anim_80132DD4[];
extern AnimWord anim_80132E08[];
extern AnimWord anim_80132E2C[];

void *ptrs_80132C84[4] = {
    anim_80132DCC,
    anim_80132DD4,
    anim_80132E08,
    anim_80132E2C,
};

AnimWord anim_80132C94[2] = {
    ANIM_STOP(0, 0, 0, 60),
};

AnimWord anim_80132C9C[2] = {
    ANIM_STOP(1, 0, 0, 60),
};

AnimWord anim_80132CA4[2] = {
    ANIM_STOP(2, 0, 0, 60),
};

AnimWord anim_80132CAC[2] = {
    ANIM_STOP(3, 0, 0, 60),
};

AnimWord anim_80132CB4[2] = {
    ANIM_STOP(4, 0, 0, 60),
};

AnimWord anim_80132CBC[2] = {
    ANIM_STOP(5, 0, 0, 60),
};

AnimWord anim_80132CC4[2] = {
    ANIM_STOP(6, 0, 0, 60),
};

AnimWord anim_80132CCC[2] = {
    ANIM_STOP(7, 0, 0, 60),
};

AnimWord anim_80132CD4[2] = {
    ANIM_STOP(8, 0, 0, 60),
};

AnimWord anim_80132CDC[2] = {
    ANIM_STOP(9, 0, 0, 60),
};

AnimWord anim_80132CE4[2] = {
    ANIM_STOP(10, 0, 0, 60),
};

AnimWord anim_80132CEC[2] = {
    ANIM_STOP(11, 0, 0, 60),
};

AnimWord anim_80132CF4[9] = {
    ANIM(12, 0, 0, 10),
    ANIM(13, 0, 0, 10),
    ANIM(12, 0, 0, 10),
    ANIM_JUMP(14, 0, 0, 10, anim_80132CF4),
};

AnimWord anim_80132D18[9] = {
    ANIM(15, 0, 0, 10),
    ANIM(16, 0, 0, 10),
    ANIM(15, 0, 0, 10),
    ANIM_JUMP(17, 0, 0, 10, anim_80132D18),
};

AnimWord anim_80132D3C[9] = {
    ANIM(18, 0, 0, 9),
    ANIM(19, 0, 0, 9),
    ANIM(18, 0, 0, 9),
    ANIM_JUMP(20, 0, 0, 9, anim_80132D3C),
};

AnimWord anim_80132D60[9] = {
    ANIM(21, 0, 0, 9),
    ANIM(22, 0, 0, 9),
    ANIM(21, 0, 0, 9),
    ANIM_JUMP(23, 0, 0, 9, anim_80132D60),
};

AnimWord anim_80132D84[9] = {
    ANIM(24, 0, 0, 8),
    ANIM(25, 0, 0, 8),
    ANIM(24, 0, 0, 8),
    ANIM_JUMP(26, 0, 0, 8, anim_80132D84),
};

AnimWord anim_80132DA8[9] = {
    ANIM(27, 0, 0, 7),
    ANIM(28, 0, 0, 7),
    ANIM(27, 0, 0, 7),
    ANIM_JUMP(29, 0, 0, 7, anim_80132DA8),
};

AnimWord anim_80132DCC[2] = {
    ANIM_STOP(30, 0, 0, 60),
};

AnimWord anim_80132DD4[13] = {
    ANIM(31, 0, 0, 9),
    ANIM(32, 0, 0, 9),
    ANIM(33, 0, 0, 11),
    ANIM(34, 0, 0, 9),
    ANIM(35, 0, 0, 9),
    ANIM_JUMP(36, 0, 0, 11, anim_80132DD4),
};

AnimWord anim_80132E08[9] = {
    ANIM(37, 0, 0, 12),
    ANIM(38, 0, 0, 12),
    ANIM(39, 0, 0, 12),
    ANIM_JUMP(38, 0, 0, 12, anim_80132E08),
};

AnimWord anim_80132E2C[20] = {
    ANIM(40, 0, 0, 6),
    ANIM(41, 0, 0, 8),
    ANIM(42, 0, 0, 8),
    ANIM(41, 0, 0, 8),
    ANIM(42, 0, 0, 8),
    ANIM(41, 0, 0, 8),
    ANIM(42, 0, 0, 8),
    ANIM(43, 0, 0, 6),
    ANIM(44, 0, 0, 6),
    ANIM_STOP(45, 0, 0, 60),
};
