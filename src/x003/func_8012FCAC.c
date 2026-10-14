// FUNC 8012fcac 1028 X003
// MATCHING 8012fcac 1028
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_80138FC8[], *D_80138FD8[];
extern int showMessageBoxDirect(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void FUN_80026e0c(int, int);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_8012FCAC(TObj *o)
{
    int m, n;
    Fix16 *q;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        if (GCD.bd087 == 0) o->state = 1;
        else if (GCD.bd087 == 1) o->state = 8;
        else if (GCD.bd087 == 2) o->state = 0x10;
        else if (GCD.bd087 == 4) o->b04 = 3;
        break;
    case 1:
        if (o->b68 == 0) break;
        m = 8;
        n = 0;
        q = &o->a;
        o->state++;
        GCD.bd087 = 1;
        goto talk;
    case 2:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
        o->state++;
        break;
    case 3:
        AnimAdvance(o);
        o->d90 = showMessageBoxDirect(8, 1, &o->a);
        o->state++;
        break;
    case 4:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
        o->state++;
        setEventStarted(0xbe, 0, 0);
        o->timer = 300;
        break;
    case 6:
        AnimAdvance(o);
        o->d90 = showMessageBoxDirect(8, 2, &o->a);
        o->state++;
        break;
    case 8:
        if (GVARS.b40 && GVARS.b41 == 0x8f) {
            o->state = 10;
            break;
        }
        if (o->b68 == 0) break;
        m = 8;
        n = 2;
        q = &o->a;
        o->state++;
        goto talk;
    case 10:
        o->state++;
        GCD.bd087 = 2;
        GVARS.b40 = 0;
        FUN_80026e0c(0x8f, 1);
        m = 8;
        n = 3;
        q = &o->a;
        goto talk;
    case 11:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
        o->state++;
        break;
    case 12:
        o->state++;
        o->d90 = showMessageBoxDirect(8, 4, &o->a);
        break;
    case 13:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
        o->state++;
        setEventComplete(0xbe, 0);
        o->timer = 300;
        break;
    case 5:
    case 14:
        if (--o->timer == -1) o->state++;
        break;
    case 16:
        if (o->visible == 0) {
            GCD.bd087 = 4;
            o->b04 = 3;
            break;
        }
        if (o->b68 == 0) break;
        m = 8;
        n = 4;
        q = &o->a;
        o->state++;
    talk:
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->d90 = showMessageBoxDirect(m, n, q);
        o->wac = 2;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_80138FC8[0]);
        break;
    case 7:
    case 9:
    case 17:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
    case 15:
        PLAYER.obj.b04 = 1;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->state = 0;
        o->wac = 6;
        o->anim = D_80138FD8[0];
        break;
    }
}
