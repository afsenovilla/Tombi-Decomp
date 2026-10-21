// FUNC 801072fc 196 X000
// MATCHING 801072fc 196
// probably: player slide entry: movetab D_80077D0C, speed wb2 clamped to +-0x144 by facing, state 1; unreferenced
// VERSION SLPS_01144 x00 80107020 196
// VERSION SLPM_80165 x00 80100B84 196
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 801072FC (func_801072FC.c): 227 -> 211.
#include "TOBJ.H"
extern unsigned char *D_8009C330;
extern char D_80077D0C[];
void func_801072FC(TObj *o)
{
    ((unsigned char *)o)[0xc7] = 1;
    o->b9d = 0;
    ((unsigned char *)o)[0xc6] = 0;
    ((unsigned char *)o)[0xd3] = 0;
    D_8009C330[0] = 0;
    o->movetab = D_80077D0C;
    *(unsigned char *)&o->wac = 0;
    o->b6b = 0;
    o->ba4 = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    D_8009C330[8] = o->bbe & 1;
    if (D_8009C330[8] == 0) {
        if (o->wb2 < -0x144) o->wb2 = -0x144;
    } else {
        if (o->wb2 > 0x144) o->wb2 = 0x144;
    }
    o->state = 1;
    o->animFrame = o->bbe & 1;
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL x000 801072FC (func_801072FC.c): flag at +0xcf (PAL +0xe3).
#include "TOBJ.H"
extern unsigned char *D_8009C330;
extern char D_80077D0C[];
void func_801072FC(TObj *o)
{
    ((unsigned char *)o)[0xc7] = 1;
    o->b9d = 0;
    ((unsigned char *)o)[0xc6] = 0;
    ((unsigned char *)o)[0xcf] = 0;
    D_8009C330[0] = 0;
    o->movetab = D_80077D0C;
    *(unsigned char *)&o->wac = 0;
    o->b6b = 0;
    o->ba4 = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    D_8009C330[8] = o->bbe & 1;
    if (D_8009C330[8] == 0) {
        if (o->wb2 < -0x144) o->wb2 = -0x144;
    } else {
        if (o->wb2 > 0x144) o->wb2 = 0x144;
    }
    o->state = 1;
    o->animFrame = o->bbe & 1;
}
#else
#include "TOBJ.H"
extern unsigned char *D_8009C330;
extern char D_80077D0C[];
void func_801072FC(TObj *o)
{
    ((unsigned char *)o)[0xc7] = 1;
    o->b9d = 0;
    ((unsigned char *)o)[0xc6] = 0;
    ((unsigned char *)o)[0xe3] = 0;
    D_8009C330[0] = 0;
    o->movetab = D_80077D0C;
    *(unsigned char *)&o->wac = 0;
    o->b6b = 0;
    o->ba4 = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    D_8009C330[8] = o->bbe & 1;
    if (D_8009C330[8] == 0) {
        if (o->wb2 < -0x144) o->wb2 = -0x144;
    } else {
        if (o->wb2 > 0x144) o->wb2 = 0x144;
    }
    o->state = 1;
    o->animFrame = o->bbe & 1;
}
#endif
