// FUNC 8011af84 48 X006
// MATCHING 8011af84 48
// VERSION SLPS_01144 x06 8011A114 48
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x06/nonmatchings/x06", func_8011A114_slps01144);
#else
#include "spad.h"
#include "TOBJ.H"

void func_8011AF84(TObj *o)
{
    int *pp = &SPAD.i334;
    int i;
    char *b;

    b = (char *)*pp;
    i = o->wac << 2;
    b += i;
    i = *(int *volatile *)pp;
    i += ((int *)b)[1];
    o->da0 = i;
}
#endif
