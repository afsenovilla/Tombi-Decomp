// FUNC 80120a7c 1776 X004
// MATCHING 80120a7c 1776
#include "TOBJ.H"
typedef struct { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
extern unsigned char D_80130FD4[];
extern A *D_80133BDC[], *D_80133BE0[], *D_80133BE4[], *D_80133C04[], *D_80133C08[], *D_80133C0C[];
extern char D_80077D0C[], D_80077CF4[], D_80077CDC[];
extern void FUN_8001e4f0(int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern short FUN_8004065c(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int func_8011FDD4(TObj *);

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

    if (o->movetab) FUN_8001fa88(o, 1 - o->animFrame);
    if ((o->b9d & 2) && o->animFrame != (o->b9d & 1)) o->movetab = 0;
    if (o->animFrame == 1)
        *(short *)(q + 4) = 0x10;
    else
        *(short *)(q + 4) = -0x10;
    if (FUN_8004065c(o, o->h->p.whole + *(short *)(q + 4), o->y.p.whole, 1 - o->animFrame))
        o->movetab = 0;
}

void func_80120A7C(TObj *o)
{
    unsigned char *q = (unsigned char *)&o->wb4;

    switch (o->substep) {
    case 0:
        FUN_8001e4f0(7);
        o->timer = 4;
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->wac = 0x20;
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
        if (!func_8011FDD4(o)) break;
        if (o->movetab == 0) {
            unsigned short w = *(unsigned short *)(q + 2);
            o->substep = 4;
            o->timer = 0x1e;
            o->wac = 0x22;
            o->d8c = w;
            setanimbox(o, D_80133BE4[0]);
        } else {
            o->b9c = 1;
            o->velV = -0x280;
            o->d8c = 0;
            o->movetab = D_80077CF4;
            o->substep++;
            q[1] = 1;
            o->wac = 0x21;
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
        if (!func_8011FDD4(o)) break;
        if (o->movetab == 0) {
            unsigned short w = *(unsigned short *)(q + 2);
            o->substep = 4;
            o->timer = 0x1e;
            o->wac = 0x22;
            o->d8c = w;
            setanimbox(o, D_80133BE4[0]);
        } else {
            unsigned char c = q[1];
            unsigned short w;
            q[1] = c + 0xff;
            if (c) {
                o->velV = ~(o->velV - 0x80) + 1;
                if (o->velV < 0) o->b9c = 1;
                o->movetab = D_80077CDC;
                return;
            }
            w = *(unsigned short *)(q + 2);
            o->timer = 0x1e;
            o->wac = 0x22;
            o->d8c = w;
            o->substep++;
            setanimbox(o, D_80133BE4[0]);
        }
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x2a;
            o->substep++;
            setanimbox(o, D_80133C04[0]);
        }
        goto tail;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->timer = 0x3c;
            o->wac = 0x2b;
            o->substep++;
            setanimbox(o, D_80133C08[0]);
        }
        goto tail;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x2c;
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
        func_8011FDD4(o);
        break;
    }
}
