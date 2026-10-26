// FUNC 800fe13c 1568 X011
// MATCHING 800fe13c 1568
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
extern unsigned char D_801152E8[];
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

typedef struct { int a[90]; } AnimTable;
extern AnimTable D_800E8404; /* shared with ObjSetAnimFromTable (one table in the retail .rodata) */

static __inline__ void SetAnimFromTable(TObj *o)
{
    AnimTable t = D_800E8404;
    o->anim = (void *)t.a[PLAYER_EXT->w2c];
}

void playerStepHazardRespawn(TObj *o)
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
        if (o->b9e != 0) {
            if (o->b9e == 4 || o->b9e == 7)
                GRABBED_OBJECT->active = 1;
            q = GRABBED_OBJECT;
            if (q->type == 0x21)
                q->ba7 = 0;
            else
                q->b6a = 0;
        }
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
        GVARS.i34 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        PLAYER_EXT->b0 = 0;
        o->animFrame &= 1;
        o->active = 4;
        o->velY = -0x800;
        o->b9d = 0;
        U8(o, 0xc3) = 0;
        o->visible = 1;
        o->b69 = 0;
        o->velV = 0;
        a1 = S16(o, 0xee) - ((short *)o->h)[1];
        d = a1;
        d <<= 2;
        o->velH = d;
        S16(o, 0xe0) = 0x8c;
        (*(unsigned char*)((char*)&o->da0 + 1)) = 0;
        (*(unsigned char*)&o->wac) = 0;
        a2 = S16(o, 0xf2) - o->y.p.whole;
        d = a2;
        d <<= 2;
        o->velV = d;
        PLAYER_EXT->w2c = 0x10;
        PLAYER_EXT->w2e = 0x10;
        o->anim = D_80010A04;
        FUN_8001fe94(o, 0);
        spawnImpactEffect(3, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_8001e560(0x23, 0x24);
        startPadVibration(0, 0x81, 0x81, 0x3c);
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
        FUN_8003facc(o);
        if (--o->timer < 0x10) {
            o->anim = D_80010A04;
            FUN_8001fe94(o, 0);
        }
        if (o->timer == 0) {
            PLAYER_EXT->wE = 0;
            o->h->raw = S32(o, 0xec);
            o->y.raw = S32(o, 0xf0);
            if (PLAYER_EXT->b8) {
                S16(o, 0xe0) = 0x8c;
                o->active = 3;
            }
            if (S16(o, 0xe0) == 0)
                S16(o, 0xe0) = 0x8c;
            o->active = 3;
            if (GVARS.b38) {
                PLAYER_EXT->b8 = 0;
                o->timer = 0xb4;
                o->state = 2;
            } else if (U8(o, 0xd1) == 2) {
                o->h->p.whole = X(o)->wee;
                o->timer = 10;
                o->state = 4;
                o->y.p.whole = X(o)->wf2;
            } else {
                o->state = 9;
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
        PLAYER_EXT->w2c = 0x3c;
        SetAnimFromTable(o);
        FUN_8001fe94(o, 1);
        if (--o->timer == 0) {
            o->state = 3;
            GAME.cf06 = 0;
            ((unsigned char*)&GV2.bb1)[0] = 0;
            if (GAME.c960 != 10 || GAME.c962 != 0)
                GVARS.b38 = 2;
        }
        break;
    case 4:
        o->d8c = (unsigned char)o->d88;
        o->d88 += PLAYER_EXT->wE;
        o->y.raw += 0x50000;
        if (o->b69 || FUN_8003fd78(o, 0, 0)) {
            o->d8c = 0;
            o->state++;
        }
        if (--o->timer <= 0) {
            o->d8c = 0;
            o->state = 9;
        }
        playerHandleHazardContact(o, 0);
        break;
    case 5:
        o->anim = D_800116B8;
        FUN_8001fe94(o, 0);
        o->state++;
    case 6:
        o->y.raw += 0x50000;
        if (AnimAdvance(o))
            o->state = 9;
        FUN_8003fd78(o, 0, 0);
        playerHandleHazardContact(o, 0);
        break;
    case 9:
        U8(o, 0xd1) = 0;
        PLAYER_EXT->b8 = 0;
        t = D_801152E8[o->wb0];
        o->b04 = 1;
        o->step = 2;
        o->state = 3;
        o->d8c = t;
        break;
    case 3:
    case 7:
    case 8:
        break;
    }
}
