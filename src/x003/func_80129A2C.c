// FUNC 80129a2c 440 X003
// MATCHING 80129a2c 440
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80129A2C;

extern T80129A2C D_80135D7C[];
extern int D_1F8002C8[];
extern unsigned short D_8009C962[];
extern void AnimLoadDuration(TObj *);

void func_80129A2C(TObj *o)
{
    o->box0 = D_80135D7C[o->subtype].b0;
    o->box1 = D_80135D7C[o->subtype].b1;
    o->box2 = D_80135D7C[o->subtype].b2;
    o->box3 = D_80135D7C[o->subtype].b3;
    o->w1e = D_80135D7C[o->subtype].w4;
    o->d3c = D_1F8002C8[D_80135D7C[o->subtype].w6];
    o->anim = D_80135D7C[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0a = 0;
    if (D_8009C962[0] == 2) {
        o->anim = D_80135D7C[o->subtype].anim[0];
        AnimLoadDuration(o);
        o->active = 10;
        o->b0d = 1;
        o->w08 = 0x784e;
    }
    o->b68 = 0;
    o->b0f = 0;
    o->category |= 0x80;
    o->b04++;
}
