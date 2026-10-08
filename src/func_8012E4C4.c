// FUNC 8012e4c4 1024 X000
// MATCHING 8012e4c4 1024
#include "TOBJ.H"
typedef struct { TObj t; char pad[0xec - 0xc0]; } E;
extern TObj D_800A6038;
extern E D_800B1478[];
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_1F80019C;
extern int func_800203DC(TObj *);
extern void FUN_8011aa30(int, int, int);
extern void SfxPlay2(int, int);
extern void FUN_80020d20(int, int, int, int);
extern void FUN_80018790(TObj *);

static __inline__ int Hit(TObj *o, TObj *p)
{
    if (o->subtype && (unsigned short)(p->b.p.whole - o->b.p.whole + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)(p->a.p.whole - o->a.p.whole + (p->box0 + o->box0)) > p->box1 + o->box1)
        return 0;
    if ((unsigned short)(p->y.p.whole - o->y.p.whole + (o->box2 + p->box2)) > o->box3 + p->box3)
        return 0;
    o->b68 = 1;
    return 1;
}

static __inline__ void Act(TObj *o)
{
    SfxPlay2(0x39, 10);
    if (o->subtype == 0) {
        FUN_80020d20(0x16, 0x154, -0xbe, 0);
        FUN_80020d20(0x16, 0x17c, -0xbe, 0);
    } else {
        FUN_80020d20(0x16, 0x352, -0x96, 0x5a);
    }
    o->timer = 0x78;
    o->step++;
}

void func_8012E4C4(TObj *o)
{
    TObj *pl;
    E *e;
    switch (o->b04) {
    case 0:
        o->box0 = 0x19;
        o->box1 = 0x32;
        o->box2 = 0x19;
        o->box3 = 0x32;
        o->b04++;
        break;
    case 1:
        pl = &D_800A6038;
        switch (o->step) {
        case 0:
            if (func_800203DC(o) == 0)
                break;
            if ((D_1F8001F8 + D_1F800198) & 1) {
                if (Hit(o, pl)) {
                    FUN_8011aa30(pl->a.p.whole, (short)(pl->y.p.whole - 0x10), pl->b.p.whole);
                    Act(o);
                }
            } else {
                e = D_800B1478;
                for (D_1F80019C = 0; D_1F80019C < 4; D_1F80019C++, e++) {
                    if (e->t.active == 1 && Hit(o, &e->t)) {
                        FUN_8011aa30(e->t.a.p.whole, e->t.y.p.whole, e->t.b.p.whole);
                        Act(o);
                        break;
                    }
                }
            }
            break;
        case 1:
            if (--o->timer == -1)
                o->step = 0;
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
