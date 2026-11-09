// FUNC 80126f48 356 X010
// MATCHING 80126f48 356
#include "player.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80126F48;

extern T80126F48 TEN_THOUSAND_YEAR_ELDER_DEFS[];
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);

void initTenThousandYearElder(TObj *o)
{
    o->box0 = TEN_THOUSAND_YEAR_ELDER_DEFS[o->subtype].b0;
    o->box1 = TEN_THOUSAND_YEAR_ELDER_DEFS[o->subtype].b1;
    o->box2 = TEN_THOUSAND_YEAR_ELDER_DEFS[o->subtype].b2;
    o->box3 = TEN_THOUSAND_YEAR_ELDER_DEFS[o->subtype].b3;
    o->w1e = TEN_THOUSAND_YEAR_ELDER_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[TEN_THOUSAND_YEAR_ELDER_DEFS[o->subtype].w6];
    o->anim = TEN_THOUSAND_YEAR_ELDER_DEFS[o->subtype].anim[0];
    AnimLoadDuration(o);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = PLAYER.obj.b0f + 10;
    o->b04++;
}
