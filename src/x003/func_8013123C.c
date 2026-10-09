// FUNC 8013123c 884 X003
// MATCHING 8013123c 884
#include "TOBJ.H"

#define W1C(o) (*(unsigned short *)&(o)->category)
#define W1E(o) (*(unsigned short *)&(o)->w1e)
extern TObj D_800A6038;
extern unsigned short D_1F8001F8;
extern unsigned short D_1F800176;
int Rand(void);
TObj *FUN_800183b8(void);
void PoolFree_1F800210(TObj *o);

static __inline__ void spawn(TObj *o, short x, short y)
{
    TObj *e = FUN_800183b8();

    if (e) {
        e->active = 1;
        e->type = 0x28;
        e->b0a = 2;
        e->subtype = o->subtype;
        e->a.p.whole = x;
        e->y.p.whole = y;
        e->b.p.whole = 0;
        e->d90 = (int)&W1C(o);
        W1C(o)++;
    }
}

static __inline__ void spawn2(TObj *o, unsigned short *q)
{
    TObj *e;

    e = FUN_800183b8();
    if (e == 0)
        return;
    e->active = 1;
    e->type = 0x28;
    e->b0a = 2;
    e->subtype = 2;
    e->animFrame = o->timer;
    o->timer = (o->timer + 1) & 1;
    if (e->animFrame == 0) {
        unsigned short t = *q;
        e->y.p.whole = -0x7ee;
        e->a.p.whole = t - 0x40;
    } else {
        unsigned short t = *q;
        e->y.p.whole = -0x85f;
        e->a.p.whole = t + 0x180;
    }
    e->b.p.whole = 0x5a;
    e->d90 = (int)&o->w1e;
    o->w08 = 0xf0;
    o->w1e++;
    o->step++;
}

void func_8013123C(TObj *o)
{
    TObj *p = &D_800A6038;
    switch (o->b04) {
    case 0:
        o->b04++;
        W1C(o) = 0;
        o->w1e = 0;
        o->timer = Rand() & 1;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (p->b.p.whole == 0) {
                o->subtype = 3;
                o->step++;
            } else {
                o->subtype = 2;
                o->step = 3;
            }
            break;
        case 1:
            if (W1C(o) != 0)
                break;
            if (p->b.p.whole != 0) {
                o->step = 0;
                break;
            }
            if ((unsigned short)(p->a.p.whole - 0x1086) < 0xbe) {
                if (D_1F8001F8 & 0xf)
                    break;
                if (!(Rand() & 1))
                    break;
                spawn(o, D_1F800176 - 0x40, -0x6cc);
            } else {
                if ((unsigned short)(p->a.p.whole - 0x1144) >= 0xaf)
                    break;
                if (D_1F8001F8 & 0xf)
                    break;
                if (!(Rand() & 1))
                    break;
                spawn(o, D_1F800176 - 0x40, -0x720);
            }
            o->w08 = 0xf0;
            o->step++;
            break;
        case 2:
        case 4:
            if (--o->w08 == -1) {
                o->step = 0;
            }
            break;
        case 3: {
            unsigned short *q;

            if (W1E(o) >= 2)
                break;
            if (p->b.p.whole != 0x5a) {
                o->step = 0;
                break;
            }
            q = &D_1F800176;
            if ((unsigned short)(*q - 0x10a9) >= 0x73)
                break;
            if (D_1F8001F8 & 0xf)
                break;
            if (!(Rand() & 1))
                break;
            spawn2(o, q);
            break;
        }
        }
        break;
    case 2:
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
