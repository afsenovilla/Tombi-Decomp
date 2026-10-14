// FUNC 8012cdc0 544 X003
// MATCHING 8012cdc0 544
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;

extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_8009CF03;
extern void *D_801399BC[];
extern void *D_801399CC[];
extern int showMessageBoxDirect(int, int, V6 *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void setEventStarted(int, int, int);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_8012CDC0(TObj *o)
{
    V6 v;
    TObj *p;

    v = *(V6 *)&o->a;
    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->wac = 9;
        o->state++;
        setAnim(o, D_801399CC[0]);
        break;
    case 1:
        AnimAdvance(o);
        if (D_8009CF03 == 3) {
            o->step++;
            o->state = 0;
            PLAYER.obj.b04 = 5;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            D_8009C93F = 1;
            D_8009C942 = 1;
            break;
        }
        if (o->b68 == 0) break;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->d90 = showMessageBoxDirect(6, 6, &v);
        o->wac = 5;
        o->state++;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_801399BC[0]);
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->b68 = 0;
        o->state = 0;
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        D_8009C93F = 0;
        D_8009C942 = 0;
        setEventStarted(0xad, 0, 1);
        break;
    }
}
