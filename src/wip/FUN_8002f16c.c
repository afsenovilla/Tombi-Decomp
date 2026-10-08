// FUNC 8002f16c 536 MAIN0
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

void FUN_8002f16c(TObj *o)
{
    unsigned char b;
    short s;
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
        v = o->b6b;
        b = v + 8;
        if (v > 0x7e) b = v;
    } else {
        if (o->timer < 0) {
            DAT_800a6038 = 3;
            if (DAT_800a6118 < 1) DAT_800a6038 = 1;
            DAT_800a6104 = 3;
            o->b6b = 0;
            o->b04 = 2;
            goto L;
        }
        b = o->b6b - 2;
        if (o->b6b == 0) b = 0;
    }
    o->b6b = b;
L:
    s = 4;
    if ((DAT_800a6066 & 1) == 0) s = -4;
    o->h->raw = *(short *)((char *)DAT_800a6078 + 2) + s;
    o->y.raw = DAT_800a604e - 8;
    o->d->raw = *(short *)((char *)DAT_800a607c + 2);
    if (o->b0c == 0) {
        b = DAT_800a6047 + 1;
    } else if (o->b0c == 1) {
        b = DAT_800a6047 - 1;
    } else {
        return;
    }
    o->b0f = b;
}
