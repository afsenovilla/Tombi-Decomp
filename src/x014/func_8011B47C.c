// FUNC 8011b47c 948 X014
// MATCHING 8011b47c 948
#include "TOBJ.H"
#include "raw7.h"
extern TObj *D_8009D2E8;
extern TObj *D_8009C330;
extern volatile unsigned short D_8009D670[];
extern short D_8009C944[], D_8009C946[];
extern unsigned short D_1F8001FC, D_1F8003C6;
extern void FUN_80103628(TObj *);
extern void FUN_80103774(TObj *);
extern void FUN_8003facc(TObj *);
extern void ObjSetAnimFromTable(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_80104cd8(TObj *);

void func_8011B47C(TObj *o)
{
    TObj *q;
    volatile unsigned short *k;
    short f;
    short y;

    switch (o->state) {
    case 1:
        o->timer++;
        o->y.p.whole += 8;
        y = o->y.p.whole;
        f = 0;
        {
            TObj *p = D_8009D2E8;
            if (y + p->box2 > p->y.p.whole) {
                o->y.p.whole = p->y.p.whole - p->box2;
                f = 1;
            }
        }
        if (o->velX < 0) {
            o->h->p.whole -= 8;
            if (o->h->p.whole <= D_8009D2E8->h->p.whole) {
                o->h->p.whole = D_8009D2E8->h->p.whole;
                if (f) {
                    FUN_80103628(o);
                    FUN_80103774(o);
                }
            }
        } else {
            o->h->p.whole += 8;
            if (o->h->p.whole >= D_8009D2E8->h->p.whole) {
                o->h->p.whole = D_8009D2E8->h->p.whole;
                if (f) {
                    FUN_80103628(o);
                    FUN_80103774(o);
                }
            }
        }
        if (o->timer >= 0xb) {
            FUN_80103628(o);
            FUN_80103774(o);
        }
        break;
    case 2:
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        FUN_8003facc(o);
        k = D_8009D670;
        if (*k & 0x80)
            o->animFrame = 1;
        if (*k & 0x20)
            o->animFrame = 0;
        {
            TObj *p = D_8009D2E8;
            p->animFrame = o->animFrame & 1;
            p->h->p.whole = o->h->p.whole;
            p->y.p.whole = o->y.p.whole + p->box2;
        }
        if (D_1F8001FC & D_1F8003C6) {
            o->b69 = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            if (*k & 0x40)
                D_8009C330->animTimer = 0xf;
            else
                D_8009C330->animTimer = 0xe;
            if (D_8009C330->animFrame != D_8009C330->animTimer) {
                D_8009D2E8->state = 7;
                ObjSetAnimFromTable(o);
                AnimLoadDuration(o);
                D_8009C330->animFrame = D_8009C330->animTimer;
            }
            o->b9c = 0;
            o->velY = 0;
            o->state = 3;
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            SfxPlay2(0x22, 0x23);
            o->velY = 0;
            PlayerSetAnimIfChanged(o, 0x1d);
            o->d8c = 0x200;
            o->state = 4;
        } else
            FUN_80104cd8(o);
        break;
    case 4:
        D_8009D2E8->b04 = 2;
        D_8009D2E8->step = 3;
        D_8009D2E8->state = 0;
        q = D_8009D2E8;
        y = o->animFrame & 1;
        q->animFrame = y;
        if (D_8009D670[0] & 0x40)
            q->animFrame = y | 2;
        o->b9c = 0;
        U8(o, 0xac) = 0;
        o->step = 0x3e;
        o->state = 0;
        break;
    }
}
