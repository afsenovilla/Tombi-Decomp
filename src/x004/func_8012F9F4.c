// FUNC 8012f9f4 344 X004
// MATCHING 8012f9f4 344
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012F9F4;

extern T8012F9F4 D_801314B4[];
extern int D_1F8002C8[];

extern void AnimLoadDuration(TObj *);

void func_8012F9F4(TObj *o)
{
    o->box0 = D_801314B4[o->subtype].b0;
    o->box1 = D_801314B4[o->subtype].b1;
    o->box2 = D_801314B4[o->subtype].b2;
    o->box3 = D_801314B4[o->subtype].b3;
    o->w1e = D_801314B4[o->subtype].w4;
    o->d3c = D_1F8002C8[D_801314B4[o->subtype].w6];
    o->anim = D_801314B4[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    *(signed char *)&o->b0f = -6;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
}
