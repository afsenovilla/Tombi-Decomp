// FUNC 800ff610 2228 X009
// MATCHING 800ff610 2228
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
typedef struct { char pad[8]; unsigned char b8; char pad2[0x23]; unsigned short w2c; } G330;
typedef struct { void *p[90]; } AnimTbl;
extern G330 *D_8009C330;
extern AnimTbl D_800E8404;
extern unsigned char D_8009C970A[];
#define D_8009C970 D_8009C970A[0]
extern void playerUpdateWaterLevel(TObj *);
extern short getHazardDamage();
extern void decreaseObjectTimer(TObj *, unsigned char);
extern void FUN_8001e5f4(int, int);
extern void startPadVibration(int, int, int, int);
extern void FUN_8001f110(int);
extern int playJingle(int);
extern void FUN_8001fe94(TObj *, int);

static __inline__ short calc(void)
{
    short c = GCD.bcffb != 0;
    if (GCD.bcdac == 0xff)
        c++;
    if (GAME.selectedArea == 0 && GAME.selectedSection < 2)
        c = 0;
    if (GAME.displayExpBar == 1)
        c = 0;
    return c;
}


#define SET(a, b) { S16(o, 0xee) = a; S16(o, 0xf2) = b; break; }

void playerHandleHazardContact(TObj *o, int arg)
{
    short flag;
    unsigned char r;
    AnimTbl t;

    flag = 0;
    if (!(o->active & 1))
        return;
    playerUpdateWaterLevel(o);
    if ((*(unsigned char*)((char*)&o->da0 + 1)) == 0)
        return;
    switch ((*(unsigned char*)((char*)&o->da0 + 1))) {
    case 1:
        switch (GAME.selectedArea) {
        case 0:
            if (o->d->p.whole == 0x5a) {
                if (o->h->p.whole < 0x641)
                    break;
                SET(0xa25, -0x201);
            } else {
                short v = o->h->p.whole;
                if (v >= 0xb5f) SET(0xb62, -0x1bc)
                else if (v >= 0xae7) SET(0xae8, -0x198)
                else if (v >= 0xa5b) SET(0xa53, -0x1a0)
            }
            break;
        case 1:
            if (GAME.selectedSection != 3)
                break;
            SET(0x4c7, -0x210);
        case 3:
            U8(o, 0xcd) = 0;
            U8(o, 0xce) = 0;
            if (o->active != 1)
                goto clr;
            o->animFrame = 0;
            decreaseObjectTimer(o, getHazardDamage(o));
            o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
            return;
        case 9:
            if (GAME.selectedSection != 0)
                break;
            SET(0xed4, -0x1fb);
        case 10:
            switch (GAME.selectedSection) {
            case 0:
            case 4:
                if (GAME.selectedSpawnPoint == 3) {
                    if (o->d->p.whole == 0x5a) {
                        if (o->h->p.whole >= 0xadd) SET(0xafa, -0xc8)
                        else SET(0xa24, -0x58)
                    } else SET(0x9e6, -0x12c)
                } else {
                    if (o->d->p.whole == 0x5a) {
                        if (o->h->p.whole >= 0xadd) SET(0xa28, -0x5c)
                        else SET(0x6d2, -0xc0)
                    } else SET(0x6cc, -0x52)
                }
                break;
            case 3:
            case 7:
                if (!(GAME.purifiedAreas & 0x40))
                    goto kill;
                {
                    short v = o->h->p.whole;
                    if (v < 0x302) SET(0x2cc, -0x420)
                    else if (v < 0x47e) SET(0x44a, -0x414)
                    else if (v < 0x654) SET(0x591, -0x434)
                    else SET(0x78b, -0x3b4)
                }
                break;
            }
            break;
        case 14:
            if (GAME.selectedSection == 2)
                break;
            if (GVARS.b3f)
                return;
        kill:
            U8(o, 0xcd) = 0;
            U8(o, 0xce) = 0;
            if (o->active != 1)
                goto clr;
            decreaseObjectTimer(o, 2);
            o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        clr:
            (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
            return;
        }
        break;
    case 2:
        flag = calc();
        if (flag)
            break;
        U8(o, 0xd1) = 2;
        switch (GAME.selectedArea) {
        case 0:
            if (GAME.selectedSection == 5) SET(0xd2, -0x12f)
            if (o->d->p.whole == 0x5a) {
                if (o->h->p.whole < 0x641) SET(0x4b2, -0x114)
            } else SET(0x4b2, -0xeb)
            break;
        case 1:
            if (GAME.selectedSection != 2)
                break;
            SET(0x572, -0x110);
        case 4:
            switch (GAME.selectedSection) {
            case 6: SET(0x6f, -0x74);
            case 7: SET(0x88, -0x54);
            case 8: SET(0x8f, -0xbc);
            case 14: return;
            case 16: SET(0x2d, -0x48);
            }
            break;
        case 10:
            switch (GAME.selectedSection) {
            case 3:
            case 7:
                SET(0x5f, -0x40c);
            case 1:
            case 5:
                SET(0xb4, -0xe8);
            }
            break;
        }
        break;
    case 3:
        switch (GAME.selectedArea) {
        case 4:
            break;
        case 14:
            return;
        }
        break;
    }
    if (flag) {
        if (o->b04 == 2)
            return;
        if (o->step == 0x3d || o->step == 0x3e) return;
        if (o->step == 0x42 || o->step == 0x43) return;
        if (o->step == 0x44) return;
        if (o->step == 0x47 || o->step == 0x48) return;
        o->b04 = 1;
        o->step = 0x3d;
        U8(o, 0xcd) = 0;
        U8(o, 0xce) = 0;
        o->state = 0;
        GVARS.s44 = 0;
        GVARS.s46 = -0x40;
        (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
        FUN_8001e5f4(0xe, 0x7f);
        return;
    }
    if (o->active == 1 && arg != 0) {
        decreaseObjectTimer(o, getHazardDamage(o));
        if (SPAD.b1a4 == 0 && o->w9a != o->w98) {
            o->w9a = o->w98;
            D_8009C330->b8 = 1;
            D_8009C970 = o->w98;
            if (o->w98 <= 0) {
                GVARS.b42 = 1;
                GVARS.b38 = 1;
                FUN_8001f110(0);
                playJingle(3);
            }
        }
    }
    startPadVibration(0, 0x81, 0x81, 0x3c);
    o->active = 3;
    U8(o, 0xc7) = 1;
    U8(o, 0xcd) = 0;
    U8(o, 0xce) = 0;
    (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
    o->b04 = 2;
    o->step = 2;
    o->state = 0;
    o->substep = 0;
    if (GVARS.b38) {
        o->timer = 0xb4;
        D_8009C330->w2c = 0x2f;
        t = D_800E8404;
        o->anim = t.p[D_8009C330->w2c];
        FUN_8001fe94(o, 1);
        o->state = 2;
    }
}
