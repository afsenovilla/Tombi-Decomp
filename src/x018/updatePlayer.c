// FUNC 80102214 2036 X018
// MATCHING 80102214 2036
#include "spad.h"
#include "plx.h"
#include "gv6.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
typedef struct { char pad[0x2c]; unsigned short w2c; } G330;
typedef struct { void *p[90]; } AnimTbl;
extern G330 *D_8009C330;
extern AnimTbl D_800E8404;
extern short D_1F80016A[], D_1F80016E[], D_1F800172[];
extern unsigned char D_8009F7F0;
extern unsigned short D_8007A034, D_8007A036;
extern signed char D_8009D2B0;
extern void *memcpy(void *, void *, int);
extern void FUN_80025d90(void);
extern void FUN_800f0490(TObj *);
extern void playerNormalModeDispatch(TObj *);
extern void playerEventModeDispatch(TObj *);
extern void playerStepHurtKnockback(TObj *);
extern void playerStepHurtStun(TObj *);
extern void playerStepHazardRespawn(TObj *);
extern void playerStepDie(TObj *);
extern void playerStepHurtSpin(TObj *);
extern void playerStepPerchIdle(TObj *);
extern void playerStepForcedHold(TObj *);
extern void playerStepForcedJump(TObj *);
extern void playerStepForcedLaunch(TObj *);
extern void playerStepTossed(TObj *);
extern void FUN_8003f7cc(TObj *);
extern void FUN_8001fe94(TObj *, int);

#define RESET(o) \
    { \
        o->active = 3; \
        (*(unsigned char*)((char*)&o->da0 + 1)) = 0; \
        o->b04 = 2; \
        o->step = 2; \
        o->state = 2; \
        o->substep = 0; \
        o->timer = 0x78; \
        D_8009C330->w2c = 0x2f; \
        *(AnimTbl *)tp = D_800E8404; \
        o->anim = tp[D_8009C330->w2c]; \
        FUN_8001fe94(o, 1); \
    }

