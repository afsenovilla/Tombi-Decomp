// FUNC 8012d740 468 X003
// MATCHING 8012d740 468
#include "TOBJ.H"

extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
extern unsigned short D_80135E20[];
extern int FUN_8001f9e0(void);
extern int FUN_80020448(short, short);
extern TObj *FUN_80018448(void);

static __inline__ int inview(TObj *o)
{
    if ((unsigned short)(o->a.p.whole - D_1F800176 + 0x64) >= 0x209) return 0;
    return (unsigned short)(D_1F800186 - o->y.p.whole + 0x80) < 0x1e1;
}

void func_8012D740(TObj *o)
{
    TObj *e;

    switch (o->state) {
    case 0:
        if (((D_1F8001F8 + D_1F800198) & 3) == 0 && inview(o)) {
            o->timer = D_80135E20[FUN_8001f9e0() & 0xf];
            o->state++;
        }
        break;
    case 1:
        if (--o->timer == -1) {
            if ((o->subtype = FUN_8001f9e0() & 1) == 0) {
                o->box0 = 8;
                o->box1 = 0x10;
                o->box2 = 8;
                o->box3 = 0x10;
            } else {
                o->box0 = 0xc;
                o->box1 = 0x1c;
                o->box2 = 0xc;
                o->box3 = 0x18;
            }
            o->timer = 0x1c;
            o->b04++;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            o->anim = 0;
            if (FUN_80020448(o->a.p.whole, o->y.p.whole)) {
                e = FUN_80018448();
                if (e) {
                    e->active = 2;
                    e->type = 0x43;
                    e->a.p.whole = o->a.p.whole;
                    e->y.p.whole = o->y.p.whole + 0x10;
                    e->b.p.whole = o->b.p.whole;
                }
            }
        }
        break;
    }
}
