// VERSION SLPS_01144 x01 8012C8CC 2808
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

typedef struct { short w0; unsigned short w2; short w4; unsigned short w6; } AH;
typedef struct {
    unsigned char a0, a1;   /* 0xb4 */
    unsigned short b;       /* 0xb6 */
    short wb8;              /* 0xb8 */
} Sub;

extern AH *D_8013B344[], *D_8013B32C[], *D_8013B330[], *D_8013B348[], *D_8013B34C[], *D_8013B350[];
extern unsigned char D_8013ADE8[];
extern char D_800716F4[], D_800716DC[], MOVE_TABLE_0[];
extern void func_8001E2D4(int);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern short func_80041224(TObj *, short, short, short);
extern short func_80040E40(TObj *o, short x, short y);
extern int AnimAdvanceWithBox(TObj *);

#define SETANIM(A) { unsigned char *pv; \
    o->anim = (A)[0]; \
    pv = &D_8013ADE8[((AH *)o->anim)->w2 * 4]; \
    o->box0 = *pv++; \
    o->box1 = *pv++; \
    o->box2 = *pv++; \
    o->box3 = *pv; \
    o->animTimer = ((AH *)o->anim)->w6 & 0x3fff; }

static __inline__ int land2(TObj *o)
{
    short t;
    short w;

    if (o->b69 == 1) {
        o->d8c = 0;
        o->wb2 = 0;
        o->b69 = 0;
        o->wae = -1;
        o->b9c = 0;
        return 1;
    }
    if (func_80040E40(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        t = SPAD.s27e;
        if (t < 0) t = -t;
        if (t >= 9) t = 8;
        if (SPAD.s27e < 0) t = -t;
        o->d8c = -t & 0xff;
        o->wb2 = t;
        w = SPAD.s284;
        o->wb6 = (-t << 2) & 0xff;
        o->b9c = 0;
        o->wae = w;
        return 1;
    }
    return 0;
}

static __inline__ void blk(TObj *o)
{
    Sub *s = (Sub *)&o->wb4;
    if (o->movetab) applyAnimVelocityX(o, 1 - o->animFrame);
    if ((o->b9d & 2) && o->animFrame != (o->b9d & 1)) o->movetab = 0;
    if (o->animFrame == 1) s->wb8 = 0x10;
    else s->wb8 = -0x10;
    if (func_80041224(o, o->h->p.whole + s->wb8, o->y.p.whole, 1 - o->animFrame)) o->movetab = 0;
}

void komaPigHit(TObj *o)
{
    Sub *t = (Sub *)&o->wb4;
    unsigned char c;

    switch (o->state) {
    case 0:
        o->b9d = 0;
        func_8001E2D4(0xf);
        o->velV = -0x400;
        o->b69 = 0;
        o->d8c = 0;
        o->animFrame = 1 - o->w7a;
        o->movetab = D_800716F4;
        o->wac = 10;
        o->state++;
        SETANIM(D_8013B344);
    case 1:
        blk(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b69 = 0;
            o->state++;
        }
        break;
    case 2:
        blk(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (!land2(o)) break;
        if (o->movetab == 0) {
            o->d8c = t->b;
            o->state = 4;
            o->timer = 0x168;
            o->wac = 5;
            SETANIM(D_8013B330);
            break;
        }
        o->active = 3;
        o->velV = -0x280;
        o->d8c = 0;
        o->movetab = D_800716DC;
        o->state++;
        t->a1 = 1;
        o->b69 = 0;
        o->wac = 4;
        SETANIM(D_8013B32C);
        break;
    case 3:
        if (o->animFrame) o->d8c = (unsigned char)(o->d8c - 0x14);
        else o->d8c = (unsigned char)(o->d8c + 0x14);
        blk(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV <= 0) break;
        if (!land2(o)) break;
        if (o->movetab == 0) {
            o->d8c = t->b;
            o->state = 4;
            o->timer = 0x168;
            o->wac = 5;
            SETANIM(D_8013B330);
            break;
        }
        c = t->a1--;
        if (c) {
            o->movetab = MOVE_TABLE_0;
            o->velV = ~(o->velV - 0x80) + 1;
            break;
        }
        o->d8c = t->b;
        o->timer = 0x168;
        o->wac = 5;
        o->state++;
        SETANIM(D_8013B330);
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0xb;
            o->state++;
            SETANIM(D_8013B348);
        }
        o->y.p.whole += 2;
        land2(o);
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->active = 1;
            o->timer = 0x78;
            o->wac = 0xc;
            o->state++;
            SETANIM(D_8013B34C);
        }
        o->y.p.whole += 2;
        land2(o);
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0xd;
            o->state++;
            SETANIM(D_8013B350);
        }
        o->y.p.whole += 2;
        land2(o);
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            *(signed char *)&o->b0f = -9;
            o->active = 1;
            o->b04 = 1;
            o->step = 3;
            o->state = 0;
            o->substep = 0;
            o->b69 = 0;
            o->b68 = 0;
        }
        o->y.p.whole += 2;
        land2(o);
        break;
    }
}
#endif
