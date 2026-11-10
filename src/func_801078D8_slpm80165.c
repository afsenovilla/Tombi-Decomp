// VERSION SLPM_80165 x00 801078D8 1088
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerStepHazardRespawn (src/playerStepHazardRespawn_beta980203.c): the player fields from +0xD0 are
// 8 below the beta's, no pad vibration, no floor test while flying, no area 10 check, a 56-entry anim table (anim 0x2F); with D_1F8001A4
// (invincible debug flag) the respawn gives no invulnerability time.
#include "plx.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char p1[7];
    unsigned char b8;
    unsigned char p9[5];
    short wE;
    unsigned char p10[0x2c - 0x10];
    unsigned short w2c;
    unsigned short w2e;
} P800FE13C;

typedef struct {
    unsigned char pad[0xe0];
    short we0;
    unsigned char pad2[0xec - 0xe2];
    short wec, wee, wf0, wf2;
} X800FE13C;
#define X(o) ((X800FE13C *)(o))

extern P800FE13C *PLAYER_EXT;
extern TObj *BITTEN_OBJECT;
extern TObj *GRABBED_OBJECT;
extern unsigned char D_8009CF06[];
typedef struct { unsigned short c960, c962; char pad[0x5a2]; unsigned char cf06; } G960;
extern G960 GAME;
extern unsigned char SLOPE_TILT_TABLE[];
extern char D_80010A04[];
extern char D_800116B8[];
extern void FUN_8001fe94(TObj *, int);
extern void spawnImpactEffect(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void startPadVibration(int, int, int, int);
extern void FUN_8001fce4(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void playerHandleHazardContact(TObj *, int);
extern unsigned char D_1F8001A4;

typedef struct { int a[56]; } AnimTable;
extern AnimTable PLAYER_ANIM_TABLE; /* shared with ObjSetAnimFromTable (one table in the retail .rodata) */

static __inline__ void SetAnimFromTable(TObj *o)
{
    AnimTable t = PLAYER_ANIM_TABLE;
    o->anim = (void *)t.a[PLAYER_EXT->w2c];
}

void func_801078D8(TObj *o)
{
    TObj *q;
    short d;
    int a1;
    int a2;
    short v;
    P800FE13C *p;
    unsigned short f;
    unsigned char *cp;
    unsigned char t;

    switch (o->state) {
    case 0:
        o->timer = 0x40;
        *(signed char *)&o->b0f = -8;
        p = PLAYER_EXT;
        v = -0x10;
        if (o->animFrame & 1)
            v = 0x10;
        p->wE = v;
        o->d88 = 0x400;
        o->d8c = 0;
        if (o->b9e != 0)
            GRABBED_OBJECT->b6a = 0;
        (*(unsigned char*)&o->waa) = 0;
        o->ba7 = 0;
        o->b9e = 0;
        o->ba4 = 0;
        o->wb0 = 0;
        if ((*(unsigned char*)&o->wac) >= 2) {
            BITTEN_OBJECT = (*(TObj **)&PLX.p611c);
            BITTEN_OBJECT->b04 = 2;
            BITTEN_OBJECT->step = 2;
            BITTEN_OBJECT->state = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xcf) = 0;
        PLAYER_EXT->b0 = 0;
        o->animFrame &= 1;
        o->active = 4;
        o->velY = -0x800;
        o->b9d = 0;
        o->b69 = 0;
        o->velV = 0;
        a1 = S16(o, 0xda) - ((short *)o->h)[1];
        d = a1;
        d <<= 2;
        o->velH = d;
        S16(o, 0xcc) = 0xc8;
        (*(unsigned char*)&o->wac) = 0;
        a2 = S16(o, 0xde) - o->y.p.whole;
        d = a2;
        d <<= 2;
        o->velV = d;
        PLAYER_EXT->w2c = 0x10;
        PLAYER_EXT->w2e = 0x10;
        o->anim = D_80010A04;
        FUN_8001fe94(o, 0);
        spawnImpactEffect(3, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e560(0x23, 0x24);
        o->state = 1;
        break;
    case 1:
        o->d8c = (unsigned char)o->d88;
        o->d88 += PLAYER_EXT->wE;
        FUN_8001fce4(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0x680)
            o->velY = 0x680;
        if (--o->timer < 0x10) {
            o->anim = D_80010A04;
            FUN_8001fe94(o, 0);
        }
        if (o->timer == 0) {
            PLAYER_EXT->wE = 0;
            o->h->raw = S32(o, 0xd8);
            o->y.raw = S32(o, 0xdc);
            if (PLAYER_EXT->b8) {
                if (D_1F8001A4) {
                    S16(o, 0xcc) = 0;
                    o->active = 1;
                } else {
                    S16(o, 0xcc) = 0xc8;
                    o->active = 3;
                }
            }
            if (S16(o, 0xcc) == 0)
                S16(o, 0xcc) = 0xc8;
            o->active = 3;
            if (GVARS.hurtState) {
                PLAYER_EXT->b8 = 0;
                o->timer = 0x3c;
                o->state = 2;
            } else {
                PLAYER_EXT->b8 = 0;
                o->b04 = 1;
                o->step = 0;
                o->state = 0;
            }
            o->b69 = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->velY = 0;
        }
        break;
    case 2:
        U8(o, 0xc7) = 1;
        PLAYER_EXT->w2c = 0x2f;
        SetAnimFromTable(o);
        FUN_8001fe94(o, 1);
        if (--o->timer == 0) {
            o->state = 3;
            ((unsigned char*)&GV2.bb1)[0] = 0;
            GVARS.hurtState = 2;
        }
        break;
    }
}
#endif
