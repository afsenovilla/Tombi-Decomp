// FUNC 80118048 716 X014
// MATCHING 80118048 716
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } E8;
extern int FUN_800202b4(TObj *);

#define E(i) (((E8 *)&o->wb4)[i])

void func_80118048(TObj *o)
{
    E8 *p0 = &E(0);
    E8 *p1 = &E(1);
    E8 *p2 = &E(2);
    E8 *p3 = &E(3);

    switch (o->step) {
    case 0:
        if (--o->timer != -1) return;
        E(0).y = -90;
        E(1).y = -75;
        E(1).x = 2;
        E(2).y = -75;
        E(2).x = -2;
        o->w74 = 0;
        o->w76 = 0;
        o->w78 = 0;
        o->w7a = 0;
        E(0).x = 0;
        E(0).z = 0;
        E(1).z = 0;
        E(2).z = 0;
        E(3).x = 0;
        E(3).y = -45;
        E(3).z = 0;
        o->step++;
        return;
    case 1:
        switch (o->state) {
        case 0:
        case 4:
            o->timer = 4;
            o->state++;
            break;
        case 1:
        case 5:
            if (--o->timer == -1) {
                o->timer = 11;
                o->state++;
            }
            p0->y += 6;
            p1->y += 5;
            p2->y += 5;
            p3->y += 3;
            o->w76 += 0x18;
            o->w78 += 0x18;
            o->w7a += 0x40;
            if (o->w76 > 0x40) o->w76 = 0x40;
            if (o->w78 > 0x40) o->w78 = 0x40;
            if (o->w7a > 0xf0) o->w7a = 0xf0;
            break;
        case 2:
        case 6:
            if (--o->timer == -1) o->state++;
            p0->y += 6;
            p1->y += 5;
            p2->y += 5;
            p3->y += 3;
            break;
        case 3:
        case 7:
            o->w76 = 0;
            o->w78 = 0;
            o->w7a = 0;
            o->state++;
            o->animFrame = (o->animFrame + 12) & 0xff;
            o->d8c = o->animFrame << 4;
            p0->y = -90;
            p1->y = -75;
            p2->y = -75;
            p3->y = -45;
            break;
        case 8:
            p0->y = 0;
            p1->y = 0;
            p2->y = 0;
            p3->y = 0;
            o->b04 = 3;
            break;
        }
        FUN_800202b4(o);
        break;
    }
}
