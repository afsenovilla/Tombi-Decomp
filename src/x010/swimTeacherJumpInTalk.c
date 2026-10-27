// FUNC 80127e08 756 X010
// MATCHING 80127e08 756
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern int showMessageBoxDirect(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);
extern void setEventComplete(int, int);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void swimTeacherJumpInTalk(TObj *o)
{
    unsigned char *k;
    TObj *p;
    int m, n;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        setAnim(o, ANIMS(o)[4]);
        break;
    case 1:
        if (o->b68) {
            o->state++;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            FUN_8001f8e4(o);
            PLAYER_STATE = 5;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            (*(short*)&PLAYER.obj.animFrame) = 1 - o->animFrame;
            o->d90 = showMessageBoxDirect(2, 0x18, &o->a);
            setAnim(o, ANIMS(o)[5]);
            GCD.bd078 = 3;
            break;
        }
        k = ((unsigned char*)&GVARS.useItemPending);
        if (*k == 0) break;
        if (GVARS.useItemId != 3) break;
        *k = 0;
        o->state++;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        FUN_8001f8e4(o);
        m = 2;
        n = 0x16;
        goto talk;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->state = 0;
        break;
    case 3:
        o->state++;
        FUN_8001f8e4(o);
        m = 2;
        n = 0x17;
    talk:
        PLAYER_STATE = 5;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        (*(short*)&PLAYER.obj.animFrame) = 1 - o->animFrame;
        o->d90 = showMessageBoxDirect(m, n, &o->a);
        setAnim(o, ANIMS(o)[5]);
        break;
    case 4:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 5:
        setEventComplete(0x40, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 6:
        if (--o->timer == -1) {
            o->d90 = showMessageBoxDirect(2, 0x18, &o->a);
            o->anim = ANIMS(o)[5];
            FUN_8001fe6c(o);
            GCD.bd078 = 3;
            o->state = 2;
        }
        break;
    }
}
