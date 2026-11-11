// FUNC 8012962c 492 X003
// MATCHING 8012962c 492
// VERSION SLPS_01144 x03 80128474 448
#if defined(VERSION_SLPS_01144)
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } V12;
typedef struct { void **anims; int a; int b; } AT;
extern AT PHOENIX_SPIRIT_HINT_NPC_DEFS_ANIMS[];
extern Fix16 *PLAYER_H[];
extern void FUN_8001fe94(TObj *, int);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V12 *);

void func_80128474(TObj *o)
{
    V12 b;
    TObj *p;
    int t;

    switch (o->state) {
    case 0:
        if (o->b68) {
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            b = *(V12 *)&o->a;
            o->d90 = showMessageBoxDirect(2, 0xf, &b);
            o->anim = PHOENIX_SPIRIT_HINT_NPC_DEFS_ANIMS[o->subtype].anims[9];
            FUN_8001fe94(o, 0);
            *(short *)((char *)&PLAYER + 0x2e) = 0;   /* (JP: the retail store is PLAYER + 0x2E here, PAL clears wb2) */
            PLAYER.obj.b04 = 5;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            o->state++;
        }
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->anim = PHOENIX_SPIRIT_HINT_NPC_DEFS_ANIMS[o->subtype].anims[0];
            FUN_8001fe94(o, 0);
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
            PLAYER.obj.wb2 = 0;
            PLAYER.obj.b04 = 1;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            o->b68 = 0;
            o->state = 0;
        }
        break;
    }
    if (!o->visible) o->b04 = 3;
}
#else
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } V12;
typedef struct { void **anims; int a; int b; } AT;
extern AT PHOENIX_SPIRIT_HINT_NPC_DEFS_ANIMS[];
extern Fix16 *PLAYER_H[];
extern void FUN_8001fe94(TObj *, int);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V12 *);

void phoenixSpiritHintTalk(TObj *o)
{
    V12 b;
    TObj *p;
    int t;

    switch (o->state) {
    case 0:
        if (o->b68) {
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            b = *(V12 *)&o->a;
            o->d90 = showMessageBoxDirect(2, 0xf, &b);
            o->anim = PHOENIX_SPIRIT_HINT_NPC_DEFS_ANIMS[o->subtype].anims[9];
            FUN_8001fe94(o, 0);
            o->animFrame = o->b68 & 1;
            t = o->h->p.whole < *(short *)((char *)PLAYER_H[0] + 2);
            PLAYER.obj.b04 = 5;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            PLAYER.obj.animFrame = t;
            o->state++;
        }
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->animFrame = 1;
            o->anim = PHOENIX_SPIRIT_HINT_NPC_DEFS_ANIMS[o->subtype].anims[0];
            FUN_8001fe94(o, 0);
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
            PLAYER.obj.wb2 = 0;
            PLAYER.obj.b04 = 1;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            o->b68 = 0;
            o->state = 0;
        }
        break;
    }
    if (!o->visible) o->b04 = 3;
}
#endif
