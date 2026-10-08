// FUNC 80118388 372 X017
// MATCHING 80118388 372
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80118388;

extern T80118388 D_80119994[];
extern int D_1F8002C8[];
extern unsigned char D_8009CE1A[];
extern void AnimLoadDuration(TObj *);

void func_80118388(TObj *o)
{
    o->box0 = D_80119994[o->subtype].b0;
    o->box1 = D_80119994[o->subtype].b1;
    o->box2 = D_80119994[o->subtype].b2;
    o->box3 = D_80119994[o->subtype].b3;
    o->w1e = D_80119994[o->subtype].w4;
    o->d3c = D_1F8002C8[D_80119994[o->subtype].w6];
    o->anim = D_80119994[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0f = 0;
    o->b0a = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
    if (D_8009CE1A[0]) o->step = 1;
}
