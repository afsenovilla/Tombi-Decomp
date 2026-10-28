// FUNC 801072fc 196 X000
// MATCHING 801072fc 196
// probably: player slide entry: movetab MOVE_TABLE_2, speed wb2 clamped to +-0x144 by facing, state 1; unreferenced
// VERSION SLPS_01144 x00 80107020 196
// VERSION SLPM_80165 x00 80100B84 196
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 801072FC (playerSlideEntry.c): 227 -> 211.
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
extern char MOVE_TABLE_2[];
void playerSlideEntry(TObj *o)
{
    ((unsigned char *)o)[0xc7] = 1;
    o->b9d = 0;
    ((unsigned char *)o)[0xc6] = 0;
    ((unsigned char *)o)[0xd3] = 0;
    PLAYER_EXT[0] = 0;
    o->movetab = MOVE_TABLE_2;
    *(unsigned char *)&o->wac = 0;
    o->b6b = 0;
    o->ba4 = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    PLAYER_EXT[8] = o->bbe & 1;
    if (PLAYER_EXT[8] == 0) {
        if (o->wb2 < -0x144) o->wb2 = -0x144;
    } else {
        if (o->wb2 > 0x144) o->wb2 = 0x144;
    }
    o->state = 1;
    o->animFrame = o->bbe & 1;
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL x000 801072FC (playerSlideEntry.c): flag at +0xcf (PAL +0xe3).
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
extern char MOVE_TABLE_2[];
void playerSlideEntry(TObj *o)
{
    ((unsigned char *)o)[0xc7] = 1;
    o->b9d = 0;
    ((unsigned char *)o)[0xc6] = 0;
    ((unsigned char *)o)[0xcf] = 0;
    PLAYER_EXT[0] = 0;
    o->movetab = MOVE_TABLE_2;
    *(unsigned char *)&o->wac = 0;
    o->b6b = 0;
    o->ba4 = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    PLAYER_EXT[8] = o->bbe & 1;
    if (PLAYER_EXT[8] == 0) {
        if (o->wb2 < -0x144) o->wb2 = -0x144;
    } else {
        if (o->wb2 > 0x144) o->wb2 = 0x144;
    }
    o->state = 1;
    o->animFrame = o->bbe & 1;
}
#else
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
extern char MOVE_TABLE_2[];
void playerSlideEntry(TObj *o)
{
    ((unsigned char *)o)[0xc7] = 1;
    o->b9d = 0;
    ((unsigned char *)o)[0xc6] = 0;
    ((unsigned char *)o)[0xe3] = 0;
    PLAYER_EXT[0] = 0;
    o->movetab = MOVE_TABLE_2;
    *(unsigned char *)&o->wac = 0;
    o->b6b = 0;
    o->ba4 = 0;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    PLAYER_EXT[8] = o->bbe & 1;
    if (PLAYER_EXT[8] == 0) {
        if (o->wb2 < -0x144) o->wb2 = -0x144;
    } else {
        if (o->wb2 > 0x144) o->wb2 = 0x144;
    }
    o->state = 1;
    o->animFrame = o->bbe & 1;
}
#endif
