// FUNC 80117e04 348 X016
// MATCHING 80117e04 348
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80117E04;

extern T80117E04 D_80118D14[];
extern int D_1F8002C8[];

extern void AnimLoadDuration(TObj *);

void func_80117E04(TObj *o)
{
    o->box0 = D_80118D14[o->subtype].b0;
    o->box1 = D_80118D14[o->subtype].b1;
    o->box2 = D_80118D14[o->subtype].b2;
    o->box3 = D_80118D14[o->subtype].b3;
    o->w1e = D_80118D14[o->subtype].w4;
    o->d3c = D_1F8002C8[D_80118D14[o->subtype].w6];
    o->anim = D_80118D14[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0a = 0;
    o->b0f = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
}
