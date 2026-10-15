// FUNC 8011759c 604 X009
// MATCHING 8011759c 604
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

typedef struct { short vx, vy, vz, pad; } SVEC;

extern TObj *D_8009C94C[];
extern void cullObjectToDrawListMargin(TObj *, int);
extern void getBaseMatrix(void *);
extern void RotMatrix(SVEC *, void *);
extern void freeObjectLayer8(TObj *);
extern void spawnRotatingPlaneObjects(TObj *);
extern void rotatingPlaneSway(TObj *);
extern void rotatingPlanePlaySfx(TObj *);

void updateRotatingPlane(TObj *o)
{
    TObj *e;
    SVEC v;

    switch (o->b04) {
    case 0:
        e = (TObj *)o->d94;
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b6a = 0;
        o->b6b = 0;
        o->ba4 = 0;
        *(int *)&o->w5c = o->a.p.whole;
        o->d60 = o->y.p.whole;
        o->d64 = o->b.p.whole;
        while (e) {
            *(TObj **)&e->wa8 = o;
            if ((e->category == 4 && (TObj *)e->d90 != o) || e->category == 3) {
                e->d90 = (int)o;
                e->d30 = e->a.raw - o->a.raw;
                e->d34 = e->y.raw - o->y.raw;
                e->d38 = e->b.raw - o->b.raw;
            }
            e = (TObj *)e->d94;
        }
        o->d30 = o->h->raw;
        o->d34 = o->y.raw;
        o->d38 = o->d->raw;
        if (o->animFrame == 2) {
            (*(TObj **)&GVARS.p48)->d94 = (int)o;
            D_8009C94C[0]->d94 = (int)o;
        } else {
            spawnRotatingPlaneObjects(o);
        }
        break;
    case 1:
        if (GAME.selectedPlane == 0x20 || GVARS.b3a != 1) {
            cullObjectToDrawListMargin(o, 0xa0);
            rotatingPlaneSway(o);
            rotatingPlanePlaySfx(o);
            getBaseMatrix(&o->w48);
            v.vx = o->d84;
            v.vy = o->d88;
            v.vz = o->d8c;
            RotMatrix(&v, &o->w48);
            if (!o->subtype && GVARS.b40 && GVARS.b41 == 0x8a) GCD.bd07e = 1;
        }
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
