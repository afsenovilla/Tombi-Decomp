// FUNC 80129344 368 X009
// MATCHING 80129344 368
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80129344;

extern T80129344 D_8012B2DC[];
extern int D_1F8002C8[];

extern void AnimLoadDuration(TObj *);

void func_80129344(TObj *o)
{
    o->box0 = D_8012B2DC[o->subtype].b0;
    o->box1 = D_8012B2DC[o->subtype].b1;
    o->box2 = D_8012B2DC[o->subtype].b2;
    o->box3 = D_8012B2DC[o->subtype].b3;
    o->w1e = D_8012B2DC[o->subtype].w4;
    o->d3c = D_1F8002C8[D_8012B2DC[o->subtype].w6];
    o->anim = D_8012B2DC[o->subtype].anim[(unsigned short)o->wb4];
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
