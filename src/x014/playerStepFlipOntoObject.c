// FUNC 800f8d2c 2076 X014
// MATCHING 800f8d2c 2076
/* Debt: volatile animFrame read + volatile d88 store in case 2 pin the lhu above "sw a0,0x88" (scheduler). */
/* Tried (debt2): raw o->d84=0 after a temp read of animFrame breaks the CSE (reload ok, no volatile read needed), but sched2 still puts "sw a0,0x88" above the lhu (score 2) for every statement order/var split/pointer copy; only the volatile pair pins it. */
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    unsigned char b00, b01;
    unsigned short w02;
    char p04[4];
    unsigned char b08, b09;
    char p0a[2];
    unsigned short w0c;
    short w0e;
    char p10[0x28 - 0x10];
    unsigned short w28, w2a, w2c, w2e;
} P;
extern P *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern TObj *PLAYER_HELD_OBJECT;
extern TObj *BITTEN_OBJECT;
extern int DAT_8009c934;
extern unsigned short PAD_PRESSED;
extern unsigned short DAT_1f8003c6;
extern unsigned char PLAYER_SWING_ANIM_PAIRS[];
extern char D_80011108[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(TObj *, int);
extern int AnimAdvance(TObj *);
extern void FUN_800efc04(TObj *);
extern int FUN_8001fe0c(unsigned char, int);
extern void playerFlipOntoObjectPlace(TObj *, int, int);

#define BUTTONS(o) \
    if (*(volatile unsigned short *)&PAD_PRESSED & DAT_1f8003c6) { \
        PLAYER_EXT->b09 = 0; \
        (o)->step = 0xb; \
        (o)->state = 0; \
        (*(unsigned char*)&o->waa) = 0; \
        GRABBED_OBJECT->b69 = 0; \
    } \
    if (*(volatile unsigned short *)&PAD_PRESSED & 0x10) { \
        PLAYER_EXT->b09 = 0; \
        (o)->step = 0xb; \
        (o)->state = 0; \
        (*(unsigned char*)&o->waa) = 0; \
        GRABBED_OBJECT->b69 = 0; \
    }

static __inline__ void nextspd(TObj *o, P *pl)
{
    unsigned char *t = &PLAYER_SWING_ANIM_PAIRS[--o->wb2 * 2];
    pl->w0e = t[0];
    pl->w02 = t[1];
}

void playerStepFlipOntoObject(TObj *o)
{
    unsigned char *t;
    int a;
    int v;
    short q;
    short d;

    switch (o->state) {
    case 0:
        if ((*(unsigned char*)&o->wac) >= 2) {
            BITTEN_OBJECT = PLAYER_HELD_OBJECT;
            BITTEN_OBJECT->b04 = 2;
            BITTEN_OBJECT->step = 2;
            BITTEN_OBJECT->state = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        DAT_8009c934 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        PLAYER_EXT->b00 = 0;
        GRABBED_OBJECT->b69 = 1;
        o->wb6 = -0x420;
        PLAYER_EXT->w02 = 0x10;
        PLAYER_EXT->w0e = 0;
        o->wb2 = 2;
        PLAYER_EXT->b08 = 0;
        PLAYER_EXT->b09 = 0;
        PLAYER_EXT->w0c = 0;
        PLAYER_EXT->w28 = 0xffff;
        PLAYER_EXT->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d30 = GRABBED_OBJECT->h->p.whole + o->wb8;
        o->d34 = GRABBED_OBJECT->y.p.whole + o->wba;
        FUN_8001e5f4(4, 0x7f);
        (*(unsigned char*)&o->waa) = 1;
        o->state++;
    case 1:
        o->wb6 += PLAYER_EXT->w0e;
        if ((unsigned short)o->wb6 < 0x800)
            PLAYER_EXT->b08 = 1;
        if ((unsigned short)(o->wb6 - 0x800) < 0x800)
            PLAYER_EXT->b08 = 0;
        if ((unsigned short)(o->wb6 + 0x7ff) < 0x800)
            PLAYER_EXT->b08 = 0;
        if ((unsigned short)(o->wb6 + 0xfff) < 0x800)
            PLAYER_EXT->b08 = 1;
        if (o->wb6 == 0) {
            P *pl = PLAYER_EXT;
            if (pl->w0e > 0) {
                nextspd(o, pl);
            }
        }
        a = o->wb6 >> 4;
        PLAYER_EXT->w2e = 0xffff;
        o->anim = D_80011108;
        FUN_8001fe94(o, 0);
        PLAYER_EXT->b01 = 0x10;
        playerFlipOntoObjectPlace(o, q, a);
        if (o->wb2 < 2)
            o->state++;
        break;
    case 2:
        o->wb6 += PLAYER_EXT->w0e;
        if ((unsigned short)o->wb6 < 0x800)
            PLAYER_EXT->b08 = 1;
        if ((unsigned short)(o->wb6 - 0x800) < 0x800)
            PLAYER_EXT->b08 = 0;
        if ((unsigned short)(o->wb6 + 0x7ff) < 0x800)
            PLAYER_EXT->b08 = 0;
        if ((unsigned short)(o->wb6 + 0xfff) < 0x800)
            PLAYER_EXT->b08 = 1;
        if (o->wb6 == 0) {
            P *pl = PLAYER_EXT;
            if (pl->w0e > 0) {
                nextspd(o, pl);
            } else {
                pl->b09 = 1;
            }
        }
        PLAYER_EXT->w2c = 7;
        if (PLAYER_EXT->w2e != 7) {
            PLAYER_EXT->w2c = 7;
            FUN_800efc04(o);
            FUN_8001fe94(o, 0);
            PLAYER_EXT->w2e = PLAYER_EXT->w2c;
        }
        AnimAdvance(o);
        {
            int b = o->wb6 >> 4;
            q = (unsigned)b >> 2 & 0x3f;
            playerFlipOntoObjectPlace(o, q, b);
        }
        if (o->wb2 == 0) {
            o->d8c = 0;
            o->state++;
        }
        if (PLAYER_EXT->b09 == 0)
            break;
        BUTTONS(o);
        if (*(volatile unsigned short *)&PAD_PRESSED & 0x40) {
            v = 0x10;
            PLAYER_EXT->b09 = 0;
            (*(unsigned char*)&o->wac) = 1;
            o->b9e = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1)
                v = 0xf0;
            {
                int w; int f;
                f = *(volatile unsigned short *)&o->animFrame;
                *(volatile int *)&o->d88 = v;
                o->d8c = 0;
                v = (int)o->h;
                w = ((Fix16 *)v)->p.whole;
                if (f & 1)
                    ((Fix16 *)v)->p.whole = w + 10;
                else
                    ((Fix16 *)v)->p.whole = w - 10;
            }
            o->step = 2;
            o->state = 3;
            o->y.p.whole += 4;
            (*(unsigned char*)&o->waa) = 0;
            GRABBED_OBJECT->b69 = 0;
        }
        break;
    case 3:
        o->d30 = GRABBED_OBJECT->h->p.whole + o->wb8;
        o->d34 = GRABBED_OBJECT->y.p.whole + o->wba;
        o->h->p.whole = o->d30;
        if (o->animFrame & 1)
            o->y.p.whole = FUN_8001fe0c((GRABBED_OBJECT->d8c >> 4) + 0x70, GRABBED_OBJECT->box0) + o->d34 + 0x10;
        else
            o->y.p.whole = FUN_8001fe0c((GRABBED_OBJECT->d8c >> 4) + 0x10, GRABBED_OBJECT->box0) + o->d34 + 0x10;
        AnimAdvance(o);
        BUTTONS(o);
        if (*(volatile unsigned short *)&PAD_PRESSED & 0x40) {
            v = 0x10;
            PLAYER_EXT->b09 = 0;
            (*(unsigned char*)&o->wac) = 1;
            o->timer = 10;
            o->b9e = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->d84 = 0;
            o->y.p.whole += 8;
            if (o->animFrame & 1)
                v = 0xf0;
            o->step = 2;
            o->state = 3;
            (*(unsigned char*)&o->waa) = 0;
            o->d88 = v;
            o->d8c = 0;
            GRABBED_OBJECT->b69 = 0;
        }
        break;
    }
}
