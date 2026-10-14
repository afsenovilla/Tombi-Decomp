// FUNC 80129be0 564 X004
// MATCHING 80129be0 564
#include "gcd.h"
#include "gvars.h"
#include "player.h"
/* debt: volatile d90 store + volatile subtype read keep sw d90 before lbu subtype in case 1 (do {} while (0) after the store gives score 4). */
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_801311B8[];
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);

void func_80129BE0(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->animFrame = o->b68 & 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        o->state++;
        break;
    case 1:
        v = *(V6 *)&o->a;
        *(volatile int *)&o->d90 = showMessageBoxDirect(2, 1, &v);
        o->anim = D_801311B8[*(volatile unsigned char *)&o->subtype].anims[3];
        AnimJump(o, 0);
        o->state++;
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_801311B8[o->subtype].anims[0];
        AnimJump(o, 0);
        o->state = 15;
        if (GCD.bcddc == 0) {
            o->state = 3;
            o->timer = 200;
            setEventStarted(0x38, 0, 0);
        }
        break;
    case 3:
        if (--o->timer == 0) o->state = 15;
        break;
    case 15:
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
