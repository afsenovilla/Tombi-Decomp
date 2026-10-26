// FUNC 800f98a8 1908 X013
// MATCHING 800f98a8 1908
#include "gv6.h"
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
extern TObj *PLAYER_EXT;
extern unsigned char *BITTEN_OBJECT;
extern char D_80011108[];
extern unsigned char D_80115220[][2];
extern unsigned short D_1f8003c6;
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001e5f4(int, int);
extern void probeCollisionAtDepthC(TObj *, int, int);
extern void playerFlipPlaceAndAccelerate(TObj *, int, int);
extern void playerPerchFlipLandCheck(TObj *);
extern void playerCheckPerchLanding(TObj *);

typedef struct { unsigned char b0; char p1; short w2; char p4[8]; short wc; short we; char p10[0x28 - 0x10]; unsigned short w28, w2a; } PL;

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

static __inline__ void rot(TObj *o)
{
    PL *p = (PL *)PLAYER_EXT;
    o->wb6 += p->we;
    if ((unsigned short)o->wb6 < 0x800) U8(p, 8) = 1;
    if ((unsigned)((unsigned short)o->wb6 - 0x800) < 0x800) (*(unsigned char*)&PLAYER_EXT->w08) = 0;
    if ((unsigned short)(o->wb6 + 0x7ff) < 0x800) (*(unsigned char*)&PLAYER_EXT->w08) = 0;
    if ((unsigned short)(o->wb6 + 0xfff) < 0x800) (*(unsigned char*)&PLAYER_EXT->w08) = 1;
}

#define STEP(o) \
    o->h->p.whole += (o->animFrame & 1) ? -4 : 4; \
    probeCollisionAtDepthC(o, (short)(o->h->p.whole + ((o->animFrame & 1) ? -8 : 8)), (short)(o->y.p.whole - 0x10));

void playerStepFlipOntoPerch(TObj *o)
{
    PL *p;
    PL *q;
    short u; /* never assigned: the game passes uninitialized $s1 to playerFlipPlaceAndAccelerate in state 1 */
    int t;

    switch (o->state) {
    case 0:
        q = (PL *)PLAYER_EXT;
        o->wb6 = -0x420;
        q->w2 = 0x10;
        q->we = 0;
        o->wb2 = 2;
        U8(q, 8) = 0;
        (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
        ((PL *)PLAYER_EXT)->wc = 0;
        ((PL *)PLAYER_EXT)->w28 = 0xffff;
        ((PL *)PLAYER_EXT)->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        if ((*(unsigned char*)&o->wac) >= 2) {
            BITTEN_OBJECT = (*(unsigned char **)&PLX.p611c);
            BITTEN_OBJECT[4] = 2;
            BITTEN_OBJECT[5] = 2;
            BITTEN_OBJECT[6] = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        GVARS.i34 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        PLAYER_EXT->active = 0;
        o->anim = D_80011108;
        FUN_8001fe94(o, 0);
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole - 4;
        FUN_8001e5f4(4, 0x7f);
        o->state++;
    case 1:
        rot(o);
        if (o->wb6 == 0) {
            p = (PL *)PLAYER_EXT;
            if (p->we > 0) {
                unsigned char *b = D_80115220[--o->wb2];
                p->we = b[0];
                p->w2 = b[1];
            }
        }
        t = (short)o->wb6 >> 4;
        PLAYER_EXT->animFrame = 0xffff;
        PLAYER_EXT->visible = 0x12;
        playerFlipPlaceAndAccelerate(o, u, t);
        if (o->wb2 < 2) o->state++;
        break;
    case 2:
        rot(o);
        if (o->wb6 == 0) {
            p = (PL *)PLAYER_EXT;
            if (p->we > 0) {
                unsigned char *b = D_80115220[--o->wb2];
                p->we = b[0];
                p->w2 = b[1];
            } else {
                U8(p, 9) = 1;
            }
        }
        setanim(o, 7);
        FUN_8001fec0(o);
        {
            unsigned int v = (short)o->wb6 >> 4;
            playerFlipPlaceAndAccelerate(o, (v >> 2) & 0x3f, v);
        }
        if (o->wb2 == 0) {
            o->d8c = 0;
            o->h->p.whole = o->w74 + ((o->animFrame & 1) ? -4 : 4);
            o->y.p.whole = o->w76 + 0xc;
            o->state++;
        } else if ((*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) != 0) {
            playerFlipPlaceAndAccelerate(o, 0, 0);
            if (((volatile unsigned short*)&GV6.s70)[0] & D_1f8003c6) {
                (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
                o->step = 0xb;
                o->state = 0;
            }
            if (((volatile unsigned short*)&GV6.s70)[0] & 0x10) {
                (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
                STEP(o);
                o->step = 0xb;
                o->state = 0;
            }
            if (((volatile unsigned short*)&GV6.s70)[0] & 0x40) {
                STEP(o);
                playerPerchFlipLandCheck(o);
            }
        }
        break;
    case 3:
        FUN_8001fec0(o);
        if (((volatile unsigned short*)&GV6.s70)[0] & D_1f8003c6) {
            (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
            o->step = 0xb;
            o->state = 0;
        }
        if (((volatile unsigned short*)&GV6.s70)[0] & 0x10) {
            (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
            o->step = 0xb;
            o->state = 0;
        }
        if (((volatile unsigned short*)&GV6.s70)[0] & 0x40) {
            (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
            o->y.p.whole += 9;
            probeCollisionAtDepthC(o, o->h->p.whole, o->y.p.whole);
            setanim(o, 10);
            o->step = 0xd;
            o->state = 0;
        }
        break;
    }
    playerCheckPerchLanding(o);
}
