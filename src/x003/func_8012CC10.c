// FUNC 8012cc10 432 X003
// MATCHING 8012cc10 432
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } B12;

extern unsigned char D_8009C93F[], D_8009C942[], D_8009CF03[];
extern void *D_801399CC[], *D_801399BC[];
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);

void func_8012CC10(TObj *o)
{
    B12 v;
    TObj *p;

    v = *(B12 *)&o->a;
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        o->wac = 9;
        o->state++;
        o->anim = D_801399CC[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        FUN_8001fec0(o);
        if (o->b68 == 0)
            break;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wac = 5;
        o->state++;
        o->animFrame = o->b68 & 1;
        o->anim = D_801399BC[0];
        FUN_8001fe6c(o);
        D_8009CF03[0] = 2;
        o->d90 = (int)showMessageBoxDirect(6, 5, &v);
        break;
    case 2:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->step = 2;
        o->state = 1;
        o->b68 = 1;
        break;
    }
}
