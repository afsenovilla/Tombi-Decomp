// FUNC 80124bbc 340 X000
/* score 40 without FLAGS (b39; old wip was 40 only with -fno-schedule-insns). Block 2/3 written like the matched twin
   FUN_80124a84 (sm then e). The block-local `{ int t = dy + o->box0; uy += t; }` is what keeps p in a1 and o in a3
   (otherwise sched1 hoists o->box0 and a1 becomes a temp, p copied to t0). Left: game adds the UNextended
   (ph - p->box0) to lhu o->box0, ours adds sign-extended dy + lh o->box0; plus regs in block 2. Tried (u16) casts on
   t's operands (adds andi), int/ushort d temps (back to 77), ten orderings of the uy statements. */
#include "TOBJ.H"

int FUN_80124bbc(TObj *o, TObj *p)
{
    short dy;
    short e;
    short s;
    unsigned short h;
    unsigned short uy;
    unsigned short sm;
    int lim;
    Fix16 *oh = o->h, *ph = p->h;
    unsigned short u6 = oh->p.whole;
    unsigned short u5 = ph->p.whole;

    uy = u6 - u5 + 6;
    dy = u5 - p->box0;
    { int t = dy + o->box0; uy += t; }
    lim = o->box1 + 0xe;
    if (uy > dy + lim)
        return 0;
    sm = o->box2 + (o->y.p.whole - p->y.p.whole);
    e = p->box2 - p->y.p.whole;
    if (sm > e + o->box3)
        return 0;
    s = o->y.p.whole + o->box2 - p->y.p.whole;
    if (s <= 0)
        h = 0;
    else
        h = s * dy / e;
    if (p->h->p.whole - (short)h > o->h->p.whole + o->box0)
        return 0;
    o->h->p.whole = p->h->p.whole - h - o->box0;
    o->h->p.frac = 0;
    return 1;
}
