// FUNC 80129914 392 X004
// MATCHING 80129914 392
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80129914;

extern T80129914 D_80131180[];
extern int D_1F8002C8[];
extern unsigned char D_8009CE41;
extern short FUN_8005e420(int, int);
extern void AnimLoadDuration(TObj *);

void func_80129914(TObj *o)
{
    o->box0 = D_80131180[o->subtype].b0;
    o->box1 = D_80131180[o->subtype].b1;
    o->box2 = D_80131180[o->subtype].b2;
    o->box3 = D_80131180[o->subtype].b3;
    o->w1e = D_80131180[o->subtype].w4;
    o->d3c = D_1F8002C8[D_80131180[o->subtype].w6];
    o->anim = D_80131180[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 1;
    o->w08 = FUN_8005e420(0x120, 0x1e0);
    o->b0a = 0;
    o->b0f = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
    if (D_8009CE41 != 1) o->b04 = 3;
}
