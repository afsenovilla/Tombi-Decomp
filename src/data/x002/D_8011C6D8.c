// DATA 8011c6d8 80 X002
// Data of X002 that no C function defines (yet): one array per address something points to; animation scripts
// with the macros of include/tomba/anim.h, pointers as symbols, strings as strings, the rest as words.
#include "anim.h"

extern AnimWord anim_8011C6D8[];
extern AnimWord anim_8011C708[];

AnimWord anim_8011C6D8[12] = {
    ANIM_STOP(2, 3, -1736, 17),
    ANIM_STOP(2, 3, -1728, 17),
    ANIM_STOP(2, 3, -1708, 17),
    ANIM_STOP(2, 3, -1692, 17),
    ANIM_STOP(2, 3, -1676, 17),
    ANIM_STOP(2, 3, -1660, 17),
};

AnimWord anim_8011C708[8] = {
    ANIM_STOP(2, 3, -1644, 17),
    ANIM_STOP(2, 3, -1628, 17),
    ANIM_STOP(2, 3, -1612, 17),
    ANIM_STOP(5, 3, -1596, 17),
};
