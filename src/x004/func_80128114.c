// FUNC 80128114 496 X004
// MATCHING 80128114 496
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8013117C[];
extern unsigned char D_8009C942[], D_8009C93F[], D_8009C93E[];
extern unsigned char D_8009CE3D;
extern void AnimJump(TObj *, int);
extern int showMessageBoxDirect(int, int, V6 *);

void func_80128114(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        break;
    case 1:
        v = *(V6 *)&o->a;
        if (D_8009CE3D == 0xff)
            o->d90 = showMessageBoxDirect(4, 9, &v);
        else if (D_8009CE3D != 0)
            o->d90 = showMessageBoxDirect(4, 3, &v);
        else
            o->d90 = showMessageBoxDirect(4, 0, &v);
        o->anim = D_8013117C[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_8013117C[o->subtype].anims[0];
        AnimJump(o, 0);
        o->state++;
        break;
    case 3:
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
