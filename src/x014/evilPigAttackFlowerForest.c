// FUNC 8011ff80 480 X014
// MATCHING 8011ff80 480
#include "TOBJ.H"

typedef struct { int a; short t; short b; } E8;
typedef struct { short x0, x2, x4, x6; unsigned short n; short k; } Q;

extern unsigned char D_801262E0[];
extern int Rand(void);
extern int AnimAdvance(TObj *o);
extern void ObjSetFacingToPlayer(TObj *o);
extern void evilPigSetAnimBox(TObj *o, int n);
extern void evilPigSetBoxByDirection(TObj *o);
extern void func_80116C9C(TObj *o);
extern void func_8011F850(void);
extern void func_8011F888(void);
extern void spawnFlowerForestPigShot(TObj *o, unsigned char k);

void evilPigAttackFlowerForest(TObj *o)
{
    Q *q = (Q *)&o->wb4;

    switch (o->substep) {
    case 0:
        q->n = D_801262E0[Rand() & 0xf];
        q->k = 0;
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
            ObjSetFacingToPlayer(o);
            spawnFlowerForestPigShot(o, q->k);
            evilPigSetBoxByDirection(o);
            func_80116C9C(o);
            o->substep++;
            func_8011F888();
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            if (q->n) {
                q->n--;
                q->k++;
                o->substep = 2;
                evilPigSetAnimBox(o, 0x15);
            } else {
                E8 *e = (E8 *)o->d94;
                o->substep++;
                e += o->wae;
                o->timer = e->t;
                evilPigSetAnimBox(o, 0x12);
            }
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
