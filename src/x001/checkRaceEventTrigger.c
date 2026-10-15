// FUNC 8013a664 616 X001
// MATCHING 8013a664 616
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern unsigned char D_800A60F8;
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC, D_1F8003C4;

#define W24(o) (((unsigned short *)(o))[0x12])

int checkRaceEventTrigger(TObj *o)
{
    TObj *q = *(TObj **)&o->category;
    int r = 0;

    switch (GCD.bce59) {
    case 1:
        switch (GCD.bcede) {
        case 0:
            if (PLAYER.h->p.whole >= 0x19b)
                r = 1;
            break;
        case 1:
            r = 1;
            break;
        case 2:
            r = 1;
            break;
        }
        break;
    case 2:
        if (PLAYER.b69 == 0)
            break;
        if ((unsigned short)(PLAYER.h->p.whole - q->h->p.whole + 0x20) >= 0x40)
            break;
        if ((unsigned short)(PLAYER.y.p.whole + 0x6d4) >= 4)
            break;
        if (W24(o) != 0) {
            r = 1;
            break;
        }
        if (D_8009D2B0 == 1 && (D_1F8001FC & D_1F8003C4)) {
            GVARS.b42 = 1;
            D_800A60F8 = 1;
            r = 1;
            W24(o) = 1;
        }
        break;
    case 3:
        if (PLAYER.b04 != 1)
            break;
        if (PLAYER.b69 == 0)
            break;
        if ((unsigned short)(PLAYER.h->p.whole - q->h->p.whole + 0x20) >= 0x40)
            break;
        if (PLAYER.y.p.whole < -0x17c)
            break;
        if (W24(o) != 0) {
            r = 1;
            break;
        }
        if (D_8009D2B0 == 1 && (D_1F8001FC & D_1F8003C4)) {
            r = 1;
            GVARS.b42 = 1;
            D_800A60F8 = 1;
            GVARS.b3f = 1;
            W24(o) = 1;
        }
        break;
    case 0xff:
        r = 1;
        break;
    }
    return r;
}
