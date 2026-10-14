// FUNC 801182fc 656 X018
// MATCHING 801182fc 656
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct {
    TObj t;
    char pad[0x12];
    unsigned short wd2;
} TO2;
extern AT D_8011A564[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F[];
extern unsigned char D_8009D2B0;
extern short D_1F8001C6;
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);

static __inline__ void setAnim(TO2 *o, unsigned short n)
{
    if (o->wd2 != n) {
        o->t.anim = D_8011A564[o->t.subtype].anims[n];
        AnimJump(&o->t, 0);
        o->wd2 = n;
    }
}

void func_801182FC(TO2 *o)
{
    V6 v;
    TObj *p;

    switch (o->t.state) {
    case 0:
        if (o->t.b68 == 0) break;
        v = *(V6 *)&o->t.a;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009D2B0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->t.wbc = o->t.animFrame;
        o->t.animFrame = o->t.b68 & 1;
        setAnim(o, 4);
        v = *(V6 *)&o->t.a;
        o->t.d90 = showMessageBoxDirect(3, 5, &v);
        o->t.state++;
        break;
    case 1:
        AnimAdvance(&o->t);
        p = (TObj *)o->t.d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->t.state = 9;
        break;
    case 8:
        if (--o->t.timer > 0) break;
        o->t.state = 9;
        break;
    case 9:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        D_1F8001C6 = 0;
        setAnim(o, 1);
        o->t.animFrame = o->t.wbc;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->t.b68 = 0;
        o->t.step = 0;
        o->t.state = 0;
        break;
    }
}
