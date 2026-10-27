// FUNC 800f1d44 2280 X009
// MATCHING 800f1d44 2280
#include "gv2.h"
#include "plx.h"
#include "gv6.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
typedef struct { short x, y; } XY;
typedef struct {
    TObj o;
    char pc0[3];
    unsigned char c3;
    char pc4[0xcd - 0xc4];
    unsigned char cd, ce;
    char pcf[0xd5 - 0xcf];
    unsigned char d5, d6, d7;
} X;
#define XO(o) ((X *)(o))

extern TObj *PLAYER_EXT;
extern unsigned char *BITTEN_OBJECT;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned short TICK_COUNT, D_1f8003c4;
extern short FREE_OBJECT_COUNT;
extern XY PLAYER_FOOT_OFFSET_BY_DIR[];
extern char D_80010DE0[], D_80010E2C[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void playerUpdateSpeedFromPad(TObj *);
extern void playerSelectGroundAnim(TObj *);
extern void setObjectSpeedPolar2(TObj *, int, int);
extern void FUN_8001fce4(TObj *);
extern void FUN_8001e4f0(int);
extern int FUN_8005bdec(void);
extern short spawnPlayerFootstepEffect(void);
extern TObj *FUN_80018448(void);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = PLAYER_EXT;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
    }
}

static __inline__ void dust(TObj *o)
{
    short x, y, z;
    TObj *q;
    XY *t;
    x = o->a.p.whole;
    y = o->y.p.whole;
    z = o->b.p.whole;
    if (spawnPlayerFootstepEffect() == 0 && FREE_OBJECT_COUNT >= 6 && (q = FUN_80018448()) != 0) {
        q->active = 1;
        q->type = 0x31;
        q->subtype = 1;
        t = &PLAYER_FOOT_OFFSET_BY_DIR[TICK_COUNT & 7];
        q->a.p.whole = x;
        q->y.p.whole = y;
        q->b.p.whole = z;
        q->h->p.whole += t->x;
        q->y.p.whole += t->y;
    }
}

static __inline__ short odd(void)
{
    if (TICK_COUNT & 0xf) return 1;
    return 0;
}

