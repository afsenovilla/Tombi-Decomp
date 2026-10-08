// FUNC 8002f384 348 MAIN0
#include "TOBJ.H"
extern void **PTR_80079d58;
extern unsigned char DAT_800a6039, DAT_800a6047;
extern unsigned short DAT_800a6066, DAT_800a604e;
extern short *DAT_800a6078, *DAT_800a607c;
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void FUN_8002f384(TObj *o)
{
    unsigned char t;
    short sv;
    switch (o->state) {
    case 0:
        o->anim = *PTR_80079d58;
        AnimLoadDuration(o);
        o->timer = 0x3c;
        o->state++;
    case 1:
        break;
    default:
        return;
    }
    AnimAdvance(o);
    o->timer = o->timer - 1;
    o->visible = DAT_800a6039;
    if (o->timer > 0x20) {
        t = o->b6b + 8;
        if (o->b6b > 0x7e) t = o->b6b;
        o->b6b = t;
    } else if (o->timer < 0) {
        o->b6b = 0;
        o->b04 = 2;
    } else {
        t = o->b6b - 8;
        if (o->b6b == 0) t = 0;
        o->b6b = t;
    }
    sv = 4;
    if ((DAT_800a6066 & 1) == 0) sv = -4;
    o->h->p.whole = DAT_800a6078[1] + sv;
    o->y.p.whole = DAT_800a604e - 8;
    o->d->p.whole = DAT_800a607c[1];
    o->b0f = DAT_800a6047 - 1;
}
