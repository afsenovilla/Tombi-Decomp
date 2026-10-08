// FUNC 8012bd5c 384 X003
// MATCHING 8012bd5c 384
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8012BD5C;

extern T8012BD5C D_80135DD0[];
extern int D_1F8002C8[];
extern unsigned char D_800A6047, D_8009CE5D;
extern void AnimLoadDuration(TObj *);

void func_8012BD5C(TObj *o)
{
    o->box0 = D_80135DD0[o->subtype].b0;
    o->box1 = D_80135DD0[o->subtype].b1;
    o->box2 = D_80135DD0[o->subtype].b2;
    o->box3 = D_80135DD0[o->subtype].b3;
    o->w1e = D_80135DD0[o->subtype].w4;
    o->d3c = D_1F8002C8[D_80135DD0[o->subtype].w6];
    o->anim = D_80135DD0[o->subtype].anim[0];
    AnimLoadDuration(o);
    o->b0a = 2;
    o->d8c = 0;
    o->b0d = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = D_800A6047 + 1;
    o->b04++;
    *(int *)((char *)o + 0x88) = 0;
    if (D_8009CE5D == 0xff) o->b04 = 3;
}
