// FUNC 8011921c 636 X011
// MATCHING 8011921c 636
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119C48[];
extern unsigned char D_8009D2B0;
extern unsigned char D_8009CFDF, D_8009CFF7;
extern short D_1F8001C6;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void printInfoMessage(int, int);

void func_8011921C(TObj *o)
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
        o->anim = D_80119C48[o->subtype].anims[0];
        AnimLoadDuration(o);
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        *(int *)((char *)o + 0x90) = showMessageBoxDirect(3, 0x15, &v);
        if (D_8009CFDF == 0) {
            D_8009CFDF = 1;
            D_8009CFF7++;
        }
        printInfoMessage(D_8009CFF7 + 0x32, 2);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_80119C48[o->subtype].anims[0];
        AnimLoadDuration(o);
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->timer = 4;
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer != 0) o->state = 9;
        break;
    case 9:
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
