// FUNC 80121398 1776 X003
// MATCHING 80121398 1776
// VERSION SLPS_01144 x03 80120260 1784
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
extern unsigned char D_80130FD4[];
extern A *D_80133BDC[], *D_80133BE0[], *D_80133BE4[], *D_80133C04[], *D_80133C08[], *D_80133C0C[];
extern char MOVE_TABLE_2[], MOVE_TABLE_1[], MOVE_TABLE_0[];
extern void FUN_8001e4f0(int);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern short probeWallAt(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int komaPigCheckLanded(TObj *);

static __inline__ void setanimbox(TObj *o, A *a)
{
    unsigned char *p;

    o->anim = a;
    p = D_80130FD4 + a->w2 * 4;
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;
}

static __inline__ void wall(TObj *o)
{
    unsigned char *q = (unsigned char *)&o->wb4;

    if (o->movetab) applyAnimVelocityX(o, 1 - o->animFrame);
    if ((o->b9d & 2) && o->animFrame != (o->b9d & 1)) o->movetab = 0;
    if (o->animFrame == 1)
        *(short *)(q + 4) = 0x10;
    else
        *(short *)(q + 4) = -0x10;
    if (probeWallAt(o, o->h->p.whole + *(short *)(q + 4), o->y.p.whole, 1 - o->animFrame))
        o->movetab = 0;
}

void func_80120260(TObj *o)
{
    unsigned char *q = (unsigned char *)&o->wb4;

    switch (o->substep) {
    case 0:
        FUN_8001e4f0(7);
        o->timer = 4;
        o->velV = -0x400;
        o->movetab = MOVE_TABLE_2;
        o->wac = 0x19;
        o->substep++;
        setanimbox(o, D_80133BDC[0]);
        o->b9c = 1;
    case 1:
        AnimAdvanceWithBox(o);
        wall(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->substep++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        wall(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (!komaPigCheckLanded(o)) break;
        if (o->movetab == 0) {
            unsigned short w = *(unsigned short *)(q + 2);
            o->substep = 4;
            o->timer = 0x3c;
            o->wac = 0x1b;
            o->d8c = w;
            setanimbox(o, D_80133BE4[0]);
        } else {
            o->b9c = 1;
            o->velV = -0x280;
            o->d8c = 0;
            o->movetab = MOVE_TABLE_1;
            o->substep++;
            q[1] = 1;
            o->wac = 0x1a;
            setanimbox(o, D_80133BE0[0]);
        }
        break;
    case 3:
        if (o->animFrame)
            o->d8c = (o->d8c - 0x14) & 0xff;
        else
            o->d8c = (o->d8c + 0x14) & 0xff;
        wall(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (!komaPigCheckLanded(o)) break;
        if (o->movetab == 0) {
            unsigned short w = *(unsigned short *)(q + 2);
            o->substep = 4;
            o->timer = 0x3c;
            o->wac = 0x1b;
            o->d8c = w;
            setanimbox(o, D_80133BE4[0]);
        } else {
            unsigned char c = q[1];
            unsigned short w;
            q[1] = c + 0xff;
            if (c) {
                o->velV = ~(o->velV - 0x80) + 1;
                if (o->velV < 0) o->b9c = 1;
                o->movetab = MOVE_TABLE_0;
                return;
            }
            w = *(unsigned short *)(q + 2);
            o->timer = 0x3c;
            o->wac = 0x1b;
            o->d8c = w;
            o->substep++;
            setanimbox(o, D_80133BE4[0]);
        }
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x22;
            o->substep++;
            setanimbox(o, D_80133C04[0]);
        }
        goto tail;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->active = 1;
            o->timer = 0x3c;
            o->wac = 0x23;
            o->substep++;
            setanimbox(o, D_80133C08[0]);
        }
        goto tail;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x24;
            o->substep++;
            setanimbox(o, D_80133C0C[0]);
        }
        goto tail;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->b69 = 0;
            o->state = 0;
            o->substep = 6;
            o->b68 = 0;
            o->b9c = 0;
        }
    tail:
        o->y.p.whole += 2;
        komaPigCheckLanded(o);
        break;
    }
}
#else
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { unsigned char b0, b1; unsigned short w2; short w4; } X;
extern char MOVE_TABLE_0[];
extern char MOVE_TABLE_1[];
extern char MOVE_TABLE_2[];
extern B KOMA_PIG_ANIM_BOXES[];
extern A *KOMA_PIG_ANIMS[];
extern void playSFX(int);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern short probeWallAt(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int komaPigCheckLanded(TObj *);

#define SETANIM(k) \
    o->anim = KOMA_PIG_ANIMS[k]; \
    { unsigned char *p; { B *t = KOMA_PIG_ANIM_BOXES; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define COLL() \
    { \
        X *x = (X *)((char *)o + 0xb4); \
        if (o->movetab != 0) { \
            applyAnimVelocityX(o, 1 - o->animFrame); \
        } \
        if (o->b9d & 2) { \
            if (o->animFrame != (o->b9d & 1)) { \
                o->movetab = 0; \
            } \
        } \
        if (o->animFrame == 1) { \
            x->w4 = 0x10; \
        } else { \
            x->w4 = -0x10; \
        } \
        if (probeWallAt(o, o->h->p.whole + x->w4, o->y.p.whole, 1 - o->animFrame)) { \
            o->movetab = 0; \
        } \
    }

void komaPigLeap(TObj *o)
{
    X *x2 = (X *)((char *)o + 0xb4);
    unsigned char v;

    switch (o->substep) {
    case 0:
        playSFX(7);
        o->timer = 4;
        o->velV = -0x400;
        o->movetab = MOVE_TABLE_2;
        o->wac = 0x19;
        o->substep++;
        SETANIM(0x19);
        o->b9c = 1;
    case 1:
        AnimAdvanceWithBox(o);
        COLL();
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->substep++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        COLL();
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (komaPigCheckLanded(o) == 0) break;
        if (o->movetab == 0) {
            o->d8c = x2->w2;
            o->substep = 4;
            o->timer = 0x1e;
            o->wac = 0x1b;
            SETANIM(0x1b);
            break;
        }
        o->b9c = 1;
        o->velV = -0x280;
        o->d8c = 0;
        o->movetab = MOVE_TABLE_1;
        o->substep++;
        x2->b1 = 1;
        o->wac = 0x1a;
        SETANIM(0x1a);
        break;
    case 3:
        o->d8c = ((o->animFrame != 0) ? o->d8c - 0x14 : o->d8c + 0x14) & 0xff;
        COLL();
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (komaPigCheckLanded(o) == 0) break;
        if (o->movetab == 0) {
            o->d8c = x2->w2;
            o->substep = 4;
            o->timer = 0x1e;
            o->wac = 0x1b;
            SETANIM(0x1b);
            break;
        }
        v = x2->b1--;
        if (v != 0) {
            o->velV = ~(o->velV - 0x80) + 1;
            if (o->velV < 0) {
                o->b9c = 1;
            }
            o->movetab = MOVE_TABLE_0;
            break;
        }
        o->d8c = x2->w2;
        o->timer = 0x1e;
        o->wac = 0x1b;
        o->substep++;
        SETANIM(0x1b);
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x22;
            o->substep++;
            SETANIM(0x22);
        }
        o->y.p.whole += 2;
        komaPigCheckLanded(o);
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->timer = 0x3c;
            o->wac = 0x23;
            o->substep++;
            SETANIM(0x23);
        }
        o->y.p.whole += 2;
        komaPigCheckLanded(o);
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x24;
            o->substep++;
            SETANIM(0x24);
        }
        o->y.p.whole += 2;
        komaPigCheckLanded(o);
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->b69 = 0;
            o->state = 0;
            o->substep = 6;
            o->b68 = 0;
            o->b9c = 0;
        }
        o->y.p.whole += 2;
        komaPigCheckLanded(o);
        break;
    }
}
#endif
