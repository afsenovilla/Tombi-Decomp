// FUNC 80116bdc 880 X018
// MATCHING 80116bdc 880
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8011A4B8[];
extern short D_1F8001C6;
extern unsigned char D_8009CFE0, D_8009CFF7;
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);
extern void printInfoMessage(int, int);

#define WD2(o) (*(unsigned short *)((char *)(o) + 0xd2))

static __inline__ void setanim(TObj *o, short n)
{
    if (WD2(o) != n) {
        o->anim = D_8011A4B8[o->subtype].anims[n];
        AnimJump(o, 0);
        WD2(o) = n;
    }
}

void juiceManJoinFightTalk(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        GV2.bb0 = 2;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        setanim(o, 2);
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        if (*(unsigned short *)&o->wba == 0) {
            o->d90 = showMessageBoxDirect(3, 0x17, &v);
            o->state++;
            break;
        }
        *(int *)((char *)o + 0x90) = showMessageBoxDirect(2, 0xd, &v);
        if (D_8009CFE0 == 0) {
            D_8009CFE0 = 1;
            D_8009CFF7++;
        }
        printInfoMessage(D_8009CFF7 + 0x32, 2);
        o->state = 8;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setanim(o, 0);
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        *(short *)((char *)o + 0x20) = 4;
        if (GCD.bce0b == 0) {
            setEventStarted(0x67, 0, 0);
            o->timer = 200;
        }
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer == 0) break;
        o->state = 9;
        break;
    case 8:
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
