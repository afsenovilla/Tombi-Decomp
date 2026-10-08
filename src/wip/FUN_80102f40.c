// FUNC 80102f40 700 X000
/* score 38 (b44, was 50): P/type swap at the top fixed by `do { P->d8c = 0; } while (0)` (loop note adds one weighted
   ref to P: global-alloc priority 2*4/9 > type 1*3/5). Left: t read as lhu+lh, game does one lh + `move a0` copy (and hw
   copy in a2); also frame 0x40 comes from orphan (use) pseudos of combine (short vars), keep them. Tried: int temps
   a2 = P->h->p.whole; t = a2; with compares on a2 (gives both copies, swapped regs, frame 48), assignment-in-compare,
   (int)/(short) casts, h/hw/t types. combine's PARALLEL split (lh + lowpart copy) needs a dest without nonzero_bits. b48: confirmed - t gets lh+move (game shape) when t is live at function start (reg_nonzero_bits stays 0): `__asm__("" : : "r"(t), "r"(hw));` before the switch with `short hw = h->p.whole` vars gives score 18 (only h/t swapped a0/a1), but live-at-start makes t conflict with a0 (incoming o), so the game cannot have done that: look for another way to get the split (t set from a source with full-word nonzero_bits?). */
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
            do { DAT_8009d2e8->d8c = 0; } while (0);
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
