// FUNC 80125ae0 368 X010
// MATCHING 80125ae0 368
#include "player.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80125AE0;

extern T80125AE0 LOST_HAT_MAN_DEFS[];
extern int SPR_DATA[];
extern void AnimJump(TObj *, int);
extern void AnimLoadDuration(TObj *);

void initLostHatMan(TObj *o)
{
    o->box0 = LOST_HAT_MAN_DEFS[o->subtype].b0;
    o->box1 = LOST_HAT_MAN_DEFS[o->subtype].b1;
    o->box2 = LOST_HAT_MAN_DEFS[o->subtype].b2;
    o->box3 = LOST_HAT_MAN_DEFS[o->subtype].b3;
    o->w1e = LOST_HAT_MAN_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[LOST_HAT_MAN_DEFS[o->subtype].w6];
    AnimLoadDuration(o);
    o->anim = LOST_HAT_MAN_DEFS[o->subtype].anim[0];
    AnimJump(o, 0);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = PLAYER.obj.b0f + 1;
    o->b04++;
}
