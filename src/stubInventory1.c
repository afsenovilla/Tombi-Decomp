// FUNC 8002b010 8 MAIN0
// HEADERS tomba24
// MATCHING 8002b010 8
// Portado de psx_tomba (main/game/inventory.c, stubInventory1); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

void addPlayerAP(int arg0);

void spawnItemNotification(s32 arg0, u8 arg1);

u_char addItemToInventory(u_long item_id, u_char qty, bool printMessage);

int removeItemFromInventory(ITEM id, int qty);

u_long increaseMaxHealth(void);

u8 applyGoldenBowl(void);

int tryStartItemInteraction(int arg0, char arg1, int arg2);

void updateInventoryOverlay(void);

s32 stepPlayerXToward(s32 arg0);

typedef struct {
    char p0[7]; u8 step;
    char p8[0x20 - 8]; s32 d20; s32 d24;
    char p28[0x3f - 0x28]; u8 b3f;
    char p40[0x4a - 0x40]; s16 w4a, w4c; s16 w4e; s16 w50, w52; s16 w54; s16 w56;
    s16 w58; s16 w5a; s16 w5c; s16 w5e, w60, w62;
} S_29CDC;
#define F6 D_1F8000F6[0]

static __inline__ s32 body_29CDC(S_29CDC *s)
{
    s32 r;
    s16 n, v;
    volatile s16 *ee;
    r = 0;
    switch (s->step) {
    case 0:
        ee = D_1F8000EE;
        s->w4c -= *ee;
        s->w50 -= F6;
        s->w52 = *ee;
        s->w56 = F6;
        s->w62 = 0;
        s->w5e = 0;
        s->w60 = 0;
        s->w4a = 0;
        s->w5a = 0;
        s->step++;
        break;
    case 1:
        if (s->w4a != 90) {
            s->w4a++;
            if (s->w4a == 26) {
                D_8009BCA4 = 0;
                D_8009BCDD = 3;
            }
            D_1F8000EE[0] = s->w52 + s->w4c * s->w4a / 90;
            F6 = s->w56 + s->w50 * s->w4a / 90;
        }
        n = s->w4a;
        if (n != 90) {
            if (n >= 45) n = 90 - n;
            if (n < 32) s->w60 = ((n & 0x38) << 1) | 0x80;
            else s->w60 = 0x120;
        } else {
            s->w60 = 0x80;
        }
        if (s->b3f == 0) {
            s->w5e += s->w60;
            if (s->w5e >= 0x5a00) {
                s->w5e = 0x5a00;
                s->w5a++;
            }
            if (s->w5e <= 0x2d00) goto inc;
            if (s->w5e >= 0x4600) goto dec;
        } else {
            s->w5e -= s->w60;
            if (s->w5e < -0x59ff) {
                s->w5e = -0x5a00;
                s->w5a++;
            }
            if (s->w5e >= -0x2d00) {
            inc:
                s->w62 += 0x80;
                if (s->w62 > 0x1200) s->w62 = 0x1200;
            } else if (s->w5e < -0x1bff) {
            dec:
                s->w62 -= 0x80;
                if (s->w62 < 0) s->w62 = 0;
            }
        }
        s->d24 = s->w5e;
        s->d20 = s->w62;
        if (s->w5a != 0 || D_8009BCDD == 1) s->step++;
        break;
    case 2:
        r = 1;
        break;
    }
    return r;
}

s32 func_80029CDC(S_29CDC *s);
#undef F6

void dispatchAreaItemHandler(s32 arg0);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002A0A0);

typedef struct { u8 b0; char p[0x15]; s16 w16; } S_2A240;
typedef struct { s16 w0; s16 w2; } P_2A240;
typedef struct { char p0[0x30]; s16 w30; s16 w32; P_2A240 *d34; } O_2A240;

void func_8002A240(O_2A240 *o);

void syncObjectTargetY(u8* self);

typedef struct {
    char p0[0x2c];
    s16 s2c, s2e, s30, s32;
    Fix16 *f34;
} S_2A334;
typedef struct { char p[0x14]; s32 d14; } G_2A334;

