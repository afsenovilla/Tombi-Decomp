// FUNC 80102f40 700 X000
/* score 46 (b30, was 50): h block rewritten with a shared h pointer and t (P->h x) read once, compares on h->p.whole
   (no buf pad needed, frame 0x40 comes out). Left: (1) P/type regs swapped at the top (game P=v1,type=a0; global-alloc
   priority: type 3 refs/5 insns beats P 3 refs/9; local p, inline, switch, temps all unchanged); (2) t read as
   lhu+lh where game does lh + move a0 (and hw copy in a2). Tried: int/short t,hw,x/y temps, assignments in the
   compare, direct field forms (copy appears for hw but snap then reloads). */
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
    short t; 
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
            if (t < h->p.whole)
                h->p.whole -= 8;
            else
                goto snap;
        } else {
            h = o->h;
            t = DAT_8009d2e8->h->p.whole;
            if (h->p.whole >= t) {
            snap:
                h->p.whole = t;
                if (flag)
                    FUN_800eeb5c(o, 0x23);
            } else {
                h->p.whole += 8;
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