void playerStepWalk(TObj *o)
{
    unsigned char mode;
    short lim;
    short w;
    unsigned short t;

    if ((*(unsigned char*)&o->wac) >= 2) {
        BITTEN_OBJECT = (*(unsigned char **)&PLX.p611c);
        BITTEN_OBJECT[4] = 2;
        BITTEN_OBJECT[5] = 2;
        BITTEN_OBJECT[6] = 0;
    }
    (*(unsigned char*)&o->wac) = 0;
    if (PAD_PRESSED[0] & 0xa0) {
        switch (o->state) {
        case 0:
            XO(o)->cd = 0;
            XO(o)->ce = 0;
            (*(unsigned char*)&PLAYER_EXT->w08) = 0;
            XO(o)->c3 = 0;
            o->velX = 0;
            o->state++;
        case 1:
            if (GV2.bb3 >= 3) o->animTimer = 1;
            FUN_8001fec0(o);
            w = o->wb0;
            t = w;
            if (w < 0) {
                t = (t << 2) + 0x100;
                o->wb6 = t & 0xff;
            } else {
                t <<= 2;
                if (w > 0) o->wb6 = t & 0xff;
                else o->wb6 = 0;
            }
            playerUpdateSpeedFromPad(o);
            playerSelectGroundAnim(o);
            setObjectSpeedPolar2(o, o->wb6, o->wb2);
            o->h->raw += ((short*)&GVARS.s44)[0] << 8;
            o->y.raw += ((short*)&GVARS.s44)[1] << 8;
            FUN_8001fce4(o);
            if ((*(unsigned char*)((char*)&o->wac + 1)) == 0) {
                if (o->subtype != 0) {
                    if ((unsigned short)(o->wb0 + 5) >= 11) o->y.raw += 0x50000;
                    else o->y.raw += 0x30000;
                } else {
                    if ((unsigned short)(o->wb0 + 5) >= 11) o->y.raw += 0xc0000;
                    else o->y.raw += 0x80000;
                }
            }
            switch (o->ba6) {
            case 2:
                if (PAD_PRESSED[0] & 0x20) {
                    if (o->wb2 >= 0x200) goto inc;
                    goto clr;
                }
                break;
            case 3:
                if (PAD_PRESSED[0] & 0x80) {
                    if (o->wb2 < -0x1ff) {
                    inc:
                        (*(unsigned char*)&PLAYER_EXT->w08)++;
                        if ((*(unsigned char*)&PLAYER_EXT->w08) > 20) {
                            XO(o)->cd = 0;
                            XO(o)->ce = 0;
                            (*(unsigned char*)&PLAYER_EXT->w08) = 0;
                            o->step = 0x1c;
                            o->state = 0;
                        }
                    } else {
                    clr:
                        (*(unsigned char*)&PLAYER_EXT->w08) = 0;
                    }
                }
                break;
            case 4:
            case 5:
                o->wb2 = 0;
                break;
            default:
                if ((TICK_COUNT & 7) == 0) FUN_8001e4f0(odd());
                break;
            }
            if (GV6.s10 == 7) {
                int f = o->animFrame & 3;
                if ((f == 0 && (XO(o)->d5 == 3 || XO(o)->d6 == 3 || XO(o)->d7 == 3 || o->bbf == 3)) ||
                    (f == 1 && (XO(o)->d5 == 2 || XO(o)->d6 == 2 || XO(o)->d7 == 2 || o->bbf == 2))) {
                    XO(o)->cd = 0;
                    XO(o)->ce = 0;
                    if ((o->ba6 & 6) == 0) {
                        o->wb2 = o->wbc;
                        setanim(o, 0x1a);
                        o->step = 0x13;
                        o->state = 0;
                    } else {
                        o->wb2 = 0;
                    }
                }
            } else if ((o->animFrame & 1) ? o->wb2 > 0x144 : o->wb2 < -0x144) {
                XO(o)->cd = 0;
                XO(o)->ce = 0;
                if (o->ba6 & 6) {
                    o->wb2 = 0;
                } else {
                    setanim(o, 0x1a);
                    o->step = 0x13;
                    o->state = 0;
                }
            }
            break;
        }
        mode = 0;
        if (GV2.bb1 == 1) mode = 1;
        else if (GV2.bb1 == 2) mode = 2;
        lim = 0x200;
        if ((*(unsigned int*)&GAME.area00_eventControl) & 0x40) {
            lim = 0x360;
            if (!(PAD_PRESSED[0] & D_1f8003c4) && (unsigned)(o->subtype - 1) >= 2) lim = 0x200;
        }
        if (mode != 0) {
            if ((unsigned short)(o->wb2 + lim) >= lim * 2) {
                o->wb2 = (o->wb2 < 0) ? -lim : lim;
                if (XO(o)->cd == 0) {
                    XO(o)->cd = 1;
                    XO(o)->ce = (FUN_8005bdec() % 2) * 45;
                    if (XO(o)->ce == 0) XO(o)->ce = 12;
                } else {
                    XO(o)->ce--;
                }
                if (XO(o)->ce == 0) {
                    unsigned char st;
                    switch (mode) {
                    case 1: st = 42; break;
                    case 2: st = 43; break;
                    default: goto done;
                    }
                    XO(o)->cd = 0;
                    XO(o)->ce = 0;
                    o->step = st;
                    o->state = 0;
                }
            } else {
                XO(o)->cd = 0;
                XO(o)->ce = 0;
            }
        }
    done:
        if (((*(unsigned int*)&GAME.area00_eventControl) & 0x40) && (PAD_PRESSED[0] & D_1f8003c4)) {
            dust(o);
        }
        if (o->subtype != 0) {
            dust(o);
        }
    } else {
        playerSelectGroundAnim(o);
        if (o->animFrame & 8) {
            if ((o->animFrame & 2) && ((*(unsigned char*)&o->da0) & 2)) {
                PLAYER_EXT->animFrame = 0xffff;
                o->anim = D_80010DE0;
                (*(unsigned char*)((char*)&o->da0 + 2)) = 1;
            } else if ((o->animFrame & 4) && ((*(unsigned char*)&o->da0) & 0x10)) {
                PLAYER_EXT->animFrame = 0xffff;
                o->anim = D_80010E2C;
                (*(unsigned char*)((char*)&o->da0 + 3)) = 1;
            }
        }
        XO(o)->cd = 0;
        XO(o)->ce = 0;
        o->step = 0;
        o->state = 0;
    }
    spawnPlayerFootstepEffect();
}
