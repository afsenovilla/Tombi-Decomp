// FUNC 80117e9c 376 X017
// MATCHING 80117e9c 376
#include "gcd.h"
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3;
    unsigned short w4;
    short w6;
    void **anim;
} T80117E9C;

extern T80117E9C D_80119988[];
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);

void initWaterfallHintNpc(TObj *o)
{
    o->box0 = D_80119988[o->subtype].b0;
    o->box1 = D_80119988[o->subtype].b1;
    o->box2 = D_80119988[o->subtype].b2;
    o->box3 = D_80119988[o->subtype].b3;
    o->w1e = D_80119988[o->subtype].w4;
    o->d3c = SPR_DATA[D_80119988[o->subtype].w6];
    o->anim = D_80119988[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0f = 0;
    o->b0a = 0;
    o->category |= 0x80;
    o->b04++;
    if ((unsigned short)o->wba == 0 && GCD.bcded == 0xff) o->b04 = 3;
}
