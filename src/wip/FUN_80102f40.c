// FUNC 80102f40 700 X000
/* score 50 (was 99): h snap/+-8 layout per game (goto snap, +8 out of line), short flag, o->h->whole = P->h->whole fix, byte wac store, buf[16] for the 0x40 frame. Left: hw read twice (lh+lhu) where game copies (move a2,v1), P pointer reg in the type test, load order of o->h vs P->h. */
#include "TOBJ.H"
extern TObj *DAT_8009d2e8;
extern unsigned short DAT_8009d670;
extern unsigned char DAT_8009d2b0;
extern unsigned short DAT_1f8001fc;
extern unsigned short DAT_1f8003c6;
extern void FUN_800ee680(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_800ee4e0(TObj *, int);

void FUN_80102f40(TObj *o)
{
    short flag;
    TObj *p;
    Fix16 *h;
    int t; short hw; char buf[16];
    volatile unsigned short *pad;

    switch (o->state) {
    case 1:
        o->timer++;
        if (DAT_8009d2e8->type == 3 || DAT_8009d2e8->type == 0x1f)
            DAT_8009d2e8->d8c = 0;
        FUN_800ee680(o);
        flag = 0;
        if (o->y.p.whole + DAT_8009d2e8->box2 >= DAT_8009d2e8->y.p.whole) {
            flag = 1;
            DAT_8009d2e8->y.p.whole = o->y.p.whole + o->wba;
        } else {
            o->y.p.whole += 8;
        }
        if (o->velX < 0) {
            h = o->h;
            t = DAT_8009d2e8->h->p.whole;
            hw = h->p.whole;
            if (t < hw)
                h->p.whole = hw - 8;
            else
                goto snap;
        } else {
            h = o->h;
            t = DAT_8009d2e8->h->p.whole;
            hw = h->p.whole;
            if (t <= hw) {
            snap:
                h->p.whole = t;
                if (flag)
                    FUN_800eeb5c(o, 0x23);
            } else {
                h->p.whole = hw + 8;
            }
        }
    next:
        if (DAT_8009d2e8->b69 || o->b69 || FUN_8003fd78(o, 0, 1) || o->timer > 0x14) {
            o->timer = 0;
            FUN_800eeb5c(o, 0x23);
            *(unsigned char *)&o->wac = 3;
            o->b9c = 0;
            o->velX = 0;
            o->velY = 0;
            DAT_8009d2e8->animFrame = o->animFrame & 1;
            o->h->p.whole = DAT_8009d2e8->h->p.whole;
            DAT_8009d2e8->y.p.whole = o->y.p.whole + o->wba;
            FUN_800ee4e0(o, 0);
            o->state = 2;
        }
        break;
    case 2:
        pad = &DAT_8009d670;
        if (*pad & 0x80)
            o->animFrame = 1;
        if (*pad & 0x20)
            o->animFrame = 0;
        o->h->p.whole = DAT_8009d2e8->h->p.whole;
        DAT_8009d2e8->y.p.whole = o->y.p.whole + o->wba;
        if (DAT_1f8003c6 & DAT_1f8001fc) {
            DAT_8009d2b0 = 0;
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            o->step = 0xf;
            o->state = 0;
        }
        break;
    }
}
