// JP-only (slps01144), written from the disassembly
// VERSION SLPS_01144 x10 80126728 1128
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
#include "gametask.h"
#include "camera.h"

extern TObj PLAYER;
extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern short PLAYER_Y_WHOLE;
extern short D_8012D96C[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playerSetAnim(TObj *, int, int);
extern int showMessageBoxDirect(int, int, void *);
extern void setEventComplete(int, int);
extern void swimTeacherFloatPlayer(TObj *, int);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

void func_80126728_slps01144(TObj *o)
{
    TObj *p;
    int m, n;

    switch (o->state) {
    case 0:
        o->anim = ANIMS(o)[0];
        AnimLoadDuration(o);
        o->state++;
        VIEW_CAMERA.pos[1].p.hi = -300;
    case 1:
        if (PLAYER_Y_WHOLE < -0xe9) break;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        PLAYER.active = 5;
        PLAYER_STATE = 5;
        PLAYER_SUBSTATE = 100;
        PLAYER_STEP = 0;
        playerSetAnim(&PLAYER, 30, 0);
        PLAYER.y.p.whole = -0xd2;
        o->d34 = 0xff2e0000;
        o->timer = 0x3c;
        o->state++;
        GCD.bd078 = 4;
        break;
    case 2:
        swimTeacherFloatPlayer(o, 1);
        if (--o->timer == -1) o->state++;
        break;
    case 3:
        swimTeacherFloatPlayer(o, 1);
        o->anim = ANIMS(o)[5];
        AnimLoadDuration(o);
        m = 2;
        n = 25;
        goto talk;
    case 4:
        swimTeacherFloatPlayer(o, 1);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 5:
        swimTeacherFloatPlayer(o, 1);
        playerSetAnim(&PLAYER, 71, 0);
        PLAYER.y.p.whole -= 18;
        PLAYER.animFrame = 0;
        o->d34 = PLAYER.y.raw;
        o->d30 = PLAYER.h->p.whole;
        o->state++;
        o->d90 = showMessageBoxDirect(2, 26, &o->a);
        break;
    case 6:
        swimTeacherFloatPlayer(o, 2);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 7:
        swimTeacherFloatPlayer(o, 4);
        o->state++;
        o->d90 = showMessageBoxDirect(2, D_8012D96C[GCD.bcf1c], &o->a);
        break;
    case 8:
        swimTeacherFloatPlayer(o, 4);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 9:
        swimTeacherFloatPlayer(o, 3);
        m = 2;
        n = 35;
        goto talk;
    case 10:
        swimTeacherFloatPlayer(o, 3);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 11:
        swimTeacherFloatPlayer(o, 2);
        m = 2;
        n = 36;
    talk:
        o->state++;
        o->d90 = showMessageBoxDirect(m, n, &o->a);
        break;
    case 12:
        swimTeacherFloatPlayer(o, 2);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 13:
        swimTeacherFloatPlayer(o, 3);
        ((int*)&GAME.area00_eventControl)[0] |= 8;
        setEventComplete(8, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 14:
        swimTeacherFloatPlayer(o, 3);
        if (--o->timer == -1) {
            (*(short*)&GAME.nextArea) = 10;
            (*(short*)&GAME.nextSpawnPoint) = 3;
            (*(short*)&GAME.nextSection) = 0;
            GAME.fadeScreenControl = 3;
            GVARS.fadeEffect = 0;
            o->state++;
        }
        break;
    case 15:
        swimTeacherFloatPlayer(o, 2);
        if (GAME.fadeScreenControl == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        break;
    }
}
#endif
