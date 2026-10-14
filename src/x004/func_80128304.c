// FUNC 80128304 872 X004
// MATCHING 80128304 872
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8013117C[];
extern unsigned char D_8009D138;
extern void AnimJump(TObj *, int);
extern int showMessageBoxDirect(int, int, V6 *);
extern void removeItemFromInventory(int, int);
extern void setEventComplete(int, int);
extern void setEventStarted(int, int, int);

void func_80128304(TObj *o)
{
    V6 v;
    TObj *e;

    switch (o->state) {
    case 0:
        GVARS.b40 = 0;
        removeItemFromInventory(0x94, D_8009D138);
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        o->state++;
        break;
    case 1:
        o->state++;
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(4, 1, &v);
        o->anim = D_8013117C[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 3:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            setEventComplete(0x70, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 4:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 2, &v);
            o->anim = D_8013117C[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 5:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            setEventStarted(0x99, 0, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 6:
        if (--o->timer <= 0) o->state++;
        break;
    case 7:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(4, 3, &v);
        o->anim = D_8013117C[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 8:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            o->state = 0xf;
        }
        break;
    case 15:
        PLAYER.obj.b04 = 1;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        PLAYER.obj.wb2 = 0;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
