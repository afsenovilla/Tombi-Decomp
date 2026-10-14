// FUNC 80117fac 848 X018
// MATCHING 80117fac 848
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

typedef struct {
    TObj o;
    unsigned char pad[0x12];
    unsigned short wd2;
} P;

typedef struct { short s[6]; } V;
typedef struct { void **tbl; int x; int y; } E;

extern E D_8011A564[];
extern unsigned char D_8009D132;
extern short D_1F8001C6;
extern void FUN_8001fe94(P *, int);
extern TObj *showMessageBoxDirect(int, int, V *);
extern int AnimAdvance(P *);
extern void FUN_80026e0c(int, int);
extern void setEventComplete(int, int);

static __inline__ void SetAnim(P *o, short n)
{
    if (o->wd2 != n) {
        o->o.anim = D_8011A564[o->o.subtype].tbl[n];
        FUN_8001fe94(o, 0);
        o->wd2 = n;
    }
}

void witchMizunoCrystalBallsFoundTalk(P *o)
{
    V v;
    TObj *p;
    int k2;

    switch (o->o.state) {
    case 0:
        if (!o->o.b68) break;
        k2 = 2;
        v = *(V *)&o->o.a;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        GV2.bb0 = k2;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->o.wbc = o->o.animFrame;
        o->o.animFrame = o->o.b68 & 1;
        SetAnim(o, 4);
        if (D_8009D132 && GCD.bd0b7) {
            v = *(V *)&o->o.a;
            o->o.d90 = (int)showMessageBoxDirect(3, 4, &v);
            o->o.state = k2;
        } else {
            v = *(V *)&o->o.a;
            o->o.d90 = (int)showMessageBoxDirect(3, 3, &v);
            o->o.state++;
        }
        break;
    case 1:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->o.d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->o.state = 9;
        }
        }
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->o.d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->o.timer = 0xc8;
            FUN_80026e0c(0x8e, 1);
            FUN_80026e0c(0x13, 1);
            setEventComplete(0x72, 0);
            o->o.state = 8;
        }
        break;
    case 8:
        if (--o->o.timer > 0) break;
        o->o.state = 9;
        break;
    case 9:
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->o.animFrame = o->o.wbc;
        D_1F8001C6 = 0;
        SetAnim(o, 1);
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->o.b68 = 0;
        o->o.step = 0;
        o->o.state = 0;
        break;
    }
}
