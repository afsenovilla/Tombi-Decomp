// VERSION SCUS_94236 x00 80101574 2020
// FLAGS -O2 -G0
#if defined(VERSION_SCUS_94236)
// USA version of PAL x000 updatePlayer (src/updatePlayer.c): US globals and callee names.
#include "spad.h"
#include "plx.h"
#include "gv6.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
#include "player.h"
typedef struct { char pad[0x2c]; unsigned short w2c; } G330;
typedef struct { void *p[90]; } AnimTbl;
extern G330 *D_8009B698;
extern AnimTbl D_800E7764;
extern short PLAYER_H_WHOLE[], PLAYER_Y_WHOLE[], PLAYER_D_WHOLE[];
extern unsigned char D_8009EB58[], D_8009BCA6[], D_8009BCA7[], D_8009BCA0[], D_8009BCCC[];
extern unsigned short D_8007D5CC[], D_8007D5CE[], D_800A53A0[], D_800A545C[], D_8009C9D8[], D_8009C9DC[], D_8009C978[];
extern int D_8009BCEC[];
extern signed char D_8009C618[];
extern void *func_8005B7E0(void *, void *, int);
extern void func_800288C4(void);
extern void playerInitMode(TObj *);
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
extern void func_80042C20(TObj *);
extern void func_80022A24(TObj *, int);

#define RESET(o) \
    { \
        o->active = 3; \
        (*(unsigned char*)((char*)&o->da0 + 1)) = 0; \
        o->b04 = 2; \
        o->step = 2; \
        o->state = 2; \
        o->substep = 0; \
        o->timer = 0x78; \
        D_8009B698->w2c = 0x2f; \
        *(AnimTbl *)tp = D_800E7764; \
        o->anim = tp[D_8009B698->w2c]; \
        func_80022A24(o, 1); \
    }

void func_80101574(TObj *o)
{
    unsigned char pad[8];
    unsigned short b0, b1;
    short f;
    void *t[90];
    void **tp;

    func_8005B7E0(pad, D_8009C9D8, 8);
    b0 = SPAD.s1fc;
    b1 = SPAD.s1fe;
    U8(o, 0xe2) = 0;
    if (D_8009BCA6[0]) {
        o->visible = 1;
        return;
    }
    if (D_8009EB58[0] == 0 && D_8009C978[0] == 7) {
        unsigned char a, b, c;
        func_800288C4();
        a = U8(o, 0xd6);
        b = U8(o, 0xd5);
        c = o->bbf;
        o->bbf = 0;
        U8(o, 0xd7) = a;
        U8(o, 0xd6) = b;
        U8(o, 0xd5) = c;
    }
    if (D_8009BCA7[0]) {
        *(volatile unsigned short *)D_8009C9D8 = 0;
        SPAD.s1fc = 0;
        D_8009C9DC[0] = 0;
        SPAD.s1fe = 0;
        S16(o, 0xdc) = 0;
        S16(o, 0xde) = 0;
    }
    SPAD.b1d2 = 0;
    U8(o, 0xc7) = 0;
    switch (o->subtype) {
    case 0:
        if ((D_8009BCEC[0] & 0x40) && (*(volatile unsigned short *)D_8009C9D8 & SPAD.s3c4) && o->step == 1)
            U8(o, 0xc1) = 1;
        else
            U8(o, 0xc1) = o->ba7;
        break;
    case 1:
        U8(o, 0xc1) = 1;
        o->ba7 = 1;
        D_800A53A0[0] = D_8007D5CC[0];
        D_800A545C[0] = D_8007D5CE[0];
        break;
    case 2:
        U8(o, 0xc1) = 1;
        o->ba7 = 1;
        D_800A53A0[0] = D_8007D5CC[0];
        D_800A545C[0] = D_8007D5CE[0];
        break;
    }
    {
        signed char *k = D_8009C618;
        if (*k >= 5) goto z;
        if (*k < 3) {
        z:
            *k = 0;
        }
    }
    switch (o->b04) {
    case 0:
        playerInitMode(o);
        break;
    case 1:
        tp = t;
        if (D_8009BCA0[0]) {
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
                    o->active = 1;
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
        if (f == 0) func_80042C20(o);
        break;
    case 3:
        o->b04 = 1;
        break;
    case 5:
        tp = t;
        if (D_8009BCA0[0]) {
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
        if (D_8009BCA0[0]) {
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
        if (o->b9e == 0) func_80042C20(o);
        break;
    }
    PLAYER_H_WHOLE[0] = o->h->p.whole;
    PLAYER_Y_WHOLE[0] = o->y.p.whole;
    PLAYER_D_WHOLE[0] = o->d->p.whole;
    D_8009BCCC[0] = o->d->p.whole / 90;
    func_8005B7E0(D_8009C9D8, pad, 8);
    SPAD.s1fc = b0;
    SPAD.s1fe = b1;
}
#endif
