// FUNC 8011f8c0 372 X014
// MATCHING 8011f8c0 372
#include "gvars.h"
#include "TOBJ.H"

typedef struct { short a, b, c, d; } E8;

extern int AnimAdvance(TObj *);
extern void evilPigSetAnimBox(TObj *, int);
extern void func_8011F850(void);
extern void func_8011F888(void);
extern int func_80122F18(TObj *);
extern void evilPigSetBoxByDirection(TObj *);
extern void func_80116C9C(TObj *);

void evilPigAttackPhoenixMountain(TObj *o)
{
    switch (o->substep) {
    case 0:
        evilPigSetAnimBox(o, 0x14);
        o->substep++;
        func_8011F850();
        break;
    case 1:
        if (AnimAdvance(o)) {
            evilPigSetAnimBox(o, 0x15);
            o->substep++;
        }
        break;
    case 2:
        if (AnimAdvance(o)) {
            func_80122F18(o);
            GVARS.b58 = 0;
            evilPigSetBoxByDirection(o);
            func_80116C9C(o);
            o->timer = 0x14;
            o->substep++;
            func_8011F888();
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            E8 *t = (E8 *)o->d94;
            o->substep++;
            t += o->wae;
            o->timer = t->c;
            evilPigSetAnimBox(o, 0x12);
        }
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->state = 0;
            o->substep = 0;
            o->step++;
        }
        break;
    }
}