void updatePlayer(TObj *o)
{
    unsigned char pad[8];
    unsigned short b0, b1;
    short f;
    void *t[90];
    void **tp;

    memcpy(pad, &GV6.s70, 8);
    b0 = SPAD.s1fc;
    b1 = SPAD.s1fe;
    U8(o, 0xe2) = 0;
    if (GVARS.b3e) {
        o->visible = 1;
        return;
    }
    if (D_8009F7F0 == 0 && GV6.s10 == 7) {
        unsigned char a, b, c;
        FUN_80025d90();
        a = U8(o, 0xd6);
        b = U8(o, 0xd5);
        c = o->bbf;
        o->bbf = 0;
        U8(o, 0xd7) = a;
        U8(o, 0xd6) = b;
        U8(o, 0xd5) = c;
    }
    if (GVARS.b3f) {
        *(volatile unsigned short *)&GV6.s70 = 0;
        SPAD.s1fc = 0;
        GV6.s74 = 0;
        SPAD.s1fe = 0;
        S16(o, 0xdc) = 0;
        S16(o, 0xde) = 0;
    }
    SPAD.b1d2 = 0;
    U8(o, 0xc7) = 0;
    switch (o->subtype) {
    case 0:
        if ((GAME.area00_eventControl & 0x40) && (*(volatile unsigned short *)&GV6.s70 & SPAD.s3c4) && o->step == 1)
            U8(o, 0xc1) = 1;
        else
            U8(o, 0xc1) = o->ba7;
        break;
    case 1:
        U8(o, 0xc1) = 1;
        o->ba7 = 1;
        (*(unsigned short*)&PLAYER.obj.w08) = D_8007A034;
        PLX.s60fc = D_8007A036;
        break;
    case 2:
        U8(o, 0xc1) = 1;
        o->ba7 = 1;
        (*(unsigned short*)&PLAYER.obj.w08) = D_8007A034;
        PLX.s60fc = D_8007A036;
        break;
    }
    {
        signed char *k = &D_8009D2B0;
        if (*k >= 5) goto z;
        if (*k < 3) {
        z:
            *k = 0;
        }
    }
    switch (o->b04) {
    case 0:
        FUN_800f0490(o);
        break;
    case 1:
        tp = t;
        if (GVARS.b38) {
            RESET(o);
            break;
        }
        if (o->step != 0x30 && S16(o, 0xe0) > 0) {
            if ((*(unsigned char*)((char*)&o->da0 + 3)) & 2) {
                o->visible ^= 1;
            } else if ((*(unsigned char*)((char*)&o->da0 + 2)) & 2) {
                o->visible ^= 1;
            } else {
                S16(o, 0xe0) = S16(o, 0xe0) - 1;
                o->visible = S16(o, 0xe0) & 1;
                if (S16(o, 0xe0) <= 0) {
                    if (U8(o, 0xcc) != 2) o->active = 1;
                    S16(o, 0xe0) = 0;
                    o->visible = 1;
                }
            }
        }
        if (o->w22 > 0 && --o->w22 <= 0) {
            o->active = 1;
            o->visible = 1;
        }
        playerNormalModeDispatch(o);
        break;
    case 2:
        if (S16(o, 0xe0) > 0) {
            S16(o, 0xe0) = S16(o, 0xe0) - 1;
            if (U8(o, 0xd1) == 0) o->visible = S16(o, 0xe0) & 1;
            if (S16(o, 0xe0) <= 0) {
                o->active = 1;
                S16(o, 0xe0) = 0;
                o->visible = 1;
            }
        }
        switch (o->step) {
        case 0: playerStepHurtKnockback(o); break;
        case 1: playerStepHurtStun(o); break;
        case 2: playerStepHazardRespawn(o); break;
        case 3: playerStepDie(o); break;
        case 4: playerStepHurtSpin(o); break;
        }
        (*(unsigned char*)&o->wa8) = 0;
        o->ba6 = 0;
        f = 0;
        if (o->b9e == 0 && o->b04 == 5) {
            f = o->step == 0x40;
            if (o->step == 0x65) f++;
        }
        if (f == 0) FUN_8003f7cc(o);
        break;
    case 3:
        o->b04 = 1;
        break;
    case 5:
        tp = t;
        if (GVARS.b38) {
            RESET(o);
            break;
        }
        if (o->w22 > 0 && --o->w22 <= 0) {
            o->active = 1;
            o->w22 = 0;
            o->visible = 1;
        }
        playerEventModeDispatch(o);
        break;
    case 6:
        tp = t;
        if (GVARS.b38) {
            RESET(o);
            break;
        }
        if (o->w22 > 0 && --o->w22 <= 0) {
            o->active = 1;
            o->w22 = 0;
            o->visible = 1;
        }
        switch (o->step) {
        case 0:
            playerStepPerchIdle(o);
            break;
        case 3:
            o->animFrame &= 1;
            playerStepForcedHold(o);
            break;
        case 4:
            o->animFrame &= 1;
            playerStepForcedJump(o);
            break;
        case 5:
            o->animFrame &= 1;
            playerStepForcedLaunch(o);
            o->d8c = 0;
            break;
        case 6:
            playerStepTossed(o);
            break;
        case 7:
            o->d8c = 0;
            playerStepForcedLaunch(o);
            if (o->b69) {
                o->b04 = 1;
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        (*(unsigned char*)&o->wa8) = 0;
        o->ba6 = 0;
        if (o->b9e == 0) FUN_8003f7cc(o);
        break;
    }
    D_1F80016A[0] = o->h->p.whole;
    D_1F80016E[0] = o->y.p.whole;
    D_1F800172[0] = o->d->p.whole;
    GAME.selectedPlane = o->d->p.whole / 90;
    memcpy(&GV6.s70, pad, 8);
    SPAD.s1fc = b0;
    SPAD.s1fe = b1;
}
