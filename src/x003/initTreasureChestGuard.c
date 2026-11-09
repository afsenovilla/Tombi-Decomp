// FUNC 8012bd5c 384 X003
// MATCHING 8012bd5c 384
// VERSION SLPS_01144 x03 8012AD48 380
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012BD5C;

extern T8012BD5C TREASURE_CHEST_GUARD_DEFS[];
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);

void initTreasureChestGuard(TObj *o)
{
    o->box0 = TREASURE_CHEST_GUARD_DEFS[o->subtype].b0;
    o->box1 = TREASURE_CHEST_GUARD_DEFS[o->subtype].b1;
    o->box2 = TREASURE_CHEST_GUARD_DEFS[o->subtype].b2;
    o->box3 = TREASURE_CHEST_GUARD_DEFS[o->subtype].b3;
    o->w1e = TREASURE_CHEST_GUARD_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[TREASURE_CHEST_GUARD_DEFS[o->subtype].w6];
    o->anim = TREASURE_CHEST_GUARD_DEFS[o->subtype].anim[0];
    AnimLoadDuration(o);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = PLAYER.obj.b0f + 1;
    o->b04++;
    if (GCD.bce5d == 0xff) o->b04 = 3;
}
#else
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012BD5C;

extern T8012BD5C TREASURE_CHEST_GUARD_DEFS[];
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);

void initTreasureChestGuard(TObj *o)
{
    o->box0 = TREASURE_CHEST_GUARD_DEFS[o->subtype].b0;
    o->box1 = TREASURE_CHEST_GUARD_DEFS[o->subtype].b1;
    o->box2 = TREASURE_CHEST_GUARD_DEFS[o->subtype].b2;
    o->box3 = TREASURE_CHEST_GUARD_DEFS[o->subtype].b3;
    o->w1e = TREASURE_CHEST_GUARD_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[TREASURE_CHEST_GUARD_DEFS[o->subtype].w6];
    o->anim = TREASURE_CHEST_GUARD_DEFS[o->subtype].anim[0];
    AnimLoadDuration(o);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = PLAYER.obj.b0f + 1;
    o->b04++;
    *(int *)((char *)o + 0x88) = 0;
    if (GCD.bce5d == 0xff) o->b04 = 3;
}
#endif
