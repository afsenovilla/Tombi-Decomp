// FUNC 8012338c 1940 X003
/* Real size 1940 B: includes the csv piece func_801237E4 (828 B), the tail of this function. */
/* score ~6 (word diffs) for the whole 1940 B (csv pieces func_8012338C + func_801237E4): case 0 loads o->state after the velV store (game: before), and in case 2 the game schedules li a0,1 (b9c/b5 constant) before li v0,3. Tried: statement-order hill-climbing in both blocks, chained stores, o->state = o->state + 1. */
#include "TOBJ.H"

typedef struct { unsigned char b4, b5; unsigned short b6; short b8; } Q;

extern char D_80077D0C[], D_80077CF4[], D_80077CDC[];
extern unsigned short *D_8013877C[], *D_80138764[], *D_80138760[], *D_80138780[], *D_80138784[], *D_80138788[];
extern unsigned char D_80135B30[];
extern void FUN_8001e4f0(int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern short FUN_8004065c(TObj *, short, short, short);
extern int func_801206F0(TObj *);
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8002b920(TObj *);

#define SETANIM(o, tbl)                                              \
    {                                                                \
        unsigned short *a = tbl[0];                                  \
        o->anim = a;                                                 \
        {                                                            \
            unsigned char *pp = &D_80135B30[a[1] * 4];               \
            o->box0 = *pp++;                                         \
            o->box1 = *pp++;                                         \
            o->box2 = *pp;                                           \
            o->box3 = pp[1];                                         \
        }                                                            \
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;      \
    }

#define MOVE(o, dv)                                                              \
    {                                                                            \
        Q *r = (Q *)&o->wb4;                                                     \
        if (o->movetab != 0)                                                     \
            FUN_8001fa88(o, 1 - o->animFrame);                                   \
        if ((o->b9d & 2) && o->animFrame != (o->b9d & 1))                        \
            o->movetab = 0;                                                      \
        if (o->animFrame == 1)                                                   \
            r->b8 = 0x10;                                                        \
        else                                                                     \
            r->b8 = -0x10;                                                       \
        if (FUN_8004065c(o, o->h->p.whole + r->b8, o->y.p.whole, 1 - o->animFrame)) \
            o->movetab = 0;                                                      \
        o->velV += dv;                                                           \
        o->y.raw += o->velV << 8;                                                \
    }

void func_8012338C(TObj *o)
{
    Q *q = (Q *)&o->wb4;
    unsigned char t;

    switch (o->state) {
    case 0:
        FUN_8001e4f0(0xf);
        o->velV = -0x400;
        o->movetab = D_80077D0C;
        o->b9c = 1;
        o->wac = 0x21;
        o->b69 = 0;
        o->d8c = 0;
        o->animFrame = 1 - o->w7a;
        o->state = o->state + 1;
        SETANIM(o, D_8013877C);
    case 1:
        MOVE(o, 0x40);
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        break;
    case 2:
        MOVE(o, 0x40);
        if (func_801206F0(o)) {
            if (o->movetab == 0) {
                o->d8c = q->b6;
                o->state = 4;
                o->timer = 0x168;
                o->wac = 0x1b;
                SETANIM(o, D_80138764);
                FUN_8002b920(o);
            } else {
                o->active = 3;
                o->b9c = 1;
                o->velV = -0x280;
                o->d8c = 0;
                o->movetab = D_80077CF4;
                o->state++;
                q->b5 = 1;
                o->b69 = 0;
                o->wac = 0x1a;
                SETANIM(o, D_80138760);
            }
        }
        break;
    case 3:
        if (o->animFrame)
            o->d8c = (o->d8c - 0x14) & 0xff;
        else
            o->d8c = (o->d8c + 0x14) & 0xff;
        MOVE(o, 0x30);
        if (o->velV > 0) {
            o->b9c = 2;
            if (func_801206F0(o)) {
                if (o->movetab == 0) {
                    o->d8c = q->b6;
                    o->state = 4;
                    o->timer = 0x168;
                    o->wac = 0x1b;
                    SETANIM(o, D_80138764);
                    FUN_8002b920(o);
                } else {
                    t = q->b5;
                    q->b5 = t + 0xff;
                    if (t != 0) {
                        o->velV = ~(o->velV - 0x80) + 1;
                        if (o->velV < 0)
                            o->b9c = 1;
                        if (o->velV < -0x300)
                            o->velV = -0x300;
                        o->movetab = D_80077CDC;
                    } else {
                        o->d8c = q->b6;
                        o->timer = 0x168;
                        o->wac = 0x1b;
                        o->state++;
                        SETANIM(o, D_80138764);
                        FUN_8002b920(o);
                    }
                }
            }
        }
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x22;
            o->state++;
            SETANIM(o, D_80138780);
        }
        o->y.p.whole += 2;
        func_801206F0(o);
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->timer = 0x78;
            o->wac = 0x23;
            o->state++;
            SETANIM(o, D_80138784);
        }
        o->y.p.whole += 2;
        func_801206F0(o);
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x24;
            o->state++;
            SETANIM(o, D_80138788);
        }
        o->y.p.whole += 2;
        func_801206F0(o);
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            *(signed char *)&o->b0f = -9;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 0;
            o->substep = 6;
            o->b69 = 0;
            o->b68 = 0;
            o->b9c = 0;
        }
        o->y.p.whole += 2;
        func_801206F0(o);
        break;
    }
}
