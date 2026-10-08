// FUNC 80132f80 724 X000
#include "TOBJ.H"
extern Fix16 *D_800A6078;
extern void *D_8013B18C, *D_8013B180, *D_8013B17C;
extern char D_80077CF4[];
extern int Rand(void);
extern void AnimJump(TObj *, int);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern short func_8004065C(TObj *, short, short, int);
extern void FUN_8001faf4(TObj *);
extern short TileCollideAt(TObj *, short, short);
void func_80132F80(TObj *o)
{
    unsigned short px, hx;
    short near;
    Fix16 *pp = D_800A6078;
    int x, t;
    px = pp->p.whole;
    hx = o->h->p.whole;
    near = (unsigned short)(px - hx + 0x40) < 0x80;
    switch (o->state) {
    case 0:
        o->animFrame = (short)hx < (short)px;
        if (Rand() & 1) {
            o->anim = D_8013B18C;
            AnimJump(o, 1);
            o->movetab = D_80077CF4;
            o->velY = -0x200;
            o->state = 2;
        } else {
            o->anim = D_8013B180;
            AnimLoadDuration(o);
            o->timer = 0x40 >> (near * 5);
            o->state = 1;
        }
        break;
    case 1:
        AnimAdvance(o);
        if (--o->timer == 0) {
            o->state = 0;
        } else if (near) {
            o->state = 0;
        }
        break;
    case 2:
        AnimAdvance(o);
        t = o->h->p.whole;
        if (func_8004065C(o, (o->animFrame & 1) ? t - 8 : t + 8, o->y.p.whole, 0) == 0) {
            FUN_8001faf4(o);
        }
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0) {
            o->anim = D_8013B18C;
            AnimJump(o, 4);
            o->state = 3;
        }
        goto common;
    case 3:
        t = (short)hx;
        if (o->animFrame & 1) x = t - 8;
        else x = t + 8;
        if (func_8004065C(o, x, o->y.p.whole, 0) == 0) {
            FUN_8001faf4(o);
        }
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8) != 0) {
            o->anim = D_8013B17C;
            AnimLoadDuration(o);
            o->state = 0;
        }
    common:
        if (o->b68 == 0 && o->h->p.whole >= 0x49f) {
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
