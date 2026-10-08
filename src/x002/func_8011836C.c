// FUNC 8011836c 456 X002
// MATCHING 8011836c 456
#include "TOBJ.H"
typedef struct { unsigned char b0, b1, b2, b3; unsigned short w4; short idx; void **anims; } T12;
extern T12 D_8011C770[];
extern int D_1F8002C8[];
extern int D_1F800304[];
extern void FUN_8001fe6c(TObj *);
extern short FUN_8005e420(int, int);

void func_8011836C(TObj *o)
{
    o->box0 = D_8011C770[o->subtype].b0;
    o->box1 = D_8011C770[o->subtype].b1;
    o->box2 = D_8011C770[o->subtype].b2;
    o->box3 = D_8011C770[o->subtype].b3;
    o->w1e = D_8011C770[o->subtype].w4;
    o->d3c = D_1F8002C8[D_8011C770[o->subtype].idx];
    o->anim = D_8011C770[o->subtype].anims[(unsigned short)o->wb4];
    FUN_8001fe6c(o);
    o->d8c = 0;
    o->b0d = 0;
    if (o->subtype == 9) {
        o->b0d = 1;
        o->w08 = FUN_8005e420(0x90, 0x1f0);
    }
    if (o->subtype == 1) {
        o->b0d = 0x81;
        o->w08 = FUN_8005e420(0x80, 0x1f9);
        o->d3c = D_1F800304[0];
    }
    if (o->subtype != 0) o->active = 2;
    o->b0a = 0;
    o->b0f = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
}
