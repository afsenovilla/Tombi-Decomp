// FUNC 80126e54 1576 X004
// MATCHING 80126e54 1576
#include "spad.h"
#include "gvars.h"
#include "player.h"
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
extern unsigned short D_1F8001C8;
extern void *D_80134608[], *D_80134610[], *D_80134614[];
extern char D_801348F8[];
extern char D_8013490C[];
extern char D_80134950[];
extern char D_801349AC[];
static void *D_801345F4[4] = {
    D_801348F8, D_8013490C, D_80134950, D_801349AC,
};
extern char MOVE_TABLE_0[], D_80077D90[];
extern short FUN_8005e420(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int Rand(void);
extern int cullObjectPerspective(TObj *);
extern void blackCrowPerched(TObj *);
extern void blackCrowSwoop(TObj *);
extern void blackCrowGrabbed(TObj *);
extern short probeCollisionAtDepthB(TObj *, short, short);
extern void applyAnimVelocityXY(TObj *, int);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern void freeObjectLayer2(TObj *);

#define X(o) ((S *)(o))

void updateBlackCrow(TObj *o)
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
        o->d3c = SPAD.i2dc;
        o->movetab = MOVE_TABLE_0;
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
        if (GVARS.b42) {
            cullObjectPerspective(o);
            break;
        }
        if (!o->b9e) {
            switch (o->step) {
            case 0:
                blackCrowPerched(o);
                break;
            case 1:
                blackCrowSwoop(o);
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
        if (cullObjectPerspective(o)) {
            AnimAdvance(o);
            X(o)->wca = 1;
        } else {
            X(o)->wca = 0;
        }
        if (!((*(Fix16 **)&PLAYER.obj.d)->p.whole < o->d->p.whole)) o->w08 = FUN_8005e420(0xe0, 0x1f0);
        else o->w08 = FUN_8005e420(0xe0, 0x1f2);
        X(o)->wc0 = D_1F8001C8;
        break;
    case 2:
        if (GVARS.b42) {
            cullObjectPerspective(o);
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
                if (!probeCollisionAtDepthB(o, o->h->p.whole + d, o->y.p.whole + 0x18)) {
                    applyAnimVelocityXY(o, 0);
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
                    applyAnimVelocityXY(o, 1);
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
            if (cullObjectPerspective(o)) {
                AnimAdvance(o);
                X(o)->wca = 1;
            } else {
                X(o)->wca = 0;
            }
            break;
        case 1:
            blackCrowGrabbed(o);
            if (cullObjectPerspective(o)) {
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
                o->movetab = MOVE_TABLE_0;
                o->wac = 0x18;
                o->state++;
                o->anim = D_80134614[0];
                AnimLoadDuration(o);
                break;
            case 1:
                applyAnimVelocityX(o, 1 - o->animFrame);
                o->velV += 0x40;
                if (o->velV > 0x400) o->velV = 0x400;
                o->y.raw += o->velV << 8;
                break;
            }
            if (o->animFrame & 1) o->d8c = (unsigned char)(o->d8c + 0x14);
            else o->d8c = (unsigned char)(o->d8c - 0x14);
            if (!cullObjectPerspective(o)) o->b04 = 3;
            AnimAdvance(o);
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
