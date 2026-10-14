// FUNC 80119034 488 X011
// MATCHING 80119034 488
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } V12;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119C48[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009D2B0[];
extern short D_1F8001C6;
extern void AnimLoadDuration(TObj *);
extern int showMessageBoxDirect(int, int, V12 *);

void func_80119034(TObj *o)
{
    V12 b;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68) {
            D_8009C942[0] = 1;
            D_8009D2B0[0] = 2;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            o->wbc = o->animFrame;
            o->animFrame = o->b68 & 1;
            b = *(V12 *)&o->a;
            o->anim = D_80119C48[o->subtype].anims[2];
            AnimLoadDuration(o);
            o->d90 = showMessageBoxDirect(2, 5, &b);
            o->state++;
        }
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->anim = D_80119C48[o->subtype].anims[0];
            AnimLoadDuration(o);
            o->state++;
        }
        break;
    case 2:
        D_8009C942[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        D_1F8001C6 = 0;
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
