// VERSION SLPS_01144 x00 80101F30 1620
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
#include "gvars.h"
#include "gcd.h"
#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
#define G PLAYER_EXT
extern unsigned char *PLAYER_EXT;
extern TObj *BITTEN_OBJECT;
extern unsigned char D_801145D4[];
extern unsigned char GAME_BITTEN_JUMP_KIND;
extern int D_800A0C7C;
extern void AnimAdvance(TObj *);
extern void func_8010EC44(TObj *);
extern void func_800EE99C_slps01144(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void func_8001E458(int, int);
extern void func_8001C988(int, int);
extern short probeSidesAndApplyTileResponse(TObj *, int, int);
extern int func_80056C34_slps01144(TObj *, int);
extern void func_800EFDF8(TObj *, int);

static __inline__ void dampVelX(TObj *o)
{
    short v = o->velX;
    if (v > 0) {
        o->velX = v - 208;
        if (o->velX < 0) o->velX = 0;
    } else {
        o->velX = v + 208;
        if (o->velX > 0) o->velX = 0;
    }
}

static __inline__ void landSfx(TObj *o)
{
    if (GCD.bcf06 && o->subtype != 1) {
        o->subtype = 1;
        if (GCD.bce61 != 0xff) func_8001C988(189, 1);
    }
}

static __inline__ void bittenKind(TObj *o)
{
    switch (GAME_BITTEN_JUMP_KIND) {
    case 1:
        o->step = 42;
        o->state = 0;
        break;
    case 2:
        o->step = 43;
        o->state = 0;
        break;
    default:
        o->state = 0;
        break;
    }
}

void func_80101F30_slps01144(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
    case 1:
        o->h->raw += GVARS.driftH << 8;
        o->y.raw += GVARS.driftY << 8;
        AnimAdvance(o);
        G[8] = 1;
        o->b9e = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        func_8010EC44(o);
        func_800EE99C_slps01144(o);
        PlayerSetAnimIfChanged(o, 4);
        if (B(o, 0xac) == 2) {
            BITTEN_OBJECT = *(TObj **)((char *)o + 0xd4);
            G[8] = 0;
            o->ba7 = 0;
            o->ba5 = 0;
            o->step = 14;
            o->state = 0;
            B(o, 0xab) &= 0x7f;
        } else if (o->b69 == 1) {
            o->ba7 = 0;
            G[8] = 0;
            func_8001E458(28, 127);
            o->b9c = 0;
            B(o, 0xac) = 0;
            dampVelX(o);
            {
                unsigned short t = o->velX;
                o->velX = 0;
                o->velY = 0;
                o->wb2 = t;
            }
            o->d8c = D_801145D4[o->wb0];
            o->ba5 = 0;
            *(short *)(G + 0x20) = 0;
            landSfx(o);
            if (B(o, 0xab) & 0x80) {
                o->ba4 = 0;
                bittenKind(o);
            } else if (o->bbe & 8) {
                G[8] = o->animFrame & 1;
                o->step = 27;
                o->state = 0;
            } else {
                o->ba4 = 0;
                *(unsigned short *)(G + 0x2e) = 0xffff;
                *(unsigned short *)(G + 0x28) = 0xffff;
                *(unsigned short *)(G + 0x2a) = 0xffff;
                {
                    unsigned char t = D_801145D4[o->wb0];
                    int p;
                    o->step = 1;
                    o->state = 0;
                    p = SPAD.s1fc & SPAD.s3c6;
                    o->d8c = t;
                    if (p) {
                        o->step = 2;
                        o->state = 0;
                    }
                }
            }
        } else if (probeSidesAndApplyTileResponse(o, 4, 0)) {
            func_8001E458(28, 127);
            o->d8c = D_801145D4[o->wb0];
            o->ba7 = 0;
            o->b9c = 0;
            B(o, 0xac) = 0;
            G[8] = 0;
            dampVelX(o);
            o->wb2 = (unsigned short)o->velX;
            landSfx(o);
            if (B(o, 0xab) & 0x80) {
                o->ba4 = 0;
                o->ba5 = 0;
                *(short *)(G + 0x20) = 0;
                bittenKind(o);
            } else if (o->bbe != 0) {
                o->step = 1;
                o->velY = 0;
                o->state = 0;
                if (o->step != 16 && o->step != 23 && o->step != 27 && o->step != 30) {
                    unsigned char f = o->bbe;
                    if ((f & 2) && o->b9c == 0) {
                        if (f & 1) {
                            if (o->wb2 >= -324) goto next;
                        } else {
                            if (o->wb2 < 325) goto next;
                        }
                        G[8] = o->animFrame & 1;
                        o->step = 16;
                        o->state = 0;
                    } else if (f & 4) {
                        G[8] = o->animFrame & 1;
                        o->step = 23;
                        o->state = 0;
                    } else if (f & 0x10) {
                        if (f & 1) {
                            if (o->wb2 >= -324) goto next;
                        } else {
                            if (o->wb2 < 325) goto next;
                        }
                        G[8] = o->animFrame & 1;
                        o->step = 30;
                        o->state = 0;
                    }
                }
            next:
                if (o->bbe & 0x20) {
                    G[8] = o->animFrame & 1;
                    o->step = 31;
                    o->state = 0;
                }
            } else {
                o->ba4 = 0;
                o->ba5 = 0;
                *(unsigned short *)(G + 0x2e) = 0xffff;
                *(unsigned short *)(G + 0x28) = 0xffff;
                *(unsigned short *)(G + 0x2a) = 0xffff;
                *(short *)(G + 0x20) = 0;
                o->step = 1;
                o->state = 0;
                if (SPAD.s1fc & SPAD.s3c6) {
                    o->step = 2;
                    o->state = 0;
                }
            }
        }
        if (o->velY > 0) o->b9c = 2;
        break;
    }
    if (B(o, 0xac) < 2) {
        D_800A0C7C = func_80056C34_slps01144(o, 0);
        if (D_800A0C7C) {
            G[8] = 0;
            *(short *)(G + 0x20) = 0;
            B(o, 0xac) = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->wb2 = 0;
            func_800EFDF8(o, D_800A0C7C == 1);
        }
    }
}
#endif
