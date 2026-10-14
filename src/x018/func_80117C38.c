// FUNC 80117c38 884 X018
// MATCHING 80117c38 884
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct { TObj o; char pc0[0xd2 - 0xc0]; unsigned short wd2; } TX;
extern AT D_8011A564[];
extern unsigned char D_8009CE16[];
extern short D_1F8001C6;
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);

static __inline__ void setanim(TObj *o, short n)
{
    if (((TX *)o)->wd2 != n) {
        o->anim = D_8011A564[o->subtype].anims[n];
        AnimJump(o, 0);
        ((TX *)o)->wd2 = n;
    }
}

void func_80117C38(TObj *o)
{
    V6 v;
    TObj *q;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        GV2.bb0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        setanim(o, 4);
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 2, &v);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 3, &v);
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        setanim(o, 1);
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->timer = 4;
        if (D_8009CE16[0] == 0) {
            setEventStarted(0x72, 0, 0);
            o->timer = 200;
        }
        o->state = 8;
        break;
    case 8:
        if (--o->timer > 0) break;
        o->state = 9;
        break;
    case 9:
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->animFrame = o->wbc;
        D_1F8001C6 = 0;
        setanim(o, 1);
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
