// FUNC 801066c4 852 X017
// MATCHING 801066c4 852
#include "gv6.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char pad[9]; unsigned char b9; char pad2[0x20 - 10]; short w20; } P;
extern P *PLAYER_EXT;
extern TObj *BITTEN_OBJECT;
extern void playerSetJumpRiseVelocity(TObj *);
extern void setFacingFromPad(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void playerJumpRiseUpdate(TObj *);
extern void playerBittenJumpGeneric(TObj *);
extern void playerBittenJumpType03(TObj *);
extern void playerBittenJumpType04(TObj *);
extern void playerBittenJumpType0E(TObj *);
extern void func_8011D3EC(TObj *);
extern void func_801213D0(TObj *);

void playerStepBittenJump(TObj *o)
{
    TObj *q;
    switch (o->state) {
    case 0:
        switch (BITTEN_OBJECT->type) {
        case 19:
            PLAYER_EXT->w20 = 0;
            BITTEN_OBJECT->d84 = 0;
            playerSetJumpRiseVelocity(o);
            break;
        case 0: case 2: case 8: case 10: case 11: case 18: case 30: case 33: case 40:
        case 56: case 58: case 59: case 60: case 61: case 62: case 63: case 64: case 65: case 66:
            BITTEN_OBJECT->animFrame = o->animFrame & 1;
        case 4: case 26:
            q = BITTEN_OBJECT;
            PLAYER_EXT->w20 = 0;
            {
            int t;
            Fix16 *dst = q->h;
            t = o->h->p.whole;
            dst->p.whole = (o->animFrame & 1) ? t + 8 : t - 8;
            }
            goto tail;
        case 3: case 28: case 31: case 43:
            PLAYER_EXT->w20 = 0;
            BITTEN_OBJECT->animFrame = o->animFrame & 1;
            {
            int t = o->h->p.whole;
            BITTEN_OBJECT->h->p.whole = (o->animFrame & 1) ? t + 8 : t - 8;
            }
            goto tail;
        case 41:
            if (BITTEN_OBJECT->b6a != 1) goto e41;
        case 14:
            PLAYER_EXT->w20 = 13;
            o->velY = -0x600;
            break;
        e41:
            PLAYER_EXT->w20 = 0;
            {
            int t = o->h->p.whole;
            BITTEN_OBJECT->h->p.whole = (o->animFrame & 1) ? t + 8 : t - 8;
            }
        tail:
            BITTEN_OBJECT->y.p.whole = o->y.p.whole + 8;
            BITTEN_OBJECT->d8c = o->d88 - 0xc0;
            playerSetJumpRiseVelocity(o);
            break;
        }
        setFacingFromPad(o);
        o->d88 = 0x1c0;
        o->d8c = 0;
        o->wb2 = 0;
        o->wb0 = 0;
        PLAYER_EXT->b9 = BITTEN_OBJECT->type;
        o->b69 = 0;
        o->velV = 0;
        PlayerSetAnimIfChanged(o, 13);
        if ((GAME.area00_eventControl & 0x40) && (*(volatile unsigned short *)&GV6.s70 & *(unsigned short *)0x1F8003C4)) {
            o->ba7 = 1;
        } else {
            o->ba7 = 0;
        }
        o->state = 1;
        o->substep = 0;
    case 1:
        if (PLAYER_EXT->b9 != 14) {
            playerJumpRiseUpdate(o);
        }
    case 2:
        switch (PLAYER_EXT->b9) {
        case 0: case 2: case 8: case 10: case 11: case 30: case 33: case 40:
        case 56: case 58: case 59: case 60: case 61: case 62: case 63: case 64: case 65: case 66:
            playerBittenJumpGeneric(o);
            break;
        case 3: case 18: case 28: case 31: case 43:
            playerBittenJumpType03(o);
            break;
        case 41:
            if (BITTEN_OBJECT->b6a == 1) {
                playerBittenJumpType0E(o);
                break;
            }
        case 4:
            playerBittenJumpType04(o);
            break;
        case 14:
            playerBittenJumpType0E(o);
            break;
        case 26:
            func_8011D3EC(o);
            break;
        case 19:
            func_801213D0(o);
            break;
        }
        break;
    }
}
