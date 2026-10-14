// FUNC 8012d198 788 X004
// MATCHING 8012d198 788
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern unsigned char D_8009CDF5, D_8009CE63[];
extern void *D_801357D8[], *D_801357DC[], *D_801357E0[];
extern short D_1F80016A;
extern int showMessageBoxDirect(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern short FUN_80040278(TObj *, int, int);
extern void setEventStarted(int, int, int);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_8012D198(TObj *o)
{
    TObj *p;
    int m, n;

    switch (o->state) {
    case 0:
        if (D_8009CDF5) {
            o->b04 = 3;
            break;
        }
        o->b68 = 0;
        o->animFrame = 1;
        if (D_8009CE63[0] == 0xff) o->state = 3;
        else o->state = 1;
        break;
    case 1:
        if (o->b68 == 0) break;
        m = 5;
        n = 2;
        goto talk;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wac = 0;
        o->anim = D_801357D8[0];
        o->state = 0;
        break;
    case 3:
        if (o->b68 == 0) break;
        m = 5;
        n = 3;
    talk:
        o->state++;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->d90 = showMessageBoxDirect(m, n, &o->a);
        o->wac = 2;
        setAnim(o, D_801357E0[0]);
        break;
    case 4:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 1;
        o->anim = D_801357DC[0];
        FUN_8001fe6c(o);
        o->state++;
        break;
    case 5:
        AnimAdvance(o);
        o->h->p.whole -= 2;
        o->y.p.whole += 2;
        FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        if (o->h->p.whole < 10) {
            setEventStarted(0x51, 0, 0);
            o->timer = 0x168;
            o->state++;
        }
        break;
    case 6:
        if (o->timer >= 0x141) o->h->p.whole -= 2;
        if (--o->timer == -1) o->state++;
        break;
    case 7:
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->b04 = 3;
        break;
    }
    if (D_1F80016A >= 0x9f) PLAYER.obj.a.p.whole = 0x9e;
}
