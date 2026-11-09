// FUNC 8011d204 252 X000
// MATCHING 8011d204 252
// probably: frame helpers: steps animFrame toward 0x80 (and func_8011D230 up to 0x8F); unreferenced
// VERSION SLPM_80165 x00 80116314 44
// VERSION SLPS_01144 x00 8011C1E4 252
// VERSION PCPX_96084 tomba 8003B068 44
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_SLPM_80165)
// Demo-only helper: steps the frame (+0x2e) towards 128.
void func_80116314(char *o)
{
    unsigned short x = *(unsigned short *)(o + 0x2e);
    if (x != 128) {
        if (x <= 128) *(unsigned short *)(o + 0x2e) = x + 1;
        else *(unsigned short *)(o + 0x2e) = x - 1;
    }
}
#elif defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x000 8011D204 (chainedPartFrameTo80.c): SLPM_80165 C of src/chainedPartFrameTo80.c (func_80116314_slpm80165, twin score 0.909) [-O2 -G0] (tools/demo_sweep.py)
// Demo-only helper: steps the frame (+0x2e) towards 128.
void func_80116314(char *o)
{
    unsigned short x = *(unsigned short *)(o + 0x2e);
    if (x != 128) {
        if (x <= 128) *(unsigned short *)(o + 0x2e) = x + 1;
        else *(unsigned short *)(o + 0x2e) = x - 1;
    }
}
#else
#include "TOBJ.H"

void chainedPartFrameTo80(TObj *o)
{
    if (o->animFrame != 0x80) {
        if (o->animFrame < 0x81) o->animFrame++;
        else o->animFrame--;
    }
}

void func_8011D230(TObj *o)
{
    if (o->animFrame != 0x8f) o->animFrame++;
}

void func_8011D250(TObj *o)
{
    if (o->animFrame != 0x87) {
        if (o->animFrame < 0x87) o->animFrame++;
        else o->animFrame--;
    }
}

void func_8011D27C(TObj *o)
{
    if (o->animFrame != 0x83) {
        if (o->animFrame < 0x83) o->animFrame++;
        else o->animFrame--;
    }
}

void func_8011D2A8(TObj *o)
{
    o->animFrame += 2;
    if (o->animFrame >= 0x90) o->animFrame = 0x8f;
}

void func_8011D2D4(TObj *o)
{
    o->animFrame += 2;
    if (o->animFrame >= 0x88) o->animFrame = 0x87;
}
#endif
