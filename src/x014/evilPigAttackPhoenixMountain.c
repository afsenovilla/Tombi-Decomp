// FUNC 8011f8c0 372 X014
// MATCHING 8011f8c0 372
#include "gvars.h"
#include "TOBJ.H"

typedef struct { short a, b, c, d; } E8;

extern int AnimAdvance(TObj *);
extern void evilPigSetAnimBox(TObj *, int);
extern void evilPigPlayAttackSfx(void);
extern void evilPigPlayAttackSfxB(void);
extern int spawnPhoenixMountainPigShot(TObj *);
extern void evilPigSetBoxByDirection(TObj *);
extern void spawnEvilPigAttackFlash(TObj *);

void evilPigAttackPhoenixMountain(TObj *o)
{
    switch (o->substep) {
    case 0:
        evilPigSetAnimBox(o, 0x14);
        o->substep++;
        evilPigPlayAttackSfx();
        break;
    case 1:
        if (AnimAdvance(o)) {
            evilPigSetAnimBox(o, 0x15);
            o->substep++;
        }
        break;
    case 2:
        if (AnimAdvance(o)) {
            spawnPhoenixMountainPigShot(o);
            GVARS.b58 = 0;
            evilPigSetBoxByDirection(o);
            spawnEvilPigAttackFlash(o);
            o->timer = 0x14;
            o->substep++;
            evilPigPlayAttackSfxB();
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
