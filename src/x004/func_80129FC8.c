// FUNC 80129fc8 348 X004
// MATCHING 80129fc8 348
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80129FC8;

extern T80129FC8 D_801311B0[];
extern int D_1F8002C8[];

extern void AnimLoadDuration(TObj *);

void func_80129FC8(TObj *o)
{
    o->box0 = D_801311B0[o->subtype].b0;
    o->box1 = D_801311B0[o->subtype].b1;
    o->box2 = D_801311B0[o->subtype].b2;
    o->box3 = D_801311B0[o->subtype].b3;
    o->w1e = D_801311B0[o->subtype].w4;
    o->d3c = D_1F8002C8[D_801311B0[o->subtype].w6];
    o->anim = D_801311B0[o->subtype].anim[o->b6b];
    AnimLoadDuration(o);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b04++;
}
