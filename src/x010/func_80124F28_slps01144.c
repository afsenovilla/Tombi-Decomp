// JP x10 mermaid talk (retail form: one state machine, 31-entry jump table)
// VERSION SLPS_01144 x10 80124F28 1488
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012D968[];
extern unsigned char D_8009E9E5;
extern unsigned char D_8009EAD6[];
extern void AnimJump(TObj *, int);
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventComplete(int, int);
extern void setEventStarted(int, int, int);

void func_80124F28_slps01144(TObj *o)
{
    V6 v;
    TObj *e;

    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        o->state++;
        break;
    case 1:
        GVARS.b3e = 1;
        if (D_8009E9E5 == 0xff)
            o->state = !D_8009EAD6[1] ? 15 : 18;
        else
            o->state = !D_8009EAD6[0] ? 2 : 8;
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(5, 0, &v);
        o->anim = D_8012D968[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 3:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012D968[o->subtype].anims[0];
            AnimJump(o, 0);
            setEventComplete(0x76, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 4:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(5, 1, &v);
            o->anim = D_8012D968[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 5:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012D968[o->subtype].anims[0];
            AnimJump(o, 0);
            setEventStarted(0x7e, 0, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 6:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(5, 2, &v);
            o->anim = D_8012D968[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 7:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012D968[o->subtype].anims[0];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 8:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(5, 3, &v);
        o->anim = D_8012D968[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 9:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012D968[o->subtype].anims[0];
            AnimJump(o, 0);
            D_8009EAD6[0] = 1;
            o->state = 30;
        }
        break;
    case 15:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(5, 4, &v);
        o->anim = D_8012D968[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 16:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012D968[o->subtype].anims[0];
            AnimJump(o, 0);
            setEventStarted(0x74, 0, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 17:
        if (--o->timer <= 0) {
            o->anim = D_8012D968[o->subtype].anims[0];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 18:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(5, 5, &v);
        o->anim = D_8012D968[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 19:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012D968[o->subtype].anims[0];
            AnimJump(o, 0);
            D_8009EAD6[1] = 1;
            o->state = 30;
        }
        break;
    case 30:
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
#endif
