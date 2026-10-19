// FUNC 801281fc 1532 X003
// MATCHING 801281fc 1532
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"

typedef struct { short a, b, x, y, z; } Q;
typedef struct { char p[0xd2]; short wd2; } E;
typedef void (*Fn)(TObj *);

extern void spikyLizardSubtype0Step();
extern void spikyLizardSubtype1Step();
extern void spikyLizardSubtype2Step();
extern void spikyLizardSubtype3Step();
static Fn D_80135D6C[4] = {
    (Fn)spikyLizardSubtype0Step, (Fn)spikyLizardSubtype1Step, (Fn)spikyLizardSubtype2Step, (Fn)spikyLizardSubtype3Step,
};
extern unsigned short D_80135D3C[];
extern int ObjCullRegister(TObj *);
extern int Rand(void);
extern short FUN_80040278(TObj *, int, int);
extern int spikyLizardCheckWall(TObj *, int);
extern void spikyLizardLand(TObj *);
extern void spikyLizardIdle(TObj *);
extern void spikyLizardWalk(TObj *);
extern void spikyLizardRun(TObj *);
extern void spikyLizardTurnToPlayer(TObj *);
extern void spikyLizardHit(TObj *);
extern void spikyLizardGrabbed(TObj *);
extern void spikyLizardDefeated(TObj *);
extern void despawnObjectOffscreen(TObj *);
extern void freeObjectLayer2(TObj *);
extern int isObjectInView(TObj *);

void updateSpikyLizard(TObj *o)
{
    Q *q = (Q *)&o->wb4;
    short yy;
    unsigned int v;

    switch (o->b04) {
    case 0:
        if (o->step == 0) {
            q->x = o->h->p.whole;
            q->y = o->y.p.whole;
            q->z = o->d->p.whole;
            o->w1e = 0xb;
            o->step++;
            o->d3c = ((int*)&SPAD.i2dc)[0];
        } else {
            q->a = 0;
            q->b = 0;
            *(signed char *)&o->b0f = -9;
            o->d84 = 0;
            o->d88 = 0;
            o->b69 = 0;
            o->w98 = 3;
            o->w9a = 0;
            o->b68 = 0;
            o->b9e = 0;
            o->step = 0;
            *(int *)&o->anim = 0;
            o->b04++;
        }
        break;
    case 1:
        if (GVARS.b42) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        if (!(o->b68 | o->b9e)) {
            switch (o->step) {
            case 0:
                D_80135D6C[o->subtype](o);
                break;
            case 1:
                switch (o->state) {
                case 0:
                    spikyLizardLand(o);
                    break;
                case 1:
                    spikyLizardIdle(o);
                    break;
                case 2:
                    spikyLizardWalk(o);
                    break;
                case 3:
                    spikyLizardRun(o);
                    break;
                case 4:
                    spikyLizardTurnToPlayer(o);
                    break;
                }
                break;
            }
        } else {
            if (o->b9e) {
                if (o->b9f == 0) {
                    ((E *)o)->wd2 = o->h->p.whole;
                    o->b9f = 15;
                } else {
                    o->h->p.whole = (short)(((E *)o)->wd2 - 2) + (Rand() & 3);
                    if (--o->b9f == 0) {
                        o->b9e = 0;
                        o->h->p.whole = ((E *)o)->wd2;
                    }
                }
            } else {
                if (o->w9a == 0) {
                    o->d30 = o->h->p.whole;
                    o->w9a = 0x14;
                } else {
                    o->h->p.whole = (short)(o->d30 - 2) + (Rand() & 3);
                    if (o->wac != 0x1e && q->b == 0) {
                        if (o->w7a) o->d30--;
                        else o->d30++;
                        if (spikyLizardCheckWall(o, o->w7a)) {
                            if (o->w7a) o->d30++;
                            else o->d30--;
                        }
                        yy = o->y.p.whole;
                        o->y.p.whole = yy + 2;
                        if (o->b69 == 1) {
                            o->b69 = 0;
                        } else if (FUN_80040278(o, o->h->p.whole, (short)(yy + 0x10))) {
                            if (o->wac >= 0xc) {
                                o->d8c = 0;
                            } else {
                                v = (((*(unsigned short*)&SPAD.s27e) << 2) + o->d8c) & 0xff;
                                if (v != 0) {
                                    if (v < 0x80) o->d8c = o->d8c - 1;
                                    else o->d8c = o->d8c + 1;
                                    o->d8c = *(unsigned char *)&o->d8c;
                                }
                            }
                        }
                    }
                    if (--o->w9a == 0) {
                        o->b68 = 0;
                        if (o->wac == 0x1e || q->b != 0) o->h->p.whole = o->d30;
                    }
                }
            }
        }
        o->b9d = 0;
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            spikyLizardHit(o);
            if (!o->visible) despawnObjectOffscreen(o);
            break;
        case 1:
            spikyLizardGrabbed(o);
            break;
        case 2:
            spikyLizardDefeated(o);
            if (!o->visible) o->b04 = 3;
            break;
        }
        break;
    case 3:
        if (o->subtype >= 2) {
            (*(short *)o->d90)--;
            freeObjectLayer2(o);
        } else {
            *(signed char *)&o->b0f = -9;
            o->timer = D_80135D3C[Rand() & 7] + 0x12c;
            o->b0b = 0;
            o->h->p.whole = q->x;
            o->y.p.whole = q->y;
            o->d->p.whole = q->z;
            o->b04++;
        }
        break;
    case 4:
        if (o->timer) {
            o->timer--;
        } else if (((SPAD.s1f8 + SPAD.i198) & 0x3f) == 0 && !isObjectInView(o)) {
            o->active = 1;
            o->b04 = 0;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
