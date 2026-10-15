// FUNC 8011fba0 628 X014
// MATCHING 8011fba0 628
#include "TOBJ.H"
#include "gamestate.h"

typedef struct { short a, b, c, d; } E8;

extern int AnimAdvance(TObj *);
extern unsigned int FUN_8001f9e0(void);
extern unsigned char D_80126110[];
extern void evilPigSetAnimBox(TObj *, int);
extern void evilPigPlayAttackSfx(void);
extern void evilPigPlayAttackSfxB(void);
extern void FUN_8001f8e4(TObj *);
extern void spawnTrickVillagePigShotB(TObj *, unsigned char);
extern void spawnTrickVillagePigShot(TObj *);
extern void evilPigSetBoxByDirection(TObj *);
extern void spawnEvilPigAttackFlash(TObj *);

void evilPigAttackTrickVillage(TObj *o)
{
    unsigned short *q = (unsigned short *)&o->wb4;

    switch (o->substep) {
    case 0:
        if (o->d38 < 0x80 && GAME.selectedSection != 7) {
            q[5] = D_80126110[FUN_8001f9e0() & 0xf];
            if (q[5] == 0)
                q[4] = 2;
            else
                q[4] = 0;
        } else {
            q[5] = 0;
            q[4] = 2;
        }
        q[3] = (unsigned char)o->d38;
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
            FUN_8001f8e4(o);
            if (q[5] == 0)
                spawnTrickVillagePigShotB(o, q[3]);
            else
                spawnTrickVillagePigShot(o);
            evilPigSetBoxByDirection(o);
            spawnEvilPigAttackFlash(o);
            o->substep++;
            evilPigPlayAttackSfxB();
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            if (q[4] != 0) {
                q[4]--;
                if (q[4] == 1)
                    q[3] = (o->d38 + 0x10) & 0xff;
                else
                    q[3] = (o->d38 - 0x10) & 0xff;
                o->substep = 2;
                evilPigSetAnimBox(o, 0x15);
            } else {
                E8 *t = (E8 *)o->d94;
                o->substep++;
                t += o->wae;
                o->timer = t->c;
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
