// FUNC 801280fc 1120 X010
// MATCHING 801280fc 1120
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
#include "gametask.h"
#include "camera.h"

typedef struct { char p[0x4c]; short w4c, w4e; } SC;

extern TObj PLAYER;
extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern short PLAYER_Y_WHOLE;
extern short D_8012F3C8[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playerSetAnim(TObj *, int, int);
extern int showMessageBoxDirect(int, int, void *);
extern void setEventComplete(int, int);
extern void swimTeacherFloatPlayer(TObj *, int);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

void swimTeacherFloatTalk(TObj *o)
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
        GVARS.b42 = 1;
        PLAYER.active = 5;
        PLAYER_STATE = 5;
        PLAYER_SUBSTATE = 0x64;
        PLAYER_STEP = 0;
        playerSetAnim(&PLAYER, 0x1e, 0);
        PLAYER.y.p.whole = -0xd2;
        o->d34 = 0xff2e0000;
        o->ba7 = 0;
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
        n = 0x19;
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
        playerSetAnim(&PLAYER, 0x47, 0);
        o->ba7 = 0;
        PLAYER.y.p.whole = -0xe2;
        PLAYER.animFrame = 0;
        o->d34 = PLAYER.y.raw;
        o->d30 = PLAYER.h->p.whole;
        o->state++;
        o->d90 = showMessageBoxDirect(2, 0x1a, &o->a);
        break;
    case 6:
        swimTeacherFloatPlayer(o, 4);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 7:
        swimTeacherFloatPlayer(o, 2);
        o->state++;
        o->d90 = showMessageBoxDirect(2, D_8012F3C8[GCD.bcf1c], &o->a);
        break;
    case 8:
        swimTeacherFloatPlayer(o, 2);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 9:
        swimTeacherFloatPlayer(o, 3);
        m = 2;
        n = 0x23;
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
        n = 0x24;
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
        swimTeacherFloatPlayer(o, 4);
        if (GAME.fadeScreenControl == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        break;
    }
}
