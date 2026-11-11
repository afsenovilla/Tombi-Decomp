// VERSION BETA_980203 beta980203 80031A24 232
#if defined(VERSION_BETA_980203)
// Beta version: PAL x006 initWeaponTrailSprite.c: w1e read from the table (PAL 0x13), b0a 13 (PAL 2), no animFrame copy.
#include "TOBJ.H"
extern unsigned short PLAYER_ANIM_FRAME;
extern int SPR_DATA[];
extern char WEAPON_TRAIL_DEFS[];
extern char D_8007A724[]; /* the weapon-trail table: +0 w1e, +4 (WEAPON_TRAIL_DEFS) sprite index */
static char DAT_80114c24[52] = {
    0x78, 0x22, 1, -0x80, -0x6d, 0, 0, 0, 4, 0, 0, 0, 0x7c, 0x22, 1, -0x80,
    -0x6d, 0, 0, 0, 4, 0, 0, 0, -0x80, 0x22, 1, -0x80, -0x6d, 0, 0, 0,
    4, 0, 0, 0, 0x74, 0x22, 1, -0x80, -0x6d, 0, 0, 0, 4, 0, 0, 0,
    0x70, 0x22, 1, -0x80,
};
extern void FUN_8001fe6c(TObj *);

void initWeaponTrailSprite(TObj *o)
{
    o->w1e = *(unsigned short *)(D_8007A724 + o->subtype * 12);
    o->d3c = SPR_DATA[*(short *)(WEAPON_TRAIL_DEFS + o->subtype * 12)];
    o->anim = *(void **)(*(int *)(DAT_80114c24 + o->subtype * 12) + o->b0c * 4);
    FUN_8001fe6c(o);
    o->b0a = 13;
    o->b0d = 0x80;
    *(signed char *)&o->b0f = -7;
    o->d8c = 0;
    o->b6b = 0;
    o->category |= 0x80;
    o->b04++;
}
#endif