void func_8002A334(S_2A334 *o);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002A480);

typedef struct O_2A798 { char p0[0x14]; s32 y; char p1[0x2c-0x18]; s16 h2c; s16 h2e; char p2[0x34-0x30]; s32 *d34;
  char p3[0x3c-0x38]; u8 b3c; char p4[0x44-0x3d]; u16 h44; char p5[0x58-0x46]; u16 w58; } O_2A798;
typedef struct G_2A798 { char p0[0x9c]; u8 f9c; } G_2A798;

void func_8002A798(O_2A798 *o);

#define D34(o) (*(Fix16 **)&(o)->unk34)
#define W44(o) (*(u16 *)&(o)->d)

static __inline__ void body_2A9FC(GameObject *o)
{
    s32 d;
    u8 neg;
    d = D_1F80018C - D34(o)->raw;
    neg = d < 0;
    if (d > 0x20000) {
        if (d < o->y.raw) {
            if (d < 0x40000)
                o->y.raw = d;
            else
                o->y.raw = 0x40000;
        } else {
            if (o->y.raw < 0)
                o->y.raw = 0;
            o->y.raw += 0x2000;
        }
        { s32 y = o->y.raw; D34(o)->raw += y; }
    } else if (d < -0x20000) {
        if (o->y.raw < d) {
            if (d > -0x40000)
                o->y.raw = d;
            else
                o->y.raw = -0x40000;
        } else {
            if (o->y.raw > 0)
                o->y.raw = 0;
            o->y.raw -= 0x2000;
        }
        D34(o)->raw += o->y.raw;
        if (D34(o)->p.whole < (s16)o->animTimer) {
            D34(o)->raw = (s16)o->animTimer << 16;
            o->y.raw = 0;
            W44(o) |= 2;
        }
        return;
    } else {
        o->y.raw = d;
        D34(o)->raw += d;
        if (neg) {
            if (D34(o)->p.whole < (s16)o->animTimer) {
                D34(o)->raw = (s16)o->animTimer << 16;
                o->y.raw = 0;
                W44(o) |= 2;
            }
        }
    }
    if (D34(o)->p.whole > (s16)o->animFrame) {
        D34(o)->raw = (s16)o->animFrame << 16;
        o->y.raw = 0;
        W44(o) |= 1;
    }
}

void func_8002A9FC(GameObject *o);
#undef D34
#undef W44

typedef struct {
    char p0[0x18]; s32 v; char p1[0x30 - 0x1c]; s16 s30, s32; char p2[0x44 - 0x34]; u16 f44;
} O_2ABC0;
typedef union { s32 i; struct { s16 lo, hi; } s; } FX_2ABC0;
#define VF (*(volatile s32 *)&(*(FX_2ABC0 *)&D_1F8000F0).i)

static __inline__ void inl_2ABC0(O_2ABC0 *o)
{
    s32 d = D_1F800190 - VF;
    if (d >= 0) {
        if (d > 0x10000) {
            if (d < o->v) {
                if (d > 0x3ffff) o->v = 0x40000;
                else o->v = d;
            } else {
                if (o->v < 0) o->v = 0;
                o->v += 0x8000;
            }
            VF += o->v;
            if ((*(FX_2ABC0 *)&D_1F8000F0).s.hi > o->s32) {
                (*(FX_2ABC0 *)&D_1F8000F0).i = o->s32 << 16;
                o->v = 0;
                o->f44 |= 8;
            }
        } else {
            VF = D_1F800190;
            if ((*(FX_2ABC0 *)&D_1F8000F0).s.hi > o->s32) {
                (*(FX_2ABC0 *)&D_1F8000F0).i = o->s32 << 16;
                o->v = 0;
                o->f44 |= 8;
            }
        }
    } else {
        if (d < -0x40000) {
            if (o->v < d) {
                if (d > -0x40000) o->v = d;
                else o->v = -0x40000;
            } else {
                if (o->v > 0) o->v = 0;
                o->v -= 0x4000;
            }
        } else {
            o->v = d;
        }
        VF += o->v;
        if ((*(FX_2ABC0 *)&D_1F8000F0).s.hi < o->s30) {
            (*(FX_2ABC0 *)&D_1F8000F0).i = o->s30 << 16;
            o->v = 0;
            o->f44 |= 4;
        }
    }
}

