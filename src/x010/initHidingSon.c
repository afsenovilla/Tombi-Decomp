// FUNC 8012dea0 368 X010
// MATCHING 8012dea0 368
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012DEA0;

extern T8012DEA0 HIDING_SON_DEFS[];
extern int SPR_DATA[];

extern void AnimLoadDuration(TObj *);

void initHidingSon(TObj *o)
{
    o->box0 = HIDING_SON_DEFS[o->subtype].b0;
    o->box1 = HIDING_SON_DEFS[o->subtype].b1;
    o->box2 = HIDING_SON_DEFS[o->subtype].b2;
    o->box3 = HIDING_SON_DEFS[o->subtype].b3;
    o->w1e = HIDING_SON_DEFS[o->subtype].w4;
    o->d3c = SPR_DATA[HIDING_SON_DEFS[o->subtype].w6];
    o->anim = HIDING_SON_DEFS[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->b0d = 1;
    o->w08 = 0x780a;
    o->active = 3;
    o->d8c = 0;
    o->b0f = 0;
    o->b0a = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
}
