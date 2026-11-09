// FUNC 80118140 488 X011
// MATCHING 80118140 488
#include "gv2.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;

extern unsigned char D_8009C942A[];
extern short DRAW_FROZEN;
extern AT3 FACTORY_NPC_DEFS_ANIMS[];
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void FUN_8001fe6c(TObj *);

void factoryBossTalk(TObj *o)
{
    B12 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        D_8009C942A[0] = 1;
        GV2.bb0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(B12 *)&o->a;
        o->anim = FACTORY_NPC_DEFS_ANIMS[o->subtype].p[2];
        FUN_8001fe6c(o);
        o->d90 = (int)showMessageBoxDirect(2, 0xc, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = FACTORY_NPC_DEFS_ANIMS[o->subtype].p[0];
        FUN_8001fe6c(o);
        o->state++;
        break;
    case 2:
        D_8009C942A[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        DRAW_FROZEN = 0;
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
