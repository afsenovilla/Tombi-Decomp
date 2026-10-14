// FUNC 80118dc8 620 X011
// MATCHING 80118dc8 620
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119C48[];
extern unsigned char D_8009D2B0;
extern short D_1F8001C6;
extern void AnimLoadDuration(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);

void func_80118DC8(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        D_8009D2B0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(V6 *)&o->a;
        o->anim = D_80119C48[o->subtype].anims[2];
        AnimLoadDuration(o);
        o->d90 = showMessageBoxDirect(2, 4, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 5, &v);
        o->state++;
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_80119C48[o->subtype].anims[0];
        AnimLoadDuration(o);
        setEventStarted(0x4f, 0, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 3:
        if (--o->timer == 0) o->state++;
        break;
    case 4:
        ((unsigned char*)&GVARS.b42)[0] = 0;
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
