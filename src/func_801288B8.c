// FUNC 801288b8 1780 X000
// MATCHING 801288b8 1780
#include "TOBJ.H"
typedef struct P { unsigned char b0; unsigned char b1; unsigned short w2; short w4; } P;
extern char D_80077D0C[];
extern char D_80077CF4[];
extern char D_80077CDC[];
extern void *D_8013A1E0[];
extern void *D_8013A1E4[];
extern void *D_8013A1E8[];
extern void *D_8013A208[];
extern void *D_8013A20C[];
extern void *D_8013A210[];
extern unsigned char D_80138FD8[];
extern void FUN_8001e4f0(int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern short func_8004065C(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int FUN_801274cc(TObj *);

#define SETBOX(o, A) \
    { \
        unsigned short *a = A; unsigned char *b; \
        o->anim = a; \
        b = D_80138FD8 + a[1] * 4; \
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
        FUN_8001fa88(o, 1 - o->animFrame);
    }
    if (o->b9d != 0) {
        if (o->animFrame != (o->b9d & 1)) {
            o->movetab = 0;
        }
        o->b9d = 0;
    }
    if (o->animFrame == 1) p->w4 = 0x10;
    else p->w4 = -0x10;
    if (func_8004065C(o, o->h->p.whole + p->w4, o->y.p.whole, 1 - o->animFrame)) {
        o->movetab = 0;
    }
}

void func_801288B8(TObj *o)
{
    P *p = (P *)&o->wb4;
    unsigned char t;

    switch (o->substep) {
    case 0:
        FUN_8001e4f0(7);
        o->timer = 4;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->wac = 0x20;
        o->substep++;
        SETBOX(o, D_8013A1E0[0]);
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
        if (FUN_801274cc(o)) {
            if (o->movetab == 0) {
                o->d8c = p->w2;
                o->substep = 4;
                o->timer = 0x1e;
                o->wac = 0x22;
                SETBOX(o, D_8013A1E8[0]);
                ANIMT(o);
            } else {
                o->b9c = 1;
                o->velV = -0x280;
                o->d8c = 0;
                o->movetab = D_80077CF4;
                o->substep++;
                p->b1 = 1;
                o->wac = 0x21;
                SETBOX(o, D_8013A1E4[0]);
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
            if (FUN_801274cc(o)) {
                if (o->movetab == 0) {
                    o->d8c = p->w2;
                    o->substep = 4;
                    o->timer = 0x1e;
                    o->wac = 0x22;
                    SETBOX(o, D_8013A1E8[0]);
                    ANIMT(o);
                } else if (t = p->b1, p->b1 = t + 0xff, t != 0) {
                    o->velV = ~(o->velV - 0x80) + 1;
                    if (o->velV < 0) o->b9c = 1;
                    o->movetab = D_80077CDC;
                } else {
                    o->d8c = p->w2;
                    o->timer = 0x1e;
                    o->wac = 0x22;
                    o->substep++;
                    SETBOX(o, D_8013A1E8[0]);
                    ANIMT(o);
                }
            }
        }
        return;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x2a;
            o->substep++;
            SETBOX(o, D_8013A208[0]);
            ANIMT(o);
        }
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->timer = 0x3c;
            o->wac = 0x2b;
            o->substep++;
            SETBOX(o, D_8013A20C[0]);
            ANIMT(o);
        }
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x2c;
            o->substep++;
            SETBOX(o, D_8013A210[0]);
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
    FUN_801274cc(o);
}
