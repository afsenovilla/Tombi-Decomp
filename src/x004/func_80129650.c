// FUNC 80129650 652 X004
// MATCHING 80129650 652
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80131188[];
extern unsigned char D_8009D2B0;
extern short D_1F8001C6;
extern unsigned char D_8009CFDD, D_8009CFF7;
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void printInfoMessage(int, int);

#define WD2(o) (*(unsigned short *)((char *)(o) + 0xd2))

static __inline__ void setanim(TObj *o, short n)
{
    if (WD2(o) != n) {
        o->anim = D_80131188[o->subtype].anims[n];
        AnimJump(o, 0);
        WD2(o) = n;
    }
}

void func_80129650(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_8009D2B0 = 2;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        setanim(o, 2);
        *(short *)((char *)o + 0xbc) = o->animFrame;
        if (D_8009CFDD == 0) {
            D_8009CFDD = 1;
            D_8009CFF7++;
        }
        printInfoMessage(D_8009CFF7 + 0x32, 2);
        o->animFrame = o->b68 & 1;
        o->d90 = showMessageBoxDirect(5, 1, &v);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setanim(o, 0);
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->state = 9;
        break;
    case 9:
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
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
