// FUNC 80126f48 356 X010
// MATCHING 80126f48 356
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80126F48;

extern T80126F48 D_8012F3BC[];
extern int D_1F8002C8[];
extern unsigned char D_800A6047;
extern void AnimLoadDuration(TObj *);

void func_80126F48(TObj *o)
{
    o->box0 = D_8012F3BC[o->subtype].b0;
    o->box1 = D_8012F3BC[o->subtype].b1;
    o->box2 = D_8012F3BC[o->subtype].b2;
    o->box3 = D_8012F3BC[o->subtype].b3;
    o->w1e = D_8012F3BC[o->subtype].w4;
    o->d3c = D_1F8002C8[D_8012F3BC[o->subtype].w6];
    o->anim = D_8012F3BC[o->subtype].anim[0];
    AnimLoadDuration(o);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = D_800A6047 + 10;
    o->b04++;
}
