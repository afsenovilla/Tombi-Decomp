// FUNC 80129344 368 X009
// MATCHING 80129344 368
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80129344;

extern T80129344 FLOWER_TEARS_SEEKER_DEFS[];
extern int SPR_DATA[];

extern void AnimLoadDuration(TObj *);

void initFlowerTearsSeeker(TObj *o)
{
    o->box0 = FLOWER_TEARS_SEEKER_DEFS[o->subtype].b0;
    o->box1 = FLOWER_TEARS_SEEKER_DEFS[o->subtype].b1;
    o->box2 = FLOWER_TEARS_SEEKER_DEFS[o->subtype].b2;
    o->box3 = FLOWER_TEARS_SEEKER_DEFS[o->subtype].b3;
    o->w1e = FLOWER_TEARS_SEEKER_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[FLOWER_TEARS_SEEKER_DEFS[o->subtype].w6];
    o->anim = FLOWER_TEARS_SEEKER_DEFS[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->active = 10;
    o->b0d = 1;
    o->w08 = 0x7c12;
    o->d8c = 0;
    o->b0a = 0;
    o->b0f = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
}