void func_8002ABC0(O_2ABC0 *o);
#undef VF

#define W(o) (*(s32 *)((o) + 0x18))

void func_8002AD74(u8 *o);
#undef W

s32 stepScrollReturnY(u8* self);

s32 stepScrollApproachY(u8* self);

s32 stepScrollApproachX(u8* self);

s32 dampCameraAngle(void);

s32 stepScrollReturnX(u8* self);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002B110);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002B278);

void func_8002B110();
void func_8002B278();

static __inline__ void decay_2B3E8(void)
{
    s16 v = D_1F8000E6;
    if (v == 0) return;
    if (v > 0) {
        v -= 2;
        D_1F8000E6 = v;
        if (v < 0) D_1F8000E6 = 0;
    } else {
        v += 2;
        D_1F8000E6 = v;
        if (v > 0) D_1F8000E6 = 0;
    }
}

void func_8002B3E8(char *o, s32 a);

s32 stepScrollAndAngle(u8* self);

s32 stepScrollClearX(u8* self);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002B6A8);

typedef struct { char p[0x24]; s32 d24; char q[0x70 - 0x28]; s8 b70, b71, b72, b73; } S_2B704;
static __inline__ s32 approach_2B704(S_2B704 *o)
{
    switch (o->b71) {
    case 0:
        if ((o->b73 << 8) < o->d24) {
            o->d24 -= 0x80;
            return 0;
        }
        return 1;
    case 1:
        if (o->d24 >= (o->b73 << 8)) return 1;
        o->d24 += 0x80;
        return 0;
    }
    return 0;
}
static __inline__ s32 decel_2B704(S_2B704 *o)
{
    s32 v = o->d24;
    if (v != 0) {
        if (v > 0) v -= 0x80;
        else v += 0x80;
        o->d24 = v;
        return 0;
    }
    o->b71 = 0;
    o->b72 = 0;
    o->b73 = 0;
    return 1;
}

void func_8002B704(S_2B704 *o);

s32 handleLookScrollInput(u8* self);

typedef struct {
    char p0[0x20];
    s32 spd;
    char p24[0x30 - 0x24];
    s16 w30, w32;
    char p34[0x6c - 0x34];
    s8 b6c, b6d, b6e, b6f;
    char p70[0x75 - 0x70];
    u8 b75, b76, b77;
} S_2BB9C;
typedef struct {
    char p0[0x9e];
    u8 b9e;
    char p9f;
    union { s32 i; u8 b[4]; } a0;
} G_2BB9C;

static __inline__ s16 ramp_2BB9C(S_2BB9C *o)
{
    switch (o->b6d) {
    case 0:
        if (o->spd > o->b6f << 8) {
            o->spd -= 0x100;
            return 0;
        }
        return 1;
    case 1:
        if (o->spd >= o->b6f << 8) return 1;
        o->spd += 0x100;
        return 0;
    }
    return 0;
}

static __inline__ s32 decay_2BB9C(void)
{
    s16 v = D_1F8000E6;
    if (v != 0) {
        if (v > 0) {
            v -= 2;
            D_1F8000E6 = v;
            if (v < 0) D_1F8000E6 = 0;
        } else {
            v += 2;
            D_1F8000E6 = v;
            if (v > 0) D_1F8000E6 = 0;
        }
        return 1;
    }
    return 0;
}

static __inline__ void clamp_2BB9C(S_2BB9C *o)
{
    s16 f = D_1F8000F2;
    s32 s = f + D_1F8000E6;
    if (o->w30 > s) D_1F8000E6 = o->w30 - f;
    else if (o->w32 < s) D_1F8000E6 = o->w32 - f;
}

void func_8002BB9C(S_2BB9C *o);

