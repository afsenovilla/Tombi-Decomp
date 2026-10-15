// FUNC 80120330 364 X014
// MATCHING 80120330 364
#include "TOBJ.H"

typedef struct { int a; short t; short b; } E8;

extern int AnimAdvance(TObj *o);
extern void evilPigSetAnimBox(TObj *o, int n);
extern void evilPigSetBoxByDirection(TObj *o);
extern void spawnEvilPigAttackFlash(TObj *o);
extern void evilPigPlayAttackSfx(void);
extern void evilPigPlayAttackSfxB(void);
extern void spawnHauntedMansionPigShot(TObj *o);

void evilPigAttackHauntedMansion(TObj *o)
{
    switch (o->substep) {
    case 0:
        evilPigSetAnimBox(o, 0x14);
        o->substep++;
        evilPigPlayAttackSfx();
        spawnHauntedMansionPigShot(o);
        break;
    case 1:
        if (AnimAdvance(o)) {
            evilPigSetAnimBox(o, 0x15);
            o->substep++;
        }
        break;
    case 2:
        if (AnimAdvance(o)) {
            evilPigSetBoxByDirection(o);
            spawnEvilPigAttackFlash(o);
            o->timer = 0x14;
            o->substep++;
            evilPigPlayAttackSfxB();
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            E8 *e = (E8 *)o->d94;
            o->substep++;
            e += o->wae;
            o->timer = e->t;
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
