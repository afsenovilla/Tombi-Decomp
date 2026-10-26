// FUNC 800f2b98 2632 X010
// MATCHING 800f2b98 2632
#include "TOBJ.H"
typedef struct {
    unsigned char b0;
    unsigned char p1[3];
    unsigned char b4, b5;
    unsigned char p6[2];
    unsigned char b8;
    unsigned char p9[0x20 - 9];
    unsigned short w20;
    unsigned char p22[0x28 - 0x22];
    unsigned short w28, w2a, w2c, w2e;
} P800F2B98;
typedef struct {
    unsigned char p0[0xc3];
    unsigned char c3;
    unsigned char p4[0xc9 - 0xc4];
    unsigned char c9;
    unsigned char pa[0xcc - 0xca];
    unsigned char cc;
    unsigned char pd[0xe4 - 0xcd];
    unsigned char *e4;
} X800F2B98;
#define X(o) ((X800F2B98 *)(o))
typedef struct { void *p[90]; } T800F2B98;

extern P800F2B98 *PLAYER_EXT;
extern unsigned char *DAT_800a611c;
extern unsigned char *BITTEN_OBJECT;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned short DAT_1f8003c6, DAT_1f8001fc, DAT_1f8001f8;
extern short DAT_1f800238;
extern short DAT_8009c944, DAT_8009c946[];
extern unsigned char DAT_8009d2b0, DAT_8009d2b1[], DAT_8009c990;
extern int GRABBED_OBJECT;
extern unsigned char SLOPE_TILT_TABLE[];
extern short PLAYER_FOOT_OFFSET_BY_DIR[];
extern T800F2B98 PLAYER_ANIM_TABLE;
extern short FUN_8001fddc(int, int);
extern void playerSetJumpRiseVelocity(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001e560(int, int);
extern void FUN_8001fec0(TObj *);
extern short spawnPlayerFootstepEffect(void);
extern TObj *FUN_80018448(void);
extern void playerUpdateSpeedFromPad(TObj *);
extern void playerAirAccelerateX(TObj *);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern void playerAirSpinToFall(TObj *);
extern int FUN_8003facc(TObj *);
extern void FUN_80040278(TObj *, short, short);
extern void playerJumpArea3Float(TObj *);
extern void playerUpdateFall(TObj *);
extern void playerLandFromJump(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern int playerProbeGrab(TObj *, int);
extern void playerEnterGrabStep(TObj *, int);

void playerStepJump(TObj *o)
{
    T800F2B98 tab;
    short s;
    int d;
    unsigned char v;
    unsigned short m;
    unsigned char bb;

    switch (o->state) {
    case 0:
        d = o->wb2;
        o->timer = 10;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->b9e = 0;
        *(unsigned char *)&o->waa = 0;
        X(o)->c3 = 0;
        o->b69 = 0;
        o->b9c = 1;
        o->wb0 = 0;
        *(unsigned char *)&o->da0 = 0;
        *((unsigned char *)&o->da0 + 1) = 0;
        o->wb6 = 0;
        o->velX = FUN_8001fddc(0, d);
        o->velY = 0;
        PLAYER_EXT->b8 = 0;
        PLAYER_EXT->w2c = 4;
        PLAYER_EXT->w20 = 0;
        PLAYER_EXT->b5 = 0;
        if (*(unsigned char *)&o->wac >= 2) {
            BITTEN_OBJECT = DAT_800a611c;
            DAT_800a611c[4] = 2;
            BITTEN_OBJECT[5] = 2;
            BITTEN_OBJECT[6] = 0;
        }
        *(unsigned char *)&o->wac = 0;
        playerSetJumpRiseVelocity(o);
        tab = PLAYER_ANIM_TABLE;
        o->anim = tab.p[PLAYER_EXT->w2c];
        FUN_8001fe6c(o);
        PLAYER_EXT->w2e = PLAYER_EXT->w2c;
        FUN_8001e560(2, 4);
        o->state = 1;
    case 1:
        if (o->velY >= 0) {
            PLAYER_EXT->b8 = 1;
            o->state = 2;
        }
        if (PAD_PRESSED[0] & DAT_1f8003c6) {
            if (PLAYER_EXT->b8 != 0) goto common;
            if (++PLAYER_EXT->w20 >= 0xe) {
                PLAYER_EXT->b8 = 1;
                o->state = 2;
            }
        } else {
            PLAYER_EXT->b8 = 1;
            if (PLAYER_EXT->w20 >= 5) {
                o->state = 2;
                goto common;
            }
            PLAYER_EXT->w20++;
        }
        playerSetJumpRiseVelocity(o);
    case 2:
    common:
        o->h->raw += DAT_8009c944 << 8;
        o->y.raw += DAT_8009c946[0] << 8;
        FUN_8001fec0(o);
        if (o->ba7 != 0) {
            short a = o->a.p.whole, y = o->y.p.whole, b = o->b.p.whole;
            TObj *q;
            if (spawnPlayerFootstepEffect() == 0 && DAT_1f800238 >= 6 && (q = FUN_80018448()) != 0) {
                short *t;
                q->active = 1;
                q->type = 0x31;
                q->subtype = 1;
                q->a.p.whole = a;
                q->y.p.whole = y;
                q->b.p.whole = b;
                t = &PLAYER_FOOT_OFFSET_BY_DIR[(DAT_1f8001f8 & 7) * 2];
                q->h->p.whole += t[0];
                q->y.p.whole += t[1];
            }
        }
        playerUpdateSpeedFromPad(o);
        playerAirAccelerateX(o);
        o->h->raw += o->velX << 8;
        FUN_8010f0f4(o);
        FUN_8001fd94(o);
        if (o->velY >= -899) playerAirSpinToFall(o);
        if (o->velY > 0) {
            { int e;
            PLAYER_EXT->b8 = 1;
            e = 0x10;
            o->velY = 0;
            o->velV = 0;
            o->d84 = 0;
            if (o->animFrame & 1) e = 0xf0;
            o->timer = 10;
            o->b9c = 2;
            o->d88 = e;
            *(unsigned char *)&o->wac = 1;
            o->state = 3; }
        }
        if (FUN_8003facc(o)) {
            { int e;
            PLAYER_EXT->b8 = 1;
            e = 0x10;
            o->velY = 0;
            o->velV = 0;
            o->d84 = 0;
            if (o->animFrame & 1) e = 0xf0;
            o->timer = 10;
            o->b9c = 2;
            o->d88 = e;
            *(unsigned char *)&o->wac = 1;
            o->state = 3; }
        }
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10);
        o->b69 = 0;
        break;
    case 3:
        o->h->raw += DAT_8009c944 << 8;
        o->y.raw += DAT_8009c946[0] << 8;
        FUN_8001fec0(o);
        if (X(o)->c9 != 0) playerJumpArea3Float(o);
        else playerUpdateFall(o);
        playerUpdateSpeedFromPad(o);
        PLAYER_EXT->b8 = 1;
        o->b9e = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        FUN_8003facc(o);
        if (*(unsigned char *)&o->wac == 2) {
            BITTEN_OBJECT = X(o)->e4;
            PLAYER_EXT->b8 = 0;
            o->ba7 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            *((unsigned char *)&o->waa + 1) &= 0x7f;
        } else if (o->b69 == 1) {
            playerLandFromJump(o);
            if (*((unsigned char *)&o->waa + 1) & 0x80) {
                o->ba4 = 0;
                switch (DAT_8009d2b1[0]) {
                case 1: o->step = 0x2a; o->state = 0; break;
                case 2: o->step = 0x2b; o->state = 0; break;
                default: o->state = 0; break;
                }
            } else if (o->bbe & 8) {
                PLAYER_EXT->b8 = *(unsigned char *)&o->animFrame & 1;
                o->step = 0x1b;
                o->state = 0;
            } else {
                o->ba4 = 0;
                PLAYER_EXT->w2e = 0xffff;
                PLAYER_EXT->w28 = 0xffff;
                PLAYER_EXT->w2a = 0xffff;
                o->d8c = SLOPE_TILT_TABLE[o->wb0];
                o->step = 1;
                o->state = 0;
                if (DAT_1f8001fc & DAT_1f8003c6) {
                    DAT_8009d2b0 = 0;
                    o->step = 2;
                    o->state = 0;
                }
            }
        } else if (FUN_8003fd78(o, 4, 0) != 0) {
            playerLandFromJump(o);
            if (*((unsigned char *)&o->waa + 1) & 0x80) {
                o->ba4 = 0;
                o->ba5 = 0;
                PLAYER_EXT->w20 = 0;
                switch (DAT_8009d2b1[0]) {
                case 1: o->step = 0x2a; o->state = 0; break;
                case 2: o->step = 0x2b; o->state = 0; break;
                default: o->state = 0; break;
                }
            } else if (o->bbe != 0) {
                o->step = 1;
                o->velY = 0;
                o->state = 0;
                if (o->step == 0x10 || o->step == 0x17 || o->step == 0x1b || o->step == 0x1e) goto l33e8;
                bb = o->bbe;
                if ((bb & 2) && o->b9c == 0) {
                    if (bb & 1) {
                        if (o->wb2 < -0x144) goto l3344;
                    } else if (o->wb2 > 0x144) {
                    l3344:
                        PLAYER_EXT->b8 = *(unsigned char *)&o->animFrame & 1;
                        o->step = 0x10;
                        o->state = 0;
                    }
                } else if (bb & 4) {
                    PLAYER_EXT->b8 = *(unsigned char *)&o->animFrame & 1;
                    o->step = 0x17;
                    o->state = 0;
                } else if (bb & 0x10) {
                    if (bb & 1) {
                        if (o->wb2 < -0x144) goto l33c8;
                    } else if (o->wb2 > 0x144) {
                    l33c8:
                        PLAYER_EXT->b8 = *(unsigned char *)&o->animFrame & 1;
                        o->step = 0x1e;
                        o->state = 0;
                    }
                }
            l33e8:
                if (o->bbe & 0x20) {
                    PLAYER_EXT->b8 = *(unsigned char *)&o->animFrame & 1;
                    o->step = 0x1f;
                    o->state = 0;
                }
            } else {
                o->ba4 = 0;
                o->ba5 = 0;
                PLAYER_EXT->w20 = 0;
                PLAYER_EXT->w2e = 0xffff;
                PLAYER_EXT->w28 = 0xffff;
                PLAYER_EXT->w2a = 0xffff;
                o->step = 1;
                o->state = 0;
                if (DAT_1f8001fc & DAT_1f8003c6) {
                    DAT_8009d2b0 = 0;
                    o->step = 2;
                    o->state = 0;
                }
            }
        } else {
            if (DAT_8009c990 == 3 && (DAT_1f8001fc & DAT_1f8003c6)) {
                DAT_8009d2b0 = 0;
                if (X(o)->cc == 1) {
                    *(unsigned char *)&o->wac = 0;
                    PLAYER_EXT->b4 = 0;
                    s = o->wb2;
                    if (s < 0) {
                        short t = s;
                        if (!(o->animFrame & 1)) t = -s;
                        o->wb2 = t;
                    } else {
                        if (o->animFrame & 1) s = -s;
                        o->wb2 = s;
                    }
                    o->step = 0x45;
                    o->state = 0;
                    return;
                }
            }
        }
    tail:
        if (o->velY > 0) o->b9c = 2;
        break;
    }
    if (*(unsigned char *)&o->wac < 2 && (GRABBED_OBJECT = playerProbeGrab(o, 0)) != 0) {
        PLAYER_EXT->b8 = 0;
        PLAYER_EXT->w20 = 0;
        *(unsigned char *)&o->wac = 0;
        o->ba7 = 0;
        o->b9c = 0;
        o->wb2 = 0;
        playerEnterGrabStep(o, GRABBED_OBJECT == 1);
    }
    spawnPlayerFootstepEffect();
}