typedef struct { u8 b0; char p0[0x15]; s16 w16; char p1[0x16]; u16 w2e; char p2[0x79]; u8 ba9; } S_2C7D8;
typedef struct { s16 w0; s16 w2; } P_2C7D8;
typedef struct {
    char p0[0x24]; s32 d24; char p1[8]; s16 w30; s16 w32; P_2C7D8 *d34; P_2C7D8 *d38;
    u8 b3c; u8 b3d; char p2[0x1a]; u16 w58;
} O_2C7D8;
extern u8 D_800A5436;

static __inline__ void Clamp_2C7D8(O_2C7D8 *o)
{
    S_2C7D8 *s = &PLAYER;
    char pad;
    if (s->b0 < 4 || s->b0 == 7) {
        u16 x = o->d34->w2;
        s16 t;
        t = x - 0x90;
        if ((*(P_2C7D8 **)&D_800A53D8)->w2 < t) {
            (*(P_2C7D8 **)&D_800A53D8)->w2 = t;
        } else {
            t = x + 0x90;
            if (t < (*(P_2C7D8 **)&D_800A53D8)->w2)
                (*(P_2C7D8 **)&D_800A53D8)->w2 = t;
        }
        if ((*(s32 *)&GAME) == 0x2000E) {
            if (s->w16 < o->w30 - 0x54)
                s->w16 = o->w30 - 0x54;
        } else {
            if (s->w16 < o->w30 - 0x88)
                s->w16 = o->w30 - 0x88;
        }
    }
}

void func_8002C7D8(O_2C7D8 *o);

typedef struct S_2CA40 {
    char p0[0x20]; s32 vy; s32 vx; char p1[0x6e - 0x28];
    u8 b6e, b6f, b70, b71, b72, b73;
} S_2CA40;
void func_8002A9FC();
void func_8002ABC0();
void func_8002CA40(S_2CA40 *o);

void updateLookScroll(u8* self);

typedef struct {
    u8 b0, b1, b2, b3;
    char p4[0x34 - 4];
    u16 *p34;
    char p38[0x44 - 0x38];
    u16 f44;
} S_2CC20;
extern u16 D_1F80016E;

void func_8002CC20(S_2CC20 *o);

typedef struct {
    u8 b0, b1, b2, b3;
    char pad[0x34 - 4];
    u16 *p34;
    char pad2[0x44 - 0x38];
    u16 w44;
} S8002A258_2CD7C;
extern s16 D_1F8000E6_S16Arr[] asm("D_1F8000E6");

void func_8002CD7C(S8002A258_2CD7C *o);

extern u8 PLAYER_U8Arr[] asm("PLAYER");
static __inline__ void lim_2CEF8(s16 *q, s16 x, s32 k) { if (*q < x - k) *q = x - k; }
void func_8002CEF8(GameObject *o);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002CFF4);

void dispatchSectionInit(void);

void dispatchSectionUpdate(void);

void dispatchSectionDraw(void);

void dispatchSectionInput(void);

void dispatchSectionScroll(void);

void dispatchSectionSelect(void);

void dispatchSectionClose(void);

void dispatchSectionConfirm(void);

typedef struct {
    u8 b0, b1, b2, b3, b4, b5, b6, b7;
    s32 d8;
    s32 dc;
    s32 d10;
    char p14[0x3c - 0x14];
    u8 b3c, b3d, b3e, b3f;
    char p40[0x58 - 0x40];
    u16 w58;
    char p5a[0x6c - 0x5a];
    u8 b6c, b6d, b6e, b6f;
    u8 b70, b71, b72, b73;
    u8 b74, b75, b76, b77;
} S_2D784;
extern void (*D_8007C5F0[])(S_2D784 *);

void func_8002D784(S_2D784 *o);

s16 getScrollOffsetX(void);

s32 getScrollOffsetY(void);

extern s32 WMD_DATA;
extern void (*D_8007C640[])(char *, s32);

void func_8002DA2C(void);

void stubInventory1(void) {
}

void updateObjectsLayer8(void);

void stubInventory2(void);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002DBD8);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002DEC4);

void stubInventory3(void);

void spawnItemPickupObject(u8 arg0);

void cloneItemPickupObject(u8* src);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", func_8002E494);
