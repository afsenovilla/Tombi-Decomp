// FUNC 80116f7c 376 X018
// MATCHING 80116f7c 376
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80116F7C;

extern T80116F7C D_8011A4B0[];
extern int D_1F8002C8[];
extern unsigned char D_8009CDED;
extern void AnimLoadDuration(TObj *);

void func_80116F7C(TObj *o)
{
    o->box0 = D_8011A4B0[o->subtype].b0;
    o->box1 = D_8011A4B0[o->subtype].b1;
    o->box2 = D_8011A4B0[o->subtype].b2;
    o->box3 = D_8011A4B0[o->subtype].b3;
    o->w1e = D_8011A4B0[o->subtype].w4;
    o->d3c = D_1F8002C8[D_8011A4B0[o->subtype].w6];
    o->anim = D_8011A4B0[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    *(signed char *)&o->b0f = -4;
    o->d8c = 0;
    o->b0d = 0;
    o->b0a = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
    if (D_8009CDED != 0xff) o->b04 = 3;
}
