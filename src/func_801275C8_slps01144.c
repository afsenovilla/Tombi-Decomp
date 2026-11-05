// near-twin of PAL updateA00L2Type2C (tools/jp_autofix.py)
// VERSION SLPS_01144 x00 801275C8 1788
// FLAGS -O2 -G0 -fno-strength-reduce
// JP names: playSFX -> func_8001E2D4, MOVE_TABLE_1/2 -> D_800716DC/D_800716F4, the anim tables D_8013A1E0.. -> D_80137E68..,
// KOMA_PIG_ANIM_BOXES -> D_801374F4, probeWallAt -> func_80041224, enemyProbeGround -> func_801261DC (the PAL names
// do not link in the JP build: the report showed 97.9 % while the code was identical).
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct P { unsigned char b0; unsigned char b1; unsigned short w2; short w4; } P;
extern char D_800716F4[];
extern char D_800716DC[];
extern char MOVE_TABLE_0[];
extern void *D_80137E68[];
extern void *D_80137E6C[];
extern void *D_80137E70[];
extern void *D_80137E90[];
extern void *D_80137E94[];
extern void *D_80137E98[];
extern unsigned char D_801374F4[];
extern void func_8001E2D4(int);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern short func_80041224_slps01144(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int func_801261DC_slps01144(TObj *);

#define SETBOX(o, A) \
    { \
        unsigned short *a = A; unsigned char *b; \
        o->anim = a; \
        b = D_801374F4 + a[1] * 4; \
        o->box0 = *b++; \
        o->box1 = *b++; \
        o->box2 = b[0]; \
        o->box3 = b[1]; \
    }
#define ANIMT(o) o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff

static __inline__ void move(TObj *o)
{
    P *p = (P *)&o->wb4;
    if (o->movetab != 0) {
        applyAnimVelocityX(o, 1 - o->animFrame);
    }
    if (o->b9d != 0) {
        if (o->animFrame != (o->b9d & 1)) {
            o->movetab = 0;
        }
        o->b9d = 0;
    }
    if (o->animFrame == 1) p->w4 = 0x10;
    else p->w4 = -0x10;
    if (func_80041224_slps01144(o, o->h->p.whole + p->w4, o->y.p.whole, 1 - o->animFrame)) {
        o->movetab = 0;
    }
}

void func_801275C8(TObj *o)
{
    P *p = (P *)&o->wb4;
    unsigned char t;

    switch (o->substep) {
    case 0:
        func_8001E2D4(7);
        o->timer = 4;
        o->velV = -0x400;
        o->movetab = D_800716F4;
        o->wac = 0x20;
        o->substep++;
        SETBOX(o, D_80137E68[0]);
        ANIMT(o);
        o->b9c = 1;
    case 1:
        AnimAdvanceWithBox(o);
        move(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->substep++;
        }
        return;
    case 2:
        AnimAdvanceWithBox(o);
        move(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (func_801261DC_slps01144(o)) {
            if (o->movetab == 0) {
                o->d8c = p->w2;
                o->substep = 4;
                o->timer = 0x3c;
                o->wac = 0x22;
                SETBOX(o, D_80137E70[0]);
                ANIMT(o);
            } else {
                o->b9c = 1;
                o->velV = -0x280;
                o->d8c = 0;
                o->movetab = D_800716DC;
                o->substep++;
                p->b1 = 1;
                o->wac = 0x21;
                SETBOX(o, D_80137E6C[0]);
                ANIMT(o);
            }
        }
        return;
    case 3:
        {
            int x;
            if (o->animFrame != 0) x = o->d8c - 20;
            else x = o->d8c + 20;
            o->d8c = x & 0xff;
        }
        move(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (func_801261DC_slps01144(o)) {
                if (o->movetab == 0) {
                    o->d8c = p->w2;
                    o->substep = 4;
                    o->timer = 0x3c;
                    o->wac = 0x22;
                    SETBOX(o, D_80137E70[0]);
                    ANIMT(o);
                } else if (t = p->b1, p->b1 = t + 0xff, t != 0) {
                    o->velV = ~(o->velV - 0x80) + 1;
                    if (o->velV < 0) o->b9c = 1;
                    o->movetab = MOVE_TABLE_0;
                } else {
                    o->d8c = p->w2;
                    o->timer = 0x3c;
                    o->wac = 0x22;
                    o->substep++;
                    SETBOX(o, D_80137E70[0]);
                    ANIMT(o);
                }
            }
        }
        return;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x2a;
            o->substep++;
            SETBOX(o, D_80137E90[0]);
            ANIMT(o);
        }
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->active = 1;
            o->timer = 0x3c;
            o->wac = 0x2b;
            o->substep++;
            SETBOX(o, D_80137E94[0]);
            ANIMT(o);
        }
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x2c;
            o->substep++;
            SETBOX(o, D_80137E98[0]);
            ANIMT(o);
        }
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->state = 1;
            o->b69 = 0;
            o->substep = 2;
            o->b68 = 0;
            o->b9c = 0;
        }
        break;
    default:
        return;
    }
    o->y.p.whole += 2;
    func_801261DC_slps01144(o);
}
#endif
