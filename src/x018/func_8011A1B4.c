// FUNC 8011a1b4 376 X018
// MATCHING 8011a1b4 376
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T8011A1B4;

extern T8011A1B4 D_8011A55C[];
extern int D_1F8002C8[];
extern unsigned char D_800A6047;
extern void AnimLoadDuration(TObj *);

void func_8011A1B4(TObj *o)
{
    o->box0 = D_8011A55C[o->subtype].b0;
    o->box1 = D_8011A55C[o->subtype].b1;
    o->box2 = D_8011A55C[o->subtype].b2;
    o->box3 = D_8011A55C[o->subtype].b3;
    o->w1e = D_8011A55C[o->subtype].w4;
    o->d3c = D_1F8002C8[D_8011A55C[o->subtype].w6];
    o->anim = D_8011A55C[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->b0d = 1;
    o->w08 = 0x780c;
    o->b0a = 2;
    o->d8c = 0;
    o->b68 = 0;
    o->category |= 0x80;
    o->b0f = D_800A6047 + 1;
    o->b04++;
}
