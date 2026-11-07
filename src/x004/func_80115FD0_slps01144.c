// VERSION SLPS_01144 x04 80115FD0 6472
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "gamestate.h"
typedef struct { unsigned short frac; short whole; } FP;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[7];
    signed char b0f;
    int x;
    short yf, yw;
    int z;
    char p1c[0x2e - 0x1c];
    unsigned short animFrame;
    char p30[0x40 - 0x30];
    FP *h;
    char p44[0x69 - 0x44];
    unsigned char b69;
    char p6a[0x9c - 0x6a];
    unsigned char b9c, b9d, b9e;
    char p9f[0xa2 - 0x9f];
    unsigned char ba2;
    char pa3[0xaa - 0xa3];
    unsigned char baa, bab, bac;
    char pad[0xe0 - 0xad];
    short we0;
} PL;
typedef struct { char a[4]; unsigned char b4; char p[3]; } E8;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x3f - 0x34];
    unsigned char b3f;
    char p40[0x5a - 0x40];
    short w5a;
    char p5c[0x68 - 0x5c];
    E8 *p68;
    unsigned char b6c, b6d, b6e, b6f;
} O;
typedef void (*Fn)(O *);

extern PL D_800A7B78;
#define PLAYER D_800A7B78
extern Fn D_8007582C[];
extern E8 *D_8012ED7C[20];
extern signed char D_8009EE58[];
#define D_8009D2B0 D_8009EE58[0]
extern unsigned char D_8009C982b;
extern short D_1F80016Ax[], D_1F80016Ex[];
#define PLAYER_H_WHOLE D_1F80016Ax[0]
#define D_1F80016E D_1F80016Ex[0]
extern short D_1F8000F2x[];
#define VIEW_CAMERA_Y D_1F8000F2x[0]
extern unsigned int stepScrollReturnY(O *);
extern unsigned int stepScrollClearX(O *);
extern void func_80028A84_slps01144(O *);
extern void syncObjectTargetY(O *);
extern void func_80029E88(O *);
extern int stepPlayerXToward(int);
extern int func_80027EA8_slps01144(O *, int, int);
extern void markSectionVisited(void);
extern void loadSectionBounds(O *);
extern void AnimAdvance(PL *);
extern void playerSetAnim(PL *, int, int);
extern void playerSetAnimIfChangedAt(PL *, int, int);
extern void func_80115EB4_slps01144(O *);
extern void func_80115D9C_slps01144(O *);

#define XU (*(unsigned short *)&D_1F80016Ax[0])
#define ZU (*(unsigned short *)&D_1F80016Ex[0])

#define T1(n) \
    if (func_80027EA8_slps01144(o, 1, n)) { \
        D_8009D2B0 = 0; \
        GVARS.b3f = 1; \
        GVARS.b42 = 1; \
    } \
    break;

#define T2A(n, k) \
    if (!func_80027EA8_slps01144(o, 2, n)) break; \
    o->w5a = k; \
    o->step = 4; \
    D_8009D2B0 = 0; \
    GVARS.b3f = 1; \
    GVARS.b42 = 1; \
    break;

#define T2B(n, k) \
    if (!func_80027EA8_slps01144(o, 2, n)) break; \
    GVARS.b3f = 1; \
    GVARS.b42 = 1; \
    D_8009D2B0 = 0; \
    o->w5a = k; \
    o->step = 4; \
    break;

#define F6047() \
    if (D_1F80016E < -0x3f2) \
        PLAYER.b0f = 0; \
    else if (D_1F80016E < -0x3b5) \
        PLAYER.b0f = -8;

static __inline__ short blocked(void)
{
    if (PLAYER.bac >= 2) return 1;
    if (PLAYER.b9c | PLAYER.b9e | PLAYER.baa | GVARS.useItemPending) return 1;
    if (SPAD.s1fc & 0x1000) return 0;
    return 1;
}

