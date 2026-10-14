// FUNC 8013b0f4 352 X001
// MATCHING 8013b0f4 352
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
extern TObj PLAYER;
extern unsigned short D_800A6066;
extern TObj *D_800A60C8;
extern unsigned char D_800A60F8, D_800A603C, D_800A603D, D_800A603E;
extern char D_80010748[];
extern void AnimLoadDuration(TObj *);
extern TObj *showMessageBox(int, int, int, int);
extern void FUN_800eeae4(TObj *, int, int);

void raceStartRematch(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;

    switch (o->step) {
    case 0:
        GVARS.b3f = 1;
        GVARS.b3e = 1;
        GVARS.b42 = 1;
        PLAYER.anim = D_80010748;
        AnimLoadDuration(&PLAYER);
        e->animFrame = 0;
        D_800A6066 = 0;
        D_800A60C8 = showMessageBox(2, 0x19, 0x80, 0x6c);
        FUN_800eeae4(&PLAYER, 0, 0);
        o->step++;
        break;
    case 1:
        p = D_800A60C8;
        if (p->b04 == 2) {
            p->b04 = 3;
            GVARS.b3f = 0;
            GVARS.b3e = 0;
            GVARS.b42 = 0;
            GCD.bce59 = 1;
            GCD.bcede = 1;
            D_800A60F8 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            e->subtype = 1;
            e->step = 1;
            e->state = 0;
            e->animFrame = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
