// FUNC 8002f16c 536 MAIN0
// MATCHING 8002f16c 536
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_80025f40(int, int, int, int);
extern char *PTR_80079d58[];
extern unsigned short DAT_1f8001f8;
extern unsigned char DAT_800a6039, DAT_800a6047;
extern char DAT_800a6038, DAT_800a6104;
extern short DAT_800a6118;
extern unsigned short DAT_800a6066;
extern short *DAT_800a6078;
extern short *DAT_800a607c;
extern short DAT_800a604e;

void func_8002F16C(TObj *o)
{
    unsigned char b;
    int s;
    Fix16 *f;
    short t;
    unsigned char v;
    switch (o->state) {
    case 0:
        o->anim = *(void **)(PTR_80079d58[o->subtype * 3] + o->b0c * 4);
        AnimLoadDuration(o);
        o->timer = 0xb4;
        o->state = o->state + 1;
    case 1:
        break;
    default:
        return;
    }
    if (o->b0c == 0 && (DAT_1f8001f8 & 3) == 0)
        FUN_80025f40(0, 0, 0xff, 2);
    AnimAdvance(o);
    o->timer = o->timer - 1;
    o->visible = DAT_800a6039;
    if (o->timer >= 0x3d) {
        if (o->b6b < 0x7f)
            b = o->b6b + 8;
        else
            b = o->b6b;
    } else {
        if (o->timer < 0) {
            if (DAT_800a6118 > 0)
                DAT_800a6038 = 3;
            else
                DAT_800a6038 = 1;
            DAT_800a6104 = 3;
            o->b6b = 0;
            o->b04 = 2;
            goto L;
        }
        if (o->b6b != 0)
            b = o->b6b - 2;
        else
            b = 0;
    }
    o->b6b = b;
L:
    s = DAT_800a6078[1];
    f = o->h;
    if (DAT_800a6066 & 1)
        f->p.whole = s + 4;
    else
        f->p.whole = s - 4;
    o->y.p.whole = DAT_800a604e - 8;
    o->d->p.whole = DAT_800a607c[1];
    switch (o->b0c) {
    case 0:
        b = DAT_800a6047 + 1;
        break;
    case 1:
        b = DAT_800a6047 - 1;
        break;
    default:
        return;
    }
    o->b0f = b;
}
