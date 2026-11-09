// FUNC 80035428 580 MAIN0
// MATCHING 80035428 580
// VERSION SLPS_01144 slps01144 80036350 500
// VERSION BETA_980203 beta980203 80037B90 500
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern short probeCollisionAtDepthB(TObj *, short, short);
extern void spawnWeaponHitDebris(short, short, short, unsigned char);
extern void spawnImpactEffect(int, short, short, short);
extern void FUN_8001e4f0(int);
extern short probeFloorPlaneAt(void *, short, short);
extern void weapon8to9AttachAnchor(TObj *);
extern unsigned char DAT_800a60d6;
extern unsigned short DAT_1f800282;
extern unsigned short COLLISION_HIT_FLAGS;
extern TObj *PLAYER_EXT;
extern char PLAYER[];
extern short *PLAYER_H;
extern unsigned short PLAYER_Y;
extern unsigned short DAT_800a60a8;

int func_80036350(TObj *o)
{
    short n;
    int k;

    if (o->active == 1 && probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole)) {
        o->active = 2;
        o->ba5 = 0;
        o->b69 = 0;
        if (DAT_800a60d6 == 0) {
            if (!(DAT_1f800282 & 0x800)) {
                if (DAT_1f800282 & 2) o->b69 = 1;
            }
        }
        if (!(COLLISION_HIT_FLAGS & 0x3000)) {
            switch (k) {
            case 1:
            case 2:
            case 3:
        k = (COLLISION_HIT_FLAGS >> 5) & 0xf;
                break;
                spawnWeaponHitDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, o->animFrame);
            }
        }
        spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e4f0(5);
        o->wa8 = 0x4ff;
    }
    if (o->b69 != 0 && DAT_800a60d6 != 4 && DAT_800a60d6 != 7) {
    n = 0;
    switch ((signed char)PLAYER_EXT->substep) {
    case 0:
    case 2:
        if (probeFloorPlaneAt(PLAYER, PLAYER_H[1] + 0x10, PLAYER_Y + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    case 1:
    case 3:
        if (probeFloorPlaneAt(PLAYER, PLAYER_H[1] - 0x10, PLAYER_Y + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    }
    if (n == 0) {
        weapon8to9AttachAnchor(o);
        return 1;
    }
    return 0;
    }
    o->b69 = 0;
    return 0;
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 80035428 (weapon8to9HitWall.c): PAL C of src/weapon8to9HitWall.c (weapon8to9HitWall, twin score 0.870), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
extern short probeCollisionAtDepthB(TObj *, short, short);
extern void spawnWeaponHitDebris(short, short, short, unsigned char);
extern void spawnImpactEffect(int, short, short, short);
extern void FUN_8001e4f0(int);
extern short probeFloorPlaneAt(void *, short, short);
extern void weapon8to9AttachAnchor(TObj *);
extern unsigned char DAT_800a60d6;
extern unsigned short DAT_1f800282;
extern unsigned short COLLISION_HIT_FLAGS;
extern TObj *PLAYER_EXT;
extern char PLAYER[];
extern short *PLAYER_H;
extern unsigned short PLAYER_Y;
extern unsigned short DAT_800a60a8;

int weapon8to9HitWall(TObj *o)
{
    short n;
    int k;

    if (o->active == 1 && probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole)) {
        o->active = 2;
        o->ba5 = 0;
        o->b69 = 0;
        if (DAT_800a60d6 == 0) {
            if (!(DAT_1f800282 & 0x800)) {
                if (DAT_1f800282 & 2) o->b69 = 1;
            }
        }
        if (!(COLLISION_HIT_FLAGS & 0x3000)) {
            switch (k) {
            case 1:
            case 2:
            case 3:
        k = (COLLISION_HIT_FLAGS >> 5) & 0xf;
                break;
                spawnWeaponHitDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, o->animFrame);
            }
        }
        spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e4f0(5);
        o->wa8 = 0x4ff;
    }
    if (o->b69 != 0 && DAT_800a60d6 != 4 && DAT_800a60d6 != 7) {
    n = 0;
    switch ((signed char)PLAYER_EXT->substep) {
    case 0:
    case 2:
        if (probeFloorPlaneAt(PLAYER, PLAYER_H[1] + 0x10, PLAYER_Y + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    case 1:
    case 3:
        if (probeFloorPlaneAt(PLAYER, PLAYER_H[1] - 0x10, PLAYER_Y + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    }
    if (n == 0) {
        weapon8to9AttachAnchor(o);
        return 1;
    }
    return 0;
    }
    o->b69 = 0;
    return 0;
}
#else
#include "TOBJ.H"
extern short probeCollisionAtDepthB(TObj *, short, short);
extern void spawnWeaponHitDebris(short, short, short, unsigned char);
extern void spawnImpactEffect(int, short, short, short);
extern void FUN_8001e4f0(int);
extern short probeFloorPlaneAt(void *, short, short);
extern void weapon8to9AttachAnchor(TObj *);
extern unsigned char DAT_800a60d6;
extern unsigned short DAT_1f800282;
extern unsigned short COLLISION_HIT_FLAGS;
extern TObj *PLAYER_EXT;
extern char PLAYER[];
extern short *PLAYER_H;
extern unsigned short PLAYER_Y;
extern unsigned short DAT_800a60a8;

int weapon8to9HitWall(TObj *o)
{
    short n;
    int k;

    if (o->active == 1 && probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole)) {
        o->active = 2;
        o->ba5 = 0;
        o->b69 = 0;
        if (DAT_800a60d6 == 0) {
            if (!(DAT_1f800282 & 0x800)) {
                if (DAT_1f800282 & 2) o->b69 = 1;
            }
        }
        k = (COLLISION_HIT_FLAGS >> 5) & 0xf;
        if (!(COLLISION_HIT_FLAGS & 0x3000)) {
            switch (k) {
            case 1:
            case 2:
            case 3:
                spawnWeaponHitDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, o->animFrame);
                break;
            }
        }
        spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e4f0(5);
        o->wa8 = 0x4ff;
    }
    if (o->b69 != 0 && DAT_800a60d6 != 4 && DAT_800a60d6 != 7) {
    n = 0;
    switch ((signed char)PLAYER_EXT->substep) {
    case 0:
    case 2:
        if (probeFloorPlaneAt(PLAYER, PLAYER_H[1] + 0x10, PLAYER_Y + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    case 1:
    case 3:
        if (probeFloorPlaneAt(PLAYER, PLAYER_H[1] - 0x10, PLAYER_Y + DAT_800a60a8)) {
            o->b69 = 0;
            n++;
        }
        break;
    }
    if (n == 0) {
        weapon8to9AttachAnchor(o);
        return 1;
    }
    return 0;
    }
    o->b69 = 0;
    return 0;
}
#endif
