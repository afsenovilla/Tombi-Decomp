// FUNC 8012cfcc 496 X010
// MATCHING 8012cfcc 496
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } P4;

extern short D_1F80016A_S; /* second name for PLAYER_H_WHOLE: keeps the reload in the loop (debt) */
extern short PLAYER_H_WHOLE[], PLAYER_D_WHOLE[];
extern P4 D_8012F4AC[];
extern TObj *allocObjectLayer2(void);
extern void PoolFree_1F800210(TObj *);

void spearPigAmbushSpawnStep(TObj *o)
{
    TObj *e;
    int i;
    P4 *p;

    switch (o->b04) {
    case 0:
        if (GVARS.playerLanded) o->b04++;
        break;
    case 1:
        if (GVARS.hurtState == 1) {
            o->b04++;
            o->w08 = 0x3c;
        }
        break;
    case 2:
        if (--o->w08 == 0) {
            p = D_8012F4AC;
            for (i = 0; i < 8; i++) {
                e = allocObjectLayer2();
                if (e) {
                    e->type = 0x3a;
                    e->active = 2;
                    e->b0c = i;
                    e->subtype = 3;
                    e->b0a = 2;
                    e->d8c = 0;
                    e->a.p.whole = PLAYER_H_WHOLE[0] + p->x;
                    e->y.p.whole = ((short*)((char*)&SPAD.i16c + 2))[0] + p->y;
                    e->b.p.whole = PLAYER_D_WHOLE[0] + p->z;
                    e->w74 = D_1F80016A_S + p->x / 2;
                    p++;
                }
            }
            o->w08 = 300;
            o->b04++;
        }
        break;
    case 3:
        if (--o->w08 == -1) {
            GVARS.hurtState = 2;
            PoolFree_1F800210(o);
        }
        break;
    }
}
