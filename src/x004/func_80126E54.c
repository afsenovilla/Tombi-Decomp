// FUNC 80126e54 1576 X004
// MATCHING 80126e54 1576
#include "TOBJ.H"
typedef struct {
    TObj o;
    short wc0;
    short pc2;
    short wc4, wc6, wc8;
    short wca;
    char pcc[0xd2 - 0xcc];
    short wd2;
} S;
extern unsigned char D_8009C942;
extern int D_1F8002DC;
extern unsigned short D_1F8001C8;
extern Fix16 *D_800A607C;
extern void *D_80134608[], *D_801345F4[], *D_80134610[], *D_80134614[];
extern char D_80077CDC[], D_80077D90[];
extern short FUN_8005e420(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int Rand(void);
extern int func_8011EC78(TObj *);
extern void func_80126D04(TObj *);
extern void func_80126688(TObj *);
extern void func_80126C50(TObj *);
extern short func_800411CC(TObj *, short, short);
extern void FUN_8001fa20(TObj *, int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_80018790(TObj *);

#define X(o) ((S *)(o))

void func_80126E54(TObj *o)
{
    int d;

    switch (o->b04) {
    case 0:
        o->w98 = 4;
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box2 = 10;
        o->box3 = 0x1e;
        o->active = 1;
        o->w1e = 1;
        o->b0d = 1;
        o->w08 = FUN_8005e420(0xe0, 0x1f0);
        o->b0a = 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->wac = 0x15;
        o->anim = D_80134608[0];
        AnimLoadDuration(o);
        o->d3c = D_1F8002DC;
        o->movetab = D_80077CDC;
        *(signed char *)&o->b0f = -9;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
        o->b04++;
        o->timer = 1;
        o->wb4 = 0;
        X(o)->wca = 0;
        o->d64 = 0x1000;
        {
            unsigned short c8 = D_1F8001C8;
            X(o)->wc4 = o->a.p.whole;
            X(o)->wc6 = o->y.p.whole;
            X(o)->wc8 = o->b.p.whole;
            X(o)->wc0 = c8;
        }
        break;
    case 1:
        if (D_8009C942) {
            func_8011EC78(o);
            break;
        }
        if (!o->b9e) {
            switch (o->step) {
            case 0:
                func_80126D04(o);
                break;
            case 1:
                func_80126688(o);
                break;
            }
        } else if (!o->b9f) {
            X(o)->wd2 = o->h->p.whole;
            o->b9f = 0xf;
        } else {
            o->h->p.whole = (short)(X(o)->wd2 - 2) + (Rand() & 3);
            if (--o->b9f == 0) {
                o->b9e = 0;
                o->h->p.whole = X(o)->wd2;
            }
        }
        if (func_8011EC78(o)) {
            AnimAdvance(o);
            X(o)->wca = 1;
        } else {
            X(o)->wca = 0;
        }
        if (!(D_800A607C->p.whole < o->d->p.whole)) o->w08 = FUN_8005e420(0xe0, 0x1f0);
        else o->w08 = FUN_8005e420(0xe0, 0x1f2);
        X(o)->wc0 = D_1F8001C8;
        break;
    case 2:
        if (D_8009C942) {
            func_8011EC78(o);
            break;
        }
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0:
                o->timer = 0x78;
                o->w22 = 1;
                o->wac = 0x10;
                o->anim = D_801345F4[0];
                AnimLoadDuration(o);
                o->movetab = D_80077D90;
                o->state++;
                break;
            case 1:
                if (--o->timer == 0) {
                    o->wac = 0x17;
                    o->anim = D_80134610[0];
                    AnimLoadDuration(o);
                    o->state++;
                }
                d = o->animFrame ? -0x10 : 0x10;
                if (!func_800411CC(o, o->h->p.whole + d, o->y.p.whole + 0x18)) {
                    FUN_8001fa20(o, 0);
                    o->w22++;
                }
                break;
            case 2:
                if (*(unsigned short *)&o->wb4) {
                    if (--o->w22 == 0) {
                        o->active = 1;
                        o->b04 = 1;
                        o->step = 0;
                        o->state = 3;
                        o->substep = 0;
                    }
                    FUN_8001fa20(o, 1);
                } else {
                    if (--o->w22 == 0) {
                        o->active = 1;
                        o->b04 = 1;
                        o->step = 0;
                        o->state = 0;
                        o->substep = 0;
                    }
                }
                break;
            }
            if (func_8011EC78(o)) {
                AnimAdvance(o);
                X(o)->wca = 1;
            } else {
                X(o)->wca = 0;
            }
            break;
        case 1:
            func_80126C50(o);
            if (func_8011EC78(o)) {
                AnimAdvance(o);
                X(o)->wca = 1;
            } else {
                X(o)->wca = 0;
            }
            break;
        case 2:
            switch (o->state) {
            case 0:
                o->b0b = 1;
                o->b0f = 4;
                o->velV = -0x400;
                o->movetab = D_80077CDC;
                o->wac = 0x18;
                o->state++;
                o->anim = D_80134614[0];
                AnimLoadDuration(o);
                break;
            case 1:
                FUN_8001fa88(o, 1 - o->animFrame);
                o->velV += 0x40;
                if (o->velV > 0x400) o->velV = 0x400;
                o->y.raw += o->velV << 8;
                break;
            }
            if (o->animFrame & 1) o->d8c = (unsigned char)(o->d8c + 0x14);
            else o->d8c = (unsigned char)(o->d8c - 0x14);
            if (!func_8011EC78(o)) o->b04 = 3;
            AnimAdvance(o);
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
