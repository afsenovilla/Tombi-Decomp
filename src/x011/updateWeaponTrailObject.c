// FUNC 800ec420 468 X011
// MATCHING 800ec420 468
#include "TOBJ.H"
typedef struct { short k; short pad; void **anims; int pad2; } T12;
extern T12 WEAPON_TRAIL_DEFS[];
extern int SPR_DATA[];
extern unsigned short PLAYER_ANIM_FRAME;
typedef struct G { char p0; unsigned char b1; char p1[0xf - 2]; unsigned char b0f; } G;
extern G PLAYER;
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void weaponTrailFollowStep(TObj *);
extern void weaponTrailOrbitStep(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_800187e4(TObj *);

void updateWeaponTrailObject(TObj *o)
{
    unsigned char s = o->b04;
    unsigned short v;
    switch (s) {
    case 0:
        if (o->subtype == 9)
            goto inc;
        o->w1e = 0x13;
        o->d3c = SPR_DATA[WEAPON_TRAIL_DEFS[o->subtype].k];
        o->anim = WEAPON_TRAIL_DEFS[o->subtype].anims[o->b0c];
        FUN_8001fe6c(o);
        v = PLAYER_ANIM_FRAME;
        o->b0a = 2;
        o->b0d = 0x80;
        *(signed char *)&o->b0f = -7;
        o->d8c = 0;
        o->b6b = 0;
        o->animFrame = v & 1;
        o->category |= 0x80;
        o->b04++;
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
            weaponTrailFollowStep(o);
            break;
        case 3:
        case 4:
            weaponTrailOrbitStep(o);
            break;
        case 9:
            o->visible = PLAYER.b1;
            o->b0f = PLAYER.b0f - 1;
            if (FUN_8001fec0(o) != 0)
                o->b04 = 3;
            break;
        }
        break;
    case 2:
    inc:
        o->b04 = s + 1;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
