// FUNC 80130644 364 X003
// MATCHING 80130644 364
#include "player.h"
#include "TOBJ.H"

extern unsigned char D_8009C93F[], D_8009C942[];
extern void *D_8013A3B4[], *D_8013A3A8[];
extern TObj *showMessageBoxDirect(int, int, Fix16 *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);

void func_80130644(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68 == 0)
            break;
        o->state++;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->d90 = (int)showMessageBoxDirect(2, 0xe, &o->a);
        o->wac = 3;
        o->animFrame = o->b68 & 1;
        o->anim = D_8013A3B4[0];
        FUN_8001fe6c(o);
        break;
    case 2:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wac = 0;
        o->anim = D_8013A3A8[0];
        o->state = 0;
        break;
    }
}
