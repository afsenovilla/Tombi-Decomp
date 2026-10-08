// FUNC 8011a498 356 X014
// MATCHING 8011a498 356
#include "TOBJ.H"
typedef struct { short a, b; } P2;
extern P2 D_80125C90[];
extern unsigned short D_8009C962;

void func_8011A498(TObj *o)
{
    char pad;

    switch (o->substep) {
    case 0:
        switch (D_8009C962) {
        case 1:
            o->w74 = D_80125C90[2].a;
            o->w76 = D_80125C90[2].b;
            break;
        case 5:
            o->w74 = D_80125C90[0].a;
            o->w76 = D_80125C90[0].b;
            break;
        case 7:
            o->w74 = D_80125C90[1].a;
            o->w76 = D_80125C90[1].b;
            break;
        }
        o->substep++;
        break;
    case 1:
        o->y.p.whole++;
        if (o->y.p.whole > o->w74) {
            o->y.p.whole = o->w74;
            o->substep++;
        }
        break;
    case 2:
        o->y.p.whole--;
        if (o->y.p.whole < o->w76) {
            o->y.p.whole = o->w76;
            o->substep = 1;
        }
        break;
    }
}
