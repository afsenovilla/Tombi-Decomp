// FUNC 801194d4 616 X018
// MATCHING 801194d4 616
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;
typedef struct { TObj o; char pc0[0x12]; unsigned short d2; } TObjX;

extern short D_1F8001C6;
extern AT3 D_80116564[];
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);

static __inline__ void chg(TObjX *e, unsigned short n)
{
    if (e->d2 != n) {
        e->o.anim = D_80116564[e->o.subtype].p[n];
        FUN_8001fe94(&e->o, 0);
        e->d2 = n;
    }
}

void witchMizunoHuntPigsTalk(TObjX *e)
{
    TObj *o = &e->o;
    B12 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(B12 *)&o->a;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        GV2.bb0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        chg(e, 4);
        v = *(B12 *)&o->a;
        o->d90 = (int)showMessageBoxDirect(3, 0xd, &v);
        o->state++;
        break;
    case 1:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        chg(e, 1);
        o->state = 9;
        break;
    case 9:
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
