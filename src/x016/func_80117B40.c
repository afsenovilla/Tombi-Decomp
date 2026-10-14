// FUNC 80117b40 500 X016
// MATCHING 80117b40 500
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80118D1C[];
extern unsigned char D_8009D2B0;
extern unsigned char D_8009C942[], D_8009C93F[];
extern short D_1F8001C6;
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);

void func_80117B40(TObj *o)
{
    V6 v;
    TObj *q;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        D_8009D2B0 = 2;
        o->wbc = o->animFrame;
        o->anim = D_80118D1C[o->subtype].anims[2];
        AnimLoadDuration(o);
        o->animFrame = o->b68 & 1;
        o->d90 = showMessageBoxDirect(2, 0xc, &v);
        o->state = 1;
        break;
    case 1:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->anim = D_80118D1C[o->subtype].anims[0];
        AnimLoadDuration(o);
        o->state = 9;
        break;
    case 9:
        D_8009C942[0] = 0;
        D_8009C93F[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->animFrame = o->wbc;
        D_1F8001C6 = 0;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
