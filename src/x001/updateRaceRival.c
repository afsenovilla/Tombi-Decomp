// FUNC 801312f8 740 X001
// MATCHING 801312f8 740
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

extern void *D_8013E740[];
extern TObj PLAYER;
void AnimLoadDuration(TObj *o);
int ObjCullRegister(TObj *o);
short TileCollideAt(TObj *o, int x, int y);
void freeObjectLayer2(TObj *o);
void raceRivalWaitStart(TObj *o);
void raceRivalRun(TObj *o);
void raceRivalRunCourse(TObj *o);
void raceRivalTurn(TObj *o);
void raceRivalFall(TObj *o);
void raceRivalFinish(TObj *o);
void raceRivalIdleWander(TObj *o);
void raceRivalHopAway(TObj *o);

void updateRaceRival(TObj *o)
{
    int d;

    switch (o->b04) {
    case 0:
        d = ((int*)&SPAD.i2d4)[0];
        o->w1e = 0x12;
        o->b0d = 1;
        o->w08 = 0x7813;
        o->d3c = d;
        o->b0f = PLAYER.b0f + 1;
        o->b68 = 0;
        o->wb6 = 0;
        o->wb4 = 0;
        o->d8c = 0;
        o->d84 = 0;
        o->box0 = 0x18;
        o->box1 = 0x30;
        o->box2 = 0x18;
        o->box3 = 0x30;
        o->b04++;
        o->wac = 0;
        o->anim = D_8013E740[0];
        AnimLoadDuration(o);
        switch (GCD.bce59) {
        case 1:
        case 3:
            o->step = 0;
            break;
        case 2:
            o->step = 1;
            break;
        }
        break;
    case 1:
        if (o->subtype) {
            ObjCullRegister(o);
            switch (o->step) {
            case 0:
                raceRivalWaitStart(o);
                break;
            case 1:
                raceRivalRun(o);
                break;
            case 2:
                raceRivalRunCourse(o);
                break;
            case 3:
                raceRivalTurn(o);
                break;
            case 4:
                raceRivalFall(o);
                break;
            case 5:
                raceRivalFinish(o);
                break;
            }
        } else {
            ObjCullRegister(o);
            switch (o->step) {
            case 0:
                if (o->state == 0) {
                    o->anim = D_8013E740[0];
                    AnimLoadDuration(o);
                    o->y.p.whole += 4;
                    if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
                        o->d84 = (-SPAD.s27e << 2) & 0xff;
                    }
                    o->state++;
                }
                break;
            case 1:
                raceRivalIdleWander(o);
                break;
            case 3:
                raceRivalHopAway(o);
                break;
            }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
