// FUNC 8012b2e4 368 X003
// MATCHING 8012b2e4 368
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012B2E4;

extern T8012B2E4 D_80135DC4[];
extern int D_1F8002C8[];
extern unsigned char D_800A6047;
extern void AnimJump(TObj *, int);
extern void AnimLoadDuration(TObj *);

void func_8012B2E4(TObj *o)
{
    o->box0 = D_80135DC4[o->subtype].b0;
    o->box1 = D_80135DC4[o->subtype].b1;
    o->box2 = D_80135DC4[o->subtype].b2;
    o->box3 = D_80135DC4[o->subtype].b3;
    o->w1e = D_80135DC4[o->subtype].w4;
    o->d3c = D_1F8002C8[D_80135DC4[o->subtype].w6];
    AnimLoadDuration(o);
    o->anim = D_80135DC4[o->subtype].anim[4];
    AnimJump(o, 0);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = D_800A6047 + 1;
    o->b04++;
}
