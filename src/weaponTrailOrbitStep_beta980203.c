// VERSION BETA_980203 x00 800F15DC 528
#if defined(VERSION_BETA_980203)
// Beta version: written from the disc (PAL weaponTrailOrbitStep.c in an earlier form): no start-angle table, no sparks, a fixed 0x40/0xC0 orbit angle and radius 0x20/0x10.
#include "TOBJ.H"
typedef struct { short k; short w1e; void **anims; int pad2; } T12;
extern T12 D_8011846C[];
extern unsigned char D_800ABB21;
extern unsigned char D_800ABB2F;
extern void AnimLoadDuration(TObj *);
extern void func_80022350(TObj *);
extern int func_8002223C(int, int);
extern int func_8002226C(int, int);

void weaponTrailOrbitStep(TObj *o)
{
    TObj *q;
    short r;
    short a;

    switch (o->state) {
    case 0:
        o->anim = *D_8011846C[o->subtype].anims;
        AnimLoadDuration(o);
        o->visible = D_800ABB21;
        o->b0f = D_800ABB2F - 1;
        {
            short t = 0x20;
            if (o->b0c & 2) t = 0x10;
            o->velX = t;
        }
        o->velY = 0;
        o->state++;
    case 1:
        func_80022350(o);
        q = (TObj *)o->d90;
        r = 0xc0;
        if (q->waa + (o->b0c & 1)) r = 0x40;
        a = func_8002223C(o->velY, o->velX);
        o->velH = func_8002226C(r, a);
        o->velV = func_8002223C(r, a);
        o->h->p.whole = q->h->p.whole + o->velH;
        o->y.p.whole = q->y.p.whole + o->velV;
        o->d->p.whole = q->d->p.whole;
        a = (q->waa + 0x80) & 0xff;
        o->velH = func_8002226C(a, 0x10);
        o->velV = func_8002223C(a, 0x10);
        o->h->p.whole += o->velH;
        o->y.p.whole += o->velV;
        o->velY = (o->velY + 0x10) & 0xff;
        switch (q->step) {
        case 0 ... 6:
            break;
        default:
            o->b04 = 2;
        }
        if (q->b04 != 1)
            o->b04 = 2;
        break;
    }
}
#endif
