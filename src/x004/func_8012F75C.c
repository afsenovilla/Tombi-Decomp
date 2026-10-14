// FUNC 8012f75c 632 X004
// MATCHING 8012f75c 632
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_801314BC[];
extern unsigned char D_8009CE24[];
extern short D_1F8001C6;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventComplete(int, int);
extern void FUN_80026c50(int, int, int);

void func_8012F75C(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        GV2.bb0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->d90 = showMessageBoxDirect(5, 0, &v);
        o->anim = D_801314BC[o->subtype].anims[1];
        AnimLoadDuration(o);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->timer = 4;
        D_1F8001C6 = 0;
        if (D_8009CE24[0] == 1) {
            setEventComplete(0x80, 1);
            FUN_80026c50(0x96, 1, 1);
            o->timer = 200;
        }
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer != 0) o->state = 9;
        break;
    case 9:
        D_1F8001C6 = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->anim = D_801314BC[o->subtype].anims[0];
        AnimLoadDuration(o);
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
