// FUNC 800ebc64 220 X009
// MATCHING 800ebc64 220
#include "TOBJ.H"
extern unsigned short PLAYER_ANIM_FRAME;
extern int SPR_DATA[];
extern char WEAPON_TRAIL_DEFS[];
static char DAT_80114c24[52] = {
    0x78, 0x22, 1, -0x80, -0x6d, 0, 0, 0, 4, 0, 0, 0, 0x7c, 0x22, 1, -0x80,
    -0x6d, 0, 0, 0, 4, 0, 0, 0, -0x80, 0x22, 1, -0x80, -0x6d, 0, 0, 0,
    4, 0, 0, 0, 0x74, 0x22, 1, -0x80, -0x6d, 0, 0, 0, 4, 0, 0, 0,
    0x70, 0x22, 1, -0x80,
};
extern void FUN_8001fe6c(TObj *);

void initWeaponTrailSprite(TObj *o)
{
    unsigned short u;
    o->w1e = 0x13;
    o->d3c = SPR_DATA[*(short *)(WEAPON_TRAIL_DEFS + o->subtype * 12)];
    o->anim = *(void **)(*(int *)(DAT_80114c24 + o->subtype * 12) + o->b0c * 4);
    FUN_8001fe6c(o);
    u = PLAYER_ANIM_FRAME;
    o->b0a = 2;
    o->b0d = 0x80;
    *(signed char *)&o->b0f = -7;
    o->d8c = 0;
    o->b6b = 0;
    o->animFrame = u & 1;
    o->category |= 0x80;
    o->b04++;
}
