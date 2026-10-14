// FUNC 801264b4 1056 X010
// MATCHING 801264b4 1056
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;

extern AT3 D_8012F3C4[];
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void FUN_8001fe94(TObj *, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_80026c50(int, int, int);

void func_801264B4(TObj *o)
{
    B12 v;

    switch (o->state) {
    case 2:
        v = *(B12 *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 0xa, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state++;
        break;
    case 3: {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        setEventStarted(0x7f, 0, 0);
        o->timer = 300;
        o->state++;
        break;
    }
    case 4:
        if (--o->timer > 0)
            break;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        v = *(B12 *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 0xb, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state = 15;
        break;
    case 7:
        v = *(B12 *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 0xc, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state++;
        break;
    case 8: {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        FUN_80026e0c(0x59, 1);
        FUN_80026e0c(0x5d, 1);
        FUN_80026e0c(0x5e, 1);
        FUN_80026e0c(0x5f, 1);
        FUN_80026e0c(0x73, 1);
        FUN_80026c50(0x60, 1, 1);
        setEventComplete(0x7f, 1);
        o->timer = 300;
        o->state++;
        break;
    }
    case 9:
        if (--o->timer > 0)
            break;
        v = *(B12 *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 0xd, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state++;
        break;
    case 10: {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        v = *(B12 *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 0xe, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state = 15;
        break;
    }
    case 0:
    case 5:
    case 11:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        o->state++;
        break;
    case 1:
    case 6:
    case 12:
        o->state++;
        break;
    case 13:
        v = *(B12 *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 0xe, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state = 15;
        break;
    case 14:
        break;
    case 15: {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_8012F3C4[o->subtype].p[0];
        FUN_8001fe94(o, 0);
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
        break;
    }
    }
}
