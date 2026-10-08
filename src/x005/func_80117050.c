// FUNC 80117050 340 X005
// MATCHING 80117050 340
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80117050;

extern T80117050 D_801176C4[];
extern int D_1F8002C8[];

extern void AnimLoadDuration(TObj *);

void func_80117050(TObj *o)
{
    o->box0 = D_801176C4[o->subtype].b0;
    o->box1 = D_801176C4[o->subtype].b1;
    o->box2 = D_801176C4[o->subtype].b2;
    o->box3 = D_801176C4[o->subtype].b3;
    o->w1e = D_801176C4[o->subtype].w4;
    o->d3c = D_1F8002C8[D_801176C4[o->subtype].w6];
    o->anim = D_801176C4[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0f = 0;
    o->category |= 0x80;
    o->b04++;
}
