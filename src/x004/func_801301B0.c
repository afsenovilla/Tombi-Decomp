// FUNC 801301b0 368 X004
// MATCHING 801301b0 368
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T801301B0;

extern T801301B0 D_801314C0[];
extern int D_1F8002C8[];

extern void AnimLoadDuration(TObj *);

void func_801301B0(TObj *o)
{
    o->box0 = D_801314C0[o->subtype].b0;
    o->box1 = D_801314C0[o->subtype].b1;
    o->box2 = D_801314C0[o->subtype].b2;
    o->box3 = D_801314C0[o->subtype].b3;
    o->w1e = D_801314C0[o->subtype].w4;
    o->d3c = D_1F8002C8[D_801314C0[o->subtype].w6];
    o->anim = D_801314C0[o->subtype].anim[(unsigned short)o->wb4];
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
