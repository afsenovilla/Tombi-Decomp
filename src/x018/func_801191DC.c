// FUNC 801191dc 760 X018
// MATCHING 801191dc 760
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8011A564[];
extern short D_1F8001C6;
extern unsigned char D_8009CE28;
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventComplete(int, int);
extern void increaseMaxHealth(void);
extern void printInfoMessage(int, int);
extern void FUN_80026e0c(int, int);

#define WD2(o) (*(unsigned short *)((char *)(o) + 0xd2))

static __inline__ void setanim(TObj *o, short n)
{
    if (WD2(o) != n) {
        o->anim = D_8011A564[o->subtype].anims[n];
        AnimJump(o, 0);
        WD2(o) = n;
    }
}

void func_801191DC(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        v = *(V6 *)&o->a;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GV2.bb0)[0] = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = (*(Fix16 **)&PLAYER.obj.h)->p.whole < o->h->p.whole;
        setanim(o, 4);
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0xc, &v);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->timer = 200;
        setanim(o, 1);
        o->state++;
        break;
    case 2:
        if (--o->timer > 0) break;
        if (D_8009CE28 != 0xff) setEventComplete(0x84, 0);
        increaseMaxHealth();
        printInfoMessage(0x15, 3);
        FUN_80026e0c(0x99, 1);
        o->timer = 0x50;
        o->state = 8;
        break;
    case 8:
        if (--o->timer > 0) break;
        o->state = 9;
        break;
    case 9:
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
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