void func_80115FD0_slps01144(O *o)
{
    PL *p = &PLAYER;

    switch (o->step) {
    case 0:
        if (((unsigned char*)&GAME.disableSelectMenu)[0] != 0) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) ((unsigned char*)&GAME.disableSelectMenu)[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    case 1:
        if (GVARS.playerLanded != 0) o->step++;
        func_80028A84_slps01144(o);
        syncObjectTargetY(o);
        func_80029E88(o);
        break;
    case 2:
        D_8007582C[o->subtype](o);
        switch (GAME.selectedSection) {
        case 0:
            F6047();
            if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x3de && (unsigned short)(ZU + 0x428) < 0x10 && (unsigned)(XU - 0x37b) < 0x12) {
                T1(0xb);
            }
            if (p->ba2 == 2) {
                if (D_1F80016E >= -0x185) {
                    if ((unsigned)(XU - 0x24a) < 0x60) {
                        stepPlayerXToward(0x27a);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0x358) {
                            T1(1);
                        }
                    }
                    if ((unsigned)(XU - 0x356) < 0x60) {
                        stepPlayerXToward(0x386);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x196) break;
                        T1(2);
                    }
                    if ((unsigned)(XU - 0x3b0) < 0x60) {
                        stepPlayerXToward(0x3e0);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x196) break;
                        T1(3);
                    }
                    break;
                }
                if (D_1F80016E >= -0x1df) {
                    if ((unsigned)(XU - 0x1ee) < 0x60) {
                        stepPlayerXToward(0x21e);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x358) break;
                        T1(4);
                    }
                    if ((unsigned)(XU - 0x356) < 0x60) {
                        stepPlayerXToward(0x386);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x1f0) break;
                        T1(5);
                    }
                    break;
                }
                if (D_1F80016E >= -0x2ed) {
                    if ((unsigned)(XU - 0x356) < 0x60) {
                        stepPlayerXToward(0x386);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x358) break;
                        T1(6);
                    }
                    if ((unsigned)(XU - 0x3b0) < 0x60) {
                        stepPlayerXToward(0x3e0);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x3b2) break;
                        T1(7);
                    }
                }
                break;
            }
            if (blocked()) break;
            if ((unsigned)(XU - 0x192) < 0x60) {
                if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x384) {
                    T1(9);
                }
                if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x32a) {
                    T1(10);
                }
            }
            if ((unsigned)(XU - 0x2fa) < 0x60 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x168) {
                T2A(8, 0x32a);
            }
            if ((unsigned)(XU - 0x354) < 0x60 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x21c) {
                T2A(8, 0x384);
            }
            if ((unsigned)(XU - 0x4bc) < 0x60) {
                if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x32a) {
                    T2A(8, 0x4ec);
                }
                if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x384) {
                    T2B(8, 0x4ec);
                }
            }
            if ((unsigned)(XU - 0x516) < 0x60 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x32a) {
                T2B(8, 0x546);
            }
            break;
        case 1:
            F6047();
            if (!(GAME.purifiedAreas & 8) && GCD.bcdd5 != 0xff) {
                func_80115EB4_slps01144(o);
                break;
            }
            if (PLAYER_H_WHOLE >= 0x653 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x4ec && D_1F80016E >= -0x225
                && func_80027EA8_slps01144(o, 1, 5)) {
                D_8009D2B0 = 0;
                GVARS.b3f = 1;
                GVARS.b42 = 1;
                GCD.bcfee = 1;
            }
            if (p->ba2 == 2) {
                if (D_1F80016E < -0x12b) break;
                if ((unsigned)(XU - 0x2a1) >= 0x60) break;
                stepPlayerXToward(0x2d1);
                if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0x519) break;
                T1(1);
            }
            if (PLAYER_H_WHOLE < 0x78 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x546) {
                T1(0);
            }
            if (blocked()) break;
            switch ((*(short*)((char*)&SPAD.i170 + 2))) {
            case 0x32a:
                if ((unsigned)(XU - 0x1ec) < 0x60) {
                    T2B(3, 0x21c);
                }
                break;
            case 0x384:
                if ((unsigned)(XU - 0x138) < 0x60) {
                    T2B(3, 0x168);
                }
                if ((unsigned)(XU - 0x1ec) < 0x60) {
                    T2B(3, 0x21c);
                }
                if ((unsigned)(XU - 0x2fa) < 0x60) {
                    T2B(3, 0x32a);
                }
                break;
            case 0x492:
                if ((unsigned)(XU - 0x2fa) < 0x60) {
                    T2B(3, 0x32a);
                }
                if ((unsigned)(XU - 0x354) < 0x60) {
                    T2B(3, 0x384);
                }
                break;
            case 0x4ec:
                if ((unsigned)(XU - 0x2fa) < 0x60 && D_1F80016E < -0x1ae) {
                    T2B(3, 0x32a);
                }
                if ((unsigned)(XU - 0x408) < 0x60 && D_1F80016E < -0x1f4) {
                    T1(2);
                }
                if ((unsigned)(XU - 0x4bc) < 0x60) {
                    T1(4);
                }
                break;
            }
            break;
        case 2:
            F6047();
            if (!(GAME.purifiedAreas & 8) && GCD.bcdd5 != 0xff) {
                func_80115D9C_slps01144(o);
                break;
            }
            if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x276 && (unsigned short)(ZU + 0x413) < 0xc && (unsigned)(XU - 0x3d3) < 0x12) {
                T1(0xa);
            }
            if (p->ba2 == 2) {
                if (D_1F80016E >= -0x185) {
                    if ((unsigned)(XU - 0x465) < 0x60) {
                        stepPlayerXToward(0x495);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x196) break;
                        T1(1);
                    }
                    if ((unsigned)(XU - 0x352) < 0x60) {
                        stepPlayerXToward(0x382);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x196) break;
                        T1(2);
                    }
                    break;
                }
                if (D_1F80016E >= -0x1df) {
                    if ((unsigned)(XU - 0x3b0) < 0x60) {
                        stepPlayerXToward(0x3e0);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x1f0) break;
                        T1(3);
                    }
                    break;
                }
                if (D_1F80016E >= -0x239) {
                    if (p->step != 0x20) break;
                    if ((unsigned)(XU - 0x438) < 0x60) {
                        stepPlayerXToward(0x468);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x1f0) break;
                        T1(4);
                    }
                    if ((unsigned)(XU - 0x357) < 0x60) {
                        stepPlayerXToward(0x387);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x24a) break;
                        T1(5);
                    }
                    break;
                }
                if (D_1F80016E >= -0x293) {
                    if ((unsigned)(XU - 0x2fb) < 0x60) {
                        stepPlayerXToward(0x32b);
                        if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x24a) break;
                        T1(6);
                    }
                }
                break;
            }
            if (PLAYER_H_WHOLE >= 0x6d0 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x168) {
                T1(7);
            }
            if (blocked()) break;
            if (PLAYER_H_WHOLE < 0x28a && (*(short*)((char*)&SPAD.i170 + 2)) == 0x21c) {
                T1(8);
            }
            if ((unsigned)(XU - 0x570) < 0x60) {
                if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x168) {
                    T2B(9, 0x5a0);
                }
                if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x1c2) {
                    T2B(9, 0x5a0);
                }
                if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x21c) {
                    T2B(9, 0x5a0);
                }
                break;
            }
            if ((unsigned)(XU - 0x516) < 0x60 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x21c) {
                T2B(9, 0x546);
            }
            if ((unsigned)(XU - 0x246) < 0x60 && D_1F80016E >= -0x180 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x168) {
                T1(0xb);
            }
            break;
        case 3:
            F6047();
            if (p->ba2 == 2) {
                if (D_1F80016E >= -0x185) {
                    if ((unsigned)(XU - 0x1ee) >= 0x60) break;
                    stepPlayerXToward(0x21e);
                    if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0x573) break;
                    T1(1);
                }
                if (p->yw >= -0x280) break;
                if (D_1F80016E < -0x293) break;
                if ((unsigned)(XU - 0x248) >= 0x60) break;
                stepPlayerXToward(0x278);
                if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0x4bf) break;
                T1(2);
            }
            if (blocked()) break;
            switch ((*(short*)((char*)&SPAD.i170 + 2))) {
            case 0x546:
                if ((unsigned)(XU - 0x1ec) < 0x60) {
                    T2B(5, 0x21c);
                }
                if ((unsigned)(XU - 0x2fa) < 0x60) {
                    T1(4);
                }
                break;
            case 0x5a0:
                if ((unsigned)(XU - 0x138) < 0x60) {
                    T2B(5, 0x168);
                }
                if ((unsigned)(XU - 0x2fa) < 0x60) {
                    T1(3);
                }
                break;
            }
            break;
        case 6:
            if (PLAYER_H_WHOLE < 0x14 && D_1F80016E >= -0x7b) {
                T1(0);
            }
            break;
        case 7:
            if (PLAYER_H_WHOLE < 0x14 && D_1F80016E >= -0x59) {
                T1(0);
            }
            break;
        case 8:
            if (PLAYER_H_WHOLE < 0x14 && D_1F80016E >= -0x163) {
                T1(0);
            }
            break;
        case 9:
            if (PLAYER_H_WHOLE < 0x14 && D_1F80016E >= -0x6b) {
                T1(0);
            }
            break;
        case 11: {
            signed char *q;
            if (D_1F80016E < -0x1f4) {
                o->w30 = o->w32 = -0x3ac;
                if (PLAYER_H_WHOLE < 0x14 && D_1F80016E >= -0x36f) {
                    T1(0);
                }
                if (PLAYER_H_WHOLE < 0x11f) break;
                if (D_1F80016E < -0x36f) break;
                q = &D_8009D2B0;
                if (*q == 3) break;
                if (GCD.bcdf1 != 0xff) break;
                o->step = 6;
                goto tail11;
            }
            o->w30 = o->w32 = -100;
            if (PLAYER_H_WHOLE < 0x14 && D_1F80016E >= -0x27) {
                T1(1);
            }
            if (PLAYER_H_WHOLE < 0x11f) break;
            if (D_1F80016E < -0x27) break;
            q = &D_8009D2B0;
            if (*q == 3) break;
            o->step = 10;
        tail11:
            *q = 0;
            PLAYER.active = 3;
            PLAYER.b04 = 5;
            PLAYER.step = 0;
            PLAYER.state = 0;
            break;
        }
        case 13:
            if (PLAYER_H_WHOLE < 0x14 && D_1F80016E >= -0x16d) {
                T1(0);
            }
            if (PLAYER_H_WHOLE < 0x26d) break;
            if (D_1F80016E < -0x3d) break;
            T1(1);
        case 10:
        case 19:
            if ((*(short*)((char*)&SPAD.i170 + 2)) < -0x3c) {
                T1(0);
            }
            break;
        case 14:
            if (D_1F80016E < -0xfa) {
                T1(1);
            }
            break;
        case 4:
        case 5:
        case 12:
        case 15:
        case 16:
        case 17:
        case 18:
            if (PLAYER_H_WHOLE < 0x14 && D_1F80016E >= -0x45) {
                T1(0);
            }
            break;
        }
        break;
    case 3:
        GVARS.b42 = 1;
        D_8007582C[o->subtype](o);
        break;
    case 4:
        if (p->active == 2 || p->bac >= 2) {
            o->subtype = 0;
            o->step = 2;
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            ((unsigned char*)&GAME.disableSelectMenu)[0] = 0;
            break;
        }
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        {
        E8 *e = D_8012ED7C[GAME.selectedSection] + GAME.selectedSpawnPoint;
        unsigned char t;
        o->p68 = e;
        t = e->b4;
        o->b3f = t;
        if (t == 0)
            p->animFrame = 0;
        else
            p->animFrame = 1;
        p->b04 = 5;
        p->step = 100;
        p->state = 0;
        }
        playerSetAnimIfChangedAt(p, 2, 0);
        if (stepPlayerXToward(o->w5a)) o->step++;
        break;
    case 5:
        D_8007582C[o->subtype](o);
        if (o->subtype != 0) break;
        o->step = 2;
        markSectionVisited();
        GVARS.b3f = 0;
        GVARS.b3e = 0;
        p->b04 = 1;
        p->step = 0;
        p->state = 0;
        loadSectionBounds(o);
        o->b6c = 0;
        o->b6d = 1;
        o->b6e = 2;
        o->b6f = 10;
        break;
    case 8:
        VIEW_CAMERA_Y += 2;
        if (VIEW_CAMERA_Y < -0x63) break;
        VIEW_CAMERA_Y = -100;
        o->w32 = -100;
        o->w30 = -100;
        o->step++;
        PLAYER.yw = -0x22;
        playerSetAnim(&PLAYER, 1, 0);
        PLAYER.animFrame = 1;
        break;
    case 9:
        PLAYER.h->whole -= 2;
        if (PLAYER.h->whole < 0x114) {
            PLAYER.active = 1;
            PLAYER.b04 = 1;
            PLAYER.step = 0;
            PLAYER.state = 0;
            o->step = 2;
        } else {
            AnimAdvance(&PLAYER);
        }
        break;
    case 6:
    case 10:
        if (PLAYER.state != 2) break;
        o->step++;
        playerSetAnim(&PLAYER, 1, 0);
        PLAYER.b04 = 5;
        PLAYER.step = 0x65;
        PLAYER.state = 0;
        PLAYER.animFrame = 0;
        break;
    case 7:
    case 11:
        AnimAdvance(&PLAYER);
        PLAYER.h->whole += 1;
        if (PLAYER.h->whole < 0x1b9) break;
        o->step++;
        break;
    case 12:
        VIEW_CAMERA_Y -= 2;
        if (VIEW_CAMERA_Y >= -0x3ac) break;
        VIEW_CAMERA_Y = -0x3ac;
        o->w32 = -0x3ac;
        o->w30 = -0x3ac;
        o->step++;
        PLAYER.yw = -0x36a;
        playerSetAnim(&PLAYER, 1, 0);
        PLAYER.animFrame = 1;
        break;
    case 13:
        PLAYER.h->whole -= 2;
        if (PLAYER.h->whole < 0x114) {
            PLAYER.active = 1;
            PLAYER.b04 = 1;
            PLAYER.step = 0;
            PLAYER.state = 0;
            o->step = 2;
        } else {
            AnimAdvance(&PLAYER);
        }
        break;
    }
}
#endif
