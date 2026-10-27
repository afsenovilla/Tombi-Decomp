// FUNC 8012a31c 852 X004
// MATCHING 8012a31c 852
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { Fix16 a, y, b; } V3;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_801311D0[];
extern unsigned char PLAYER[];
extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern short D_800A6066;
extern Fix16 *PLAYER_H[];
extern void AnimJump(TObj *, int);
extern int showMessageBoxDirect(int, int, V6 *);
extern void playerSetAnim(void *, int, int);
extern void FUN_8003e300(int, int, V3 *);

void townHallWorkerTalk(TObj *o)
{
    V3 v;
    TObj *p;

    switch (o->state) {
    case 0:
        PLAYER_STATE = 5;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->animFrame = o->b68 & 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        o->state++;
        break;
    case 1:
        *(V6 *)&v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 0, (V6 *)&v);
        o->anim = D_801311D0[o->subtype].anims[3];
        AnimJump(o, 0);
        o->state++;
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->active = 4;
        o->animFrame = 1;
        o->velH = 1;
        o->b6b = 0;
        o->anim = D_801311D0[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 3:
        if (o->visible == 0) {
            GVARS.b3e = 0;
            PLAYER[4] = 5;
            PLAYER_SUBSTATE = 0x64;
            PLAYER_STEP = 0;
            playerSetAnim(PLAYER, 2, 0);
            D_800A6066 = 1;
            o->state++;
            break;
        }
        o->h->p.whole -= o->velH;
        if (o->b6b) break;
        if (o->h->p.whole < 0xa0) {
            o->b6b = 1;
            GCD.bcf29 = 1;
        }
        break;
    case 4:
        PLAYER_H[0]->p.whole--;
        if (PLAYER_H[0]->p.whole < 0xa0) {
            PLAYER_H[0]->p.whole = 0xa0;
            playerSetAnim(PLAYER, 0, 0);
            PLAYER_STATE = 5;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            v.a.p.whole = 0xa0;
            v.y.p.whole = -0x44;
            v.b.p.whole = 0;
            FUN_8003e300(0x36, 0, &v);
            o->timer = 0x78;
            o->state++;
        }
        break;
    case 5:
        if (--o->timer > 0) break;
        o->state = 15;
        break;
    case 15:
        PLAYER_STATE = 1;
        GCD.bcf2d = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        GVARS.b3e = 0;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        break;
    }
}
