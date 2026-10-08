// FUNC 8012dea0 368 X010
// MATCHING 8012dea0 368
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012DEA0;

extern T8012DEA0 D_8012F530[];
extern int D_1F8002C8[];

extern void AnimLoadDuration(TObj *);

void func_8012DEA0(TObj *o)
{
    o->box0 = D_8012F530[o->subtype].b0;
    o->box1 = D_8012F530[o->subtype].b1;
    o->box2 = D_8012F530[o->subtype].b2;
    o->box3 = D_8012F530[o->subtype].b3;
    o->w1e = D_8012F530[o->subtype].w4;
    o->d3c = D_1F8002C8[D_8012F530[o->subtype].w6];
    o->anim = D_8012F530[o->subtype].anim[(unsigned short)o->wb4];
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
