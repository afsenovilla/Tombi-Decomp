// FUNC 80125fb4 1280 X010
#include "plx.h"
#include "gvars.h"
#include "player.h"
/* score 124: everything matches except that case 5 is cross-jumped into case 8 from the
   showMessageBoxDirect call (identical arg setup); in the game case 8 sets up a2 first (li a1,9 in the
   jal delay slot), so the tails only merge from setanim(o, 2). Tried: pointer/extra local for
   the V6 copy, an fx() inline, moving the copy among the global stores.
   Also tried (o31): p = &v before/after the copy, k = 9 local, casts/no-proto/short-arg calls, inline wrappers
   (own stack slot, frame 64), block-local V6: the arg order stays a0,a1,a2 (RTL order); only a computed a1
   (o->state + 1) moves a2 above a1. A second callee name in case 5 stops the merge but leaves 12 B short. */
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012F3C4[];
extern unsigned char D_8009D0E7[];
extern unsigned char D_8009C970, D_8009C971;
extern void FUN_8001fe94(TObj *, int);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventComplete(int, int);
extern void setEventStarted(int, int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_80026c50(int, int, int);
extern void FUN_8001e4f0(int);

static __inline__ void setanim(TObj *o, int n)
{
    o->anim = D_8012F3C4[o->subtype].anims[n];
    FUN_8001fe94(o, 0);
}

void func_80125FB4(TObj *o)
{
    V6 v;
    short i;
    unsigned char *c;

    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        o->state++;
        break;
    case 1:
        setEventComplete(0x7e, 0);
        o->timer = 300;
        o->state++;
        break;
    case 2:
        if (--o->timer > 0) break;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(5, 6, &v);
        setanim(o, 2);
        for (i = 0; i < 10; i++)
            FUN_80026e0c(i + 0x43, D_8009D0E7[i]);
        o->state++;
        break;
    case 3:
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_80026c50(0x2c, 1, 1);
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(5, 7, &v);
        o->state++;
        }
        break;
    case 4:
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setanim(o, 0);
        o->timer = 0x3c;
        o->state++;
        }
        break;
    case 5:
        if (--o->timer > 0) break;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(5, 8, &v);
        setanim(o, 2);
        o->state++;
        break;
    case 6:
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_80026c50(0x95, 1, 1);
        setanim(o, 0);
        setEventStarted(0x9c, 0, 0);
        o->timer = 300;
        o->state++;
        }
        break;
    case 7:
        if (--o->timer <= 0) o->state++;
        break;
    case 8:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(5, 9, &v);
        setanim(o, 2);
        o->state++;
        break;
    case 9:
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        c = &D_8009C970;
        if (*c < D_8009C971) {
            int t = D_8009C971;
            PLX.s6108 = t;
            PLX.s610a = t;
            *c = t;
            FUN_8001e4f0(10);
        }
        setanim(o, 0);
        PLAYER.obj.b04 = 1;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        PLAYER.obj.wb2 = 0;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        }
        break;
    }
}
