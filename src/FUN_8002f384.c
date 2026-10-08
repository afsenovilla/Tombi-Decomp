// FUNC 8002f384 348 MAIN0
// MATCHING 8002f384 348
#include "TOBJ.H"
extern void **PTR_80079d58;
extern unsigned char DAT_800a6039, DAT_800a6047[];
extern unsigned short DAT_800a6066, DAT_800a604e[];
extern short *DAT_800a6078, *DAT_800a607c[];
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void FUN_8002f384(TObj *o)
{
    unsigned char t;
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
        if (o->b6b > 0x7e) t = o->b6b;
        else t = o->b6b + 8;
        o->b6b = t;
    } else if (o->timer < 0) {
        o->b6b = 0;
        o->b04 = 2;
    } else {
        if (o->b6b == 0) t = 0;
        else t = o->b6b - 8;
        o->b6b = t;
    }
    {
        int x = DAT_800a6078[1];
        o->h->p.whole = (DAT_800a6066 & 1) ? x + 4 : x - 4;
    }
    o->y.p.whole = DAT_800a604e[0] - 8;
    o->d->p.whole = DAT_800a607c[0][1];
    o->b0f = DAT_800a6047[0] - 1;
}
