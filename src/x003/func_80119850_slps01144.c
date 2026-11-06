// JP x03 area mechanisms step (retail form, smaller than PAL updateAreaMechanisms)
// VERSION SLPS_01144 x03 80119850 1180
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "tobj.h"

extern unsigned char D_800A7C23;
extern unsigned char D_8009EA98;
extern unsigned char D_8009EC81;
extern unsigned char D_8009E983;
extern unsigned char D_8009EE6B;
extern unsigned char D_8009E4E7;
extern unsigned char D_8009E4EA;
extern unsigned char D_8009E4E6;
extern Fix16 *PLAYER_H;
extern void ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern int stepAreaSwitch(TObj *);

#define DOOR_STATES(o) \
    switch (o->state) { \
    case 0: \
        o->state = ((TObj *)o->d90)->state; \
        o->d30 = 0; \
        if (o->state == 3) { \
            o->d30 = 0; \
            o->h->p.whole += 0x20; \
        } \
        break; \
    case 1: \
        o->h->raw += 0x18000; \
        o->d30 += 0x18000; \
        if ((o->d30 >> 16) >= 0x20) o->state++; \
        break; \
    case 2: \
    case 3: \
        o->state = ((TObj *)o->d90)->state; \
        break; \
    case 4: \
        o->h->raw += -0x18000; \
        o->d30 += -0x18000; \
        if ((o->d30 >> 16) <= 0) o->state = 0; \
        break; \
    }

void func_80119850_slps01144(TObj *o)
{
    int r;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            r = stepAreaSwitch(o);
            if (r == 1) {
                if (D_800A7C23 == r) setEventComplete(41, 0);
                else setEventStarted(41, 0, 0);
            }
            break;
        case 1:
            DOOR_STATES(o)
            break;
        case 2:
            switch (o->state) {
            case 0:
                if (D_8009EA98) o->state = 2;
                else o->state++;
                break;
            case 1:
                if (PLAYER_H->p.whole < 37) PLAYER_H->p.whole = 37;
                o->d88 = 0;
                break;
            case 2:
                o->d88 = 1024;
                break;
            }
            break;
        case 3:
            switch (o->state) {
            case 0:
                o->state = ((TObj *)o->d90)->state;
                break;
            case 1:
                o->d88 = 0;
                break;
            case 2:
                o->d88 = 3072;
                break;
            }
            break;
        case 4:
            if (stepAreaSwitch(o)) {
                if (!(D_8009EE6B & 2)) setEventStarted(170, 0, 0);
                else setEventComplete(170, 0);
            }
            break;
        case 5:
            DOOR_STATES(o)
            break;
        case 8:
            r = stepAreaSwitch(o);
            if (D_8009EC81) {
                if (r == 2) o->substep = 1;
            } else {
                if (r == 1) setEventStarted(55, 0, 0);
            }
            switch (o->substep) {
            case 0:
                break;
            case 1:
                if (D_8009E983 == 0xff) {
                    o->substep = 0;
                    break;
                }
                setEventComplete(55, 0);
                D_8009E4E7 = 1;
                D_8009E4EA = 1;
                D_8009E4E6 = 1;
                o->timer = 240;
                o->substep++;
                break;
            case 2:
                if (--o->timer == -1) o->substep++;
                break;
            case 3:
                D_8009E4E7 = 0;
                D_8009E4EA = 0;
                D_8009E4E6 = 0;
                o->substep = 0;
                break;
            }
            break;
        case 9:
            DOOR_STATES(o)
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
