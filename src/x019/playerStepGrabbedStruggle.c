// FUNC 800fa91c 1232 X019
// MATCHING 800fa91c 1232
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
extern TObj *PLAYER_EXT;
extern unsigned char *BITTEN_OBJECT;
extern unsigned char *GRABBED_OBJECT;
extern unsigned short D_1f8001fc;
extern void ObjSetAnimFromTable(TObj *);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void startPadVibration(int, int, int, int);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = PLAYER_EXT;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        AnimJump(o, 0);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
    }
}

void playerStepGrabbedStruggle(TObj *o)
{
    int k;

    switch (o->state) {
    case 0:
        k = o->b9e;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        o->b69 = 0;
        o->b9f = 0;
        o->animFrame &= 1;
        if (k != 0) {
            if (k == 4 || k == 7)
                GRABBED_OBJECT[0] = 1;
            if (GRABBED_OBJECT[2] == 0x21)
                GRABBED_OBJECT[0xa7] = 0;
            else
                GRABBED_OBJECT[0x6a] = 0;
        }
        o->b9e = 0;
        o->bbe = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->ba4 = 0;
        o->b9d = 0;
        o->d8c = 0;
        o->wb0 = 0;
        (*(unsigned char*)&PLAYER_EXT->w08) = 0;
        (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
        PLAYER_EXT->b0a = 0;
        o->velV = 0;
        setanim(o, 0x1e);
        PLAYER_EXT->b0a = 0;
        (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0xff;
        o->substep = 0;
        o->state++;
        if ((*(unsigned char*)&o->wac) >= 2) {
            BITTEN_OBJECT = (*(unsigned char **)&PLX.p611c);
            BITTEN_OBJECT[4] = 2;
            BITTEN_OBJECT[5] = 2;
            BITTEN_OBJECT[6] = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        GVARS.i34 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        PLAYER_EXT->active = 0;
    case 1:
        o->wb0 = 0;
        o->wb6 = 0;
        AnimAdvance(o);
        if (--(*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) != 0) {
            if (D_1f8001fc) {
                if (D_1f8001fc & 0x80)
                    o->animFrame = 1;
                if (D_1f8001fc & 0x20)
                    o->animFrame = 0;
                if (D_1f8001fc & 0xf0)
                    PLAYER_EXT->b0a++;
                if (PLAYER_EXT->b0a > 10) {
                    o->b9c = 2;
                    SfxPlay2(2, 4);
                    PLAYER_EXT->b0a = 0;
                    (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
                    o->state = 3;
                    o->timer = 0;
                    o->substep = 1;
                }
            }
        } else {
            PLAYER_EXT->b0a = 0;
            (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0xff;
            o->timer = 0;
        }
        if (o->w98 != o->w9a) {
            ((unsigned char*)&GAME.playerHealth)[0] = o->w98;
            if (o->w98 <= 0) {
                o->b04 = 2;
                o->step = 3;
                o->state = 0;
            } else {
                o->state = 2;
            }
            o->w9a = o->w98;
            setanim(o, 0x10);
            PLAYER_EXT->b0a = 0;
            (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0xff;
            SfxPlay2(0x23, 0x24);
            startPadVibration(0, 0x81, 0x81, 0x3c);
            o->timer = 0x3c;
        }
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer != 0) break;
        setanim(o, 0);
        PLAYER_EXT->b0a = 0;
        (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
        o->state = 1;
        break;
    case 3:
        AnimAdvance(o);
        break;
    }
}
