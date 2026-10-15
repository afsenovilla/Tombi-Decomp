// FUNC 800f434c 132 X000
// MATCHING 800f434c 132
// VERSION SLPM_80165 x00 800F129C 108
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800F434C (playerLandResumeWalk.c): Does not reset +0x8c from the per-object table.
#include "TOBJ.H"
extern unsigned char DAT_801152e8[];
extern void func_8001e5f4(int a, int b);

void playerLandResumeWalk(TObj *o)
{
    func_8001e5f4(0x1c, 0x7f);
    o->b9c = 0;
    o->ba7 = 0;
    *(char *)&o->wac = 0;
    o->b9d = 0;
    if (o->bbe & 0x20) {
        o->step = 0x1f;
        o->state = 1;
    } else {
        o->step = 1;
        o->state = 0;
    }
}
#else
#include "TOBJ.H"
extern unsigned char DAT_801152e8[];
extern void func_8001e5f4(int a, int b);

void playerLandResumeWalk(TObj *o)
{
    func_8001e5f4(0x1c, 0x7f);
    o->b9c = 0;
    o->ba7 = 0;
    *(char *)&o->wac = 0;
    o->b9d = 0;
    o->d8c = DAT_801152e8[o->wb0];
    if (o->bbe & 0x20) {
        o->step = 0x1f;
        o->state = 1;
    } else {
        o->step = 1;
        o->state = 0;
    }
}
#endif
