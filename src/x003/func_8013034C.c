// FUNC 8013034c 364 X003
// MATCHING 8013034c 364
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_8013A368[];
extern void *D_8013A378[];
extern int showMessageBoxDirect(int, int, void *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

void func_8013034C(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68 == 0) break;
        o->state++;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->d90 = showMessageBoxDirect(2, 0xe, &o->a);
        o->wac = 4;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_8013A378[0]);
        break;
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
        o->anim = D_8013A368[0];
        o->state = 0;
        break;
    }
}
