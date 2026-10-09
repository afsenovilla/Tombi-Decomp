// FUNC 8012da68 580 X003
// MATCHING 8012da68 580
#include "TOBJ.H"

extern unsigned short D_8007A3F0[];
extern void AnimAdvance(TObj *);
extern int func_8012D66C(TObj *);
extern TObj *FUN_80018448(void);

static __inline__ short past(TObj *o)
{
    short c;

    if (o->animFrame & 1) c = o->a.p.whole < o->wb6;
    else if (o->y.p.whole < -0x46a) c = o->wb8 - 0x14 < o->a.p.whole;
    else c = o->a.p.whole > o->wb8;
    if (c) return 1;
    return 0;
}

void func_8012DA68(TObj *o)
{
    TObj *e;

    switch (o->state) {
    case 0:
        if (*(unsigned short *)&o->wb4) o->state = 1;
        else o->state = 2;
        break;
    case 1:
        if (--o->timer == -1) {
            o->state = 3;
            o->active = 3;
        }
    case 2:
        if (o->animFrame) o->a.p.whole -= 2;
        else o->a.p.whole += 2;
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        AnimAdvance(o);
        if (func_8012D66C(o) || past(o)) o->animFrame = 1 - o->animFrame;
        break;
    case 3:
        if (o->visible) {
            e = FUN_80018448();
            if (e) {
                e->active = 2;
                e->type = 0x43;
                e->a.p.whole = o->a.p.whole;
                e->y.p.whole = o->y.p.whole + 0x10;
                e->b.p.whole = o->b.p.whole;
            }
        }
        o->subtype = 1;
        o->timer = 0x1c;
        o->box0 = 0xc;
        o->box1 = 0x1c;
        o->box2 = 0xc;
        o->anim = 0;
        o->box3 = 0x18;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
        o->velX = o->animFrame;
        break;
    }
}
