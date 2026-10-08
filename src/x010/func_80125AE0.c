// FUNC 80125ae0 368 X010
// MATCHING 80125ae0 368
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80125AE0;

extern T80125AE0 D_8012F3B0[];
extern int D_1F8002C8[];
extern unsigned char D_800A6047;
extern void AnimJump(TObj *, int);
extern void AnimLoadDuration(TObj *);

void func_80125AE0(TObj *o)
{
    o->box0 = D_8012F3B0[o->subtype].b0;
    o->box1 = D_8012F3B0[o->subtype].b1;
    o->box2 = D_8012F3B0[o->subtype].b2;
    o->box3 = D_8012F3B0[o->subtype].b3;
    o->w1e = D_8012F3B0[o->subtype].w4;
    o->d3c = D_1F8002C8[D_8012F3B0[o->subtype].w6];
    AnimLoadDuration(o);
    o->anim = D_8012F3B0[o->subtype].anim[0];
    AnimJump(o, 0);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = D_800A6047 + 1;
    o->b04++;
}
