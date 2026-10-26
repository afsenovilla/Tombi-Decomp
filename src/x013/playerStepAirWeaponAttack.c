// FUNC 800f36e0 2060 X013
// MATCHING 800f36e0 2060
#include "player.h"
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char p1[6];
    signed char b7;
    unsigned char b8;
    unsigned char p9[0x20 - 9];
    unsigned short w20;
    unsigned char p22[6];
    unsigned short w28;
    unsigned short w2a;
    unsigned short w2c;
    unsigned short w2e;
} P800F36E0;

extern P800F36E0 *PLAYER_EXT;
extern TObj *BITTEN_OBJECT;
typedef struct { short x, y; } SXY;
extern short D_8009C944;
extern short D_8009C946;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned short D_1f8003c6;
extern unsigned char D_8009D2B2;
extern short D_8007A038[];
extern char D_80010E80[], D_80010EA0[], D_80010EC0[];
extern unsigned char D_801152E8[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern TObj *allocObjectLayer1(void);
extern void playerSetJumpRiseVelocity(TObj *);
extern void playerAttackDecelerate(TObj *);
extern void playerAirAccelerateX(TObj *);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern void FUN_80040278(TObj *, short, short);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001fec0(TObj *);
extern void setVelXFromPadLR(TObj *);
extern void adjustVelYFromPadDown(TObj *);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001e4f0(int);
extern void spawnGroundDebris(int, short, int, int);

#define SETANIM_IF(n) \
    PLAYER_EXT->w2c = n; \
    if (PLAYER_EXT->w2e != n) { \
        PLAYER_EXT->w2c = n; \
        FUN_800efc04(o); \
        FUN_8001fe94(o, 0); \
        PLAYER_EXT->w2e = PLAYER_EXT->w2c; \
    }

void playerStepAirWeaponAttack(TObj *o)
{
    int d;
    signed char c;
    signed char b;
    TObj *q;

    switch (o->state) {
    case 0:
        d = 0x10;
        o->timer = 10;
        o->d84 = 0;
        if (o->animFrame & 1) d = 0xf0;
        o->b9d = 1;
        U8(o, 0xc8) = 0;
        (*(unsigned char*)&o->da0) = 0;
        (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
        o->b9e = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->d88 = d;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        PLAYER_EXT->b0 = 2;
        PLAYER_EXT->w2e = -1;
        PLAYER_EXT->w28 = -1;
        PLAYER_EXT->w2a = -1;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3:
            (*(char **)&PLAYER.obj.anim) = D_80010E80;
            break;
        case 4: case 5:
            (*(char **)&PLAYER.obj.anim) = D_80010EA0;
            break;
        case 6: case 7:
            (*(char **)&PLAYER.obj.anim) = D_80010EC0;
            break;
        }
        FUN_8001fe94(o, 0);
        if ((*(unsigned char*)&o->wac) == 2) {
            BITTEN_OBJECT = PTR(o, 0xe4);
            PLAYER_EXT->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        (*(unsigned char*)&o->wac) = 0;
        c = S8(o, 0xe3);
        if (c < D_8007A038[D_8009D2B2]) {
            S8(o, 0xe3) = c + 1;
            q = allocObjectLayer1();
            if (q != 0) {
                q->active = 1;
                q->type = D_8009D2B2;
                q->step = 0;
                q->state = 0;
            }
        }
        if (PLAYER_EXT->b8 != 0) {
            o->state = 2;
        } else {
            o->state = 1;
        }
        break;
    case 1:
        if (o->velY >= 0) {
            PLAYER_EXT->b8 = 1;
            o->state = 2;
        }
        if (PAD_PRESSED[0] & D_1f8003c6) {
            if (PLAYER_EXT->b8 == 0) {
                if (++PLAYER_EXT->w20 >= 0xe) {
                    PLAYER_EXT->b8 = 1;
                    o->state = 2;
                }
                playerSetJumpRiseVelocity(o);
            }
        } else {
            PLAYER_EXT->b8 = 1;
            if (PLAYER_EXT->w20 >= 5) {
                o->state = 2;
            } else {
                PLAYER_EXT->w20++;
                playerSetJumpRiseVelocity(o);
            }
        }
    case 2:
        *(int *)o->h += D_8009C944 << 8;
        o->y.raw += D_8009C946 << 8;
        playerAttackDecelerate(o);
        playerAirAccelerateX(o);
        o->h->raw += o->velX << 8;
        FUN_8010f0f4(o);
        FUN_8001fd94(o);
        if ((*(unsigned char*)&o->wac) == 2) {
            BITTEN_OBJECT = PTR(o, 0xe4);
            PLAYER_EXT->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        if (o->velY > 0) {
            d = 0x10;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1) d = 0xf0;
            (*(unsigned char*)&o->wac) = 1;
            o->b9c = 2;
            o->d88 = d;
            o->d8c = 0;
            o->state = 3;
        }
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10);
        if (FUN_8003facc(o)) {
            d = 0x10;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1) d = 0xf0;
            (*(unsigned char*)&o->wac) = 1;
            o->b9c = 2;
            o->d88 = d;
            o->d8c = 0;
            o->state = 3;
            o->velY = 0;
            o->velV = 0;
        }
        if (U8(o, 0xc8) != 0) {
            PLAYER_EXT->b8 = 0;
            (*(unsigned char*)&o->wac) = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->step = 0x32;
            o->state = 0;
            break;
        }
        if (PLAYER_EXT->b0 != 0) break;
        if (U8(o, 0xc6) != 0) break;
        (*(unsigned char*)&o->wac) = 0;
        SETANIM_IF(4);
        d = 0x10;
        o->b9d = 0;
        o->b9c = 1;
        o->d84 = 0;
        if (o->animFrame & 1) d = 0xf0;
        o->d88 = d;
        o->d8c = 0;
        o->step = 2;
        o->state = 2;
        break;
    case 3:
        *(int *)o->h += D_8009C944 << 8;
        o->y.raw += D_8009C946 << 8;
        playerAttackDecelerate(o);
        o->d8c = 0;
        PLAYER_EXT->b8 = 1;
        if (U8(o, 0xc9) != 0) {
            FUN_8001fec0(o);
            *(int *)o->h += D_8009C944 << 7;
            o->y.raw += D_8009C946 << 7;
            setVelXFromPadLR(o);
            o->h->raw += o->velX << 8;
            if (--o->timer <= 0) {
                o->timer = 0;
                adjustVelYFromPadDown(o);
                FUN_8001fd94(o);
            }
        } else {
            playerAirAccelerateX(o);
            o->h->raw += o->velX << 8;
            FUN_8010f0f4(o);
            FUN_8001fd94(o);
        }
        if ((*(unsigned char*)&o->wac) == 2) {
            BITTEN_OBJECT = PTR(o, 0xe4);
            PLAYER_EXT->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        if (U8(o, 0xc8) != 0) {
            PLAYER_EXT->b8 = 0;
            (*(unsigned char*)&o->wac) = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->step = 0x32;
            o->state = 0;
            break;
        }
        if (o->b69 == 1 || FUN_8003fd78(o, 0, 0) != 0) {
            FUN_8001e5f4(0x1c, 0x7f);
            PLAYER_EXT->b8 = 0;
            o->step = 3;
            o->state = 1;
            o->ba7 = 0;
            (*(unsigned char*)&o->wac) = 0;
            o->b9c = 0;
            o->d8c = D_801152E8[o->wb0];
            if (o->bbe & 0x20) {
                FUN_8001e4f0(0x3e);
                spawnGroundDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
                spawnGroundDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
            }
            break;
        }
        if (PLAYER_EXT->b0 != 0) break;
        if (U8(o, 0xc6) != 0) break;
        SETANIM_IF(4);
        d = 0x10;
        b = PLAYER_EXT->b7;
        o->b9d = 0;
        (*(unsigned char*)&o->wac) = 1;
        o->d84 = 0;
        o->animFrame = b;
        o->b9c = 2;
        if (o->animFrame & 1) d = 0xf0;
        o->d88 = d;
        o->d8c = 0;
        o->step = 2;
        o->state = 3;
        break;
    }
}
