// FUNC 8012049c 412 X014
// MATCHING 8012049c 412
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"

typedef struct { int a; short t; short b; } E8;
typedef struct { short w[8]; } B4;

extern int AnimAdvance(TObj *o);
extern void evilPigSetAnimBox(TObj *o, int n);
extern void evilPigSetBoxByDirection(TObj *o);
extern void spawnEvilPigAttackFlash(TObj *o);
extern void spawnRealEvilPigEffect(TObj *o);
extern void evilPigPlayAttackSfx(void);
extern void evilPigPlayAttackSfxB(void);

void evilPigAttackRealEvilPig(TObj *o)
{
    B4 *b = (B4 *)&o->wb4;

    switch (o->substep) {
    case 0:
        o->active = 2;
        GV2.bb0 = 0;
        GVARS.b3e = 1;
        GVARS.b42 = 1;
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
            spawnRealEvilPigEffect(o);
            GVARS.b58 = 0;
            evilPigSetBoxByDirection(o);
            spawnEvilPigAttackFlash(o);
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
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            b->w[7] = 1;
        }
        break;
    }
}
