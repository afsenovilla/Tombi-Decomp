// VERSION SLPS_01144 x10 8011BE2C 1108
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"

extern TObj *BITTEN_OBJECT;
extern TObj *D_8009DEE0;
extern volatile unsigned short D_8009F200[];
extern void func_80103138_slps01144(TObj *);
extern void func_80103284_slps01144(TObj *);
extern void func_800EFD70(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern TObj *allocObjectLayer1(void);
extern void playSFXWithNote(int, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void func_801049B4_slps01144(TObj *);

void func_8011BE2C_slps01144(TObj *o)
{
    short f;
    TObj *n;
    Fix16 *p;
    int x;
    volatile unsigned short *k;
    unsigned short new_var;
    unsigned short *np;
    unsigned short m;
    Fix16 *new_var2;

    switch (o->state) {
    case 1:
        o->timer++;
        f = 0;
        o->y.p.whole += 8;
        if (o->y.p.whole + BITTEN_OBJECT->box2 > BITTEN_OBJECT->y.p.whole) {
            o->y.p.whole = BITTEN_OBJECT->y.p.whole - BITTEN_OBJECT->box2;
            f = 1;
        }
        if (o->velX < 0) {
            o->h->p.whole -= 8;
            if (o->h->p.whole <= BITTEN_OBJECT->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (f) {
                    func_80103138_slps01144(o);
                    func_80103284_slps01144(o);
                }
            }
        } else {
            o->h->p.whole += 8;
            if (o->h->p.whole >= BITTEN_OBJECT->h->p.whole) {
                o->h->p.whole = BITTEN_OBJECT->h->p.whole;
                if (f) {
                    func_80103138_slps01144(o);
                    func_80103284_slps01144(o);
                }
            }
        }
        if (o->timer >= 11) {
            func_80103138_slps01144(o);
            func_80103284_slps01144(o);
        }
        break;
    case 2:
        o->h->raw += GVARS.driftH << 8;
        o->y.raw += ((short *)&GVARS.driftY)[0] << 8;
        k = D_8009F200;
        if (*k & 0x8000) o->animFrame = 1;
        if (*k & 0x2000) o->animFrame = 0;
        {
            TObj *b;
            unsigned short t = o->animFrame;
            b = BITTEN_OBJECT;
            new_var = SPAD.s1fc;
            b->animFrame = t & 1;
            b->h->p.whole = o->h->p.whole;
            {
                register short yy asm("$3") = o->y.p.whole; register short bb asm("$5") = b->box2;
                m = SPAD.s3c6;
                yy += bb; b->y.p.whole = yy;
            }
        }
        if (new_var & m) {
            o->velY = (o->velX = (o->wb2 = (o->b69 = 0)));
            if (*k & 0x4000)
                D_8009DEE0->animTimer = 15;
            else
                D_8009DEE0->animTimer = 14;
            if (D_8009DEE0->animFrame != D_8009DEE0->animTimer) {
                func_800EFD70(o);
                AnimLoadDuration(o);
                D_8009DEE0->animFrame = D_8009DEE0->animTimer;
            }
            o->b9c = 0;
            o->velY = 0;
            o->state = 3;
        }
        if (o->y.p.whole < o->w56 + 16) o->y.p.whole = o->w56 + 16;
        break;
    case 3:
        if (AnimAdvance(o)) {
            n = allocObjectLayer1();
            if (n) {
                n->active = 1;
                n->type = 1;
                n->subtype = 13;
                n->b0c = BITTEN_OBJECT->subtype;
                n->animFrame = o->animFrame & 1;
                if (D_8009F200[0] & 0x4000) n->animFrame |= 2;
                p = n->h;
                n->a.p.whole = o->a.p.whole;
                n->y.p.whole = o->y.p.whole;
                n->b.p.whole = o->b.p.whole;
                x = p->p.whole;
                p->p.whole = (o->animFrame & 1) ? x - 16 : x + 16;
            }
            o->velY = 0;
            playSFXWithNote(34, 35);
            o->velY = 0;
            PlayerSetAnimIfChanged(o, 29);
            o->d8c = 512;
            o->state = 4;
        } else {
            func_801049B4_slps01144(o);
        }
        break;
    case 4:
        BITTEN_OBJECT->b04 = 3;
        BITTEN_OBJECT->animFrame = o->animFrame & 1;
        if (D_8009F200[0] & 0x4000) BITTEN_OBJECT->animFrame |= 2;
        o->b9c = 0;
        *(unsigned char *)&o->wac = 0;
        o->step = 62;
        o->state = 0;
        break;
    }
}
#endif
