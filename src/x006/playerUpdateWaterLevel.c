// FUNC 800ff0e8 796 X006
// MATCHING 800ff0e8 796
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

void playerUpdateWaterLevel(TObj *o)
{
    unsigned char s = o->step;
    short c;
    if (s == 0x3d || s == 0x3e || s == 0x42 || s == 0x43 || s == 0x44 || s == 0x47 || s == 0x48)
        return;
    switch (GAME.selectedArea) {
    case 0:
        if (GAME.selectedSection == 5) {
            o->w56 = -0x100;
            if ((unsigned short)(o->h->p.whole - 0xec) < 0x1cc) {
                c = o->y.p.whole < (-0xff);
                goto tail;
            }
        }
        break;
    case 1:
        if (GAME.selectedSection == 2) {
            o->w56 = -0xb1;
            if ((unsigned short)(o->h->p.whole - 0x128) < 0x5e0) {
                c = o->y.p.whole < (-0xb0);
                goto tail;
            }
        }
        break;
    case 4:
    case 12:
        switch (GAME.selectedSection) {
        case 4:
            o->w56 = -0x13;
            if ((unsigned short)(o->h->p.whole - 0x2f) < 0xf9) {
                c = o->y.p.whole < (-0x12);
                goto tail;
            }
            break;
        case 6:
            o->w56 = -0x39;
            if ((unsigned short)(o->h->p.whole - 0x87) < 0x19f) {
                c = o->y.p.whole < (-0x38);
                goto tail;
            }
            break;
        case 7:
            o->w56 = -0x1d;
            if ((unsigned short)(o->h->p.whole - 0x97) < 0x1bd) {
                c = o->y.p.whole < (-0x1c);
                goto tail;
            }
            break;
        case 8:
            o->w56 = -0xa3;
            if ((unsigned short)(o->h->p.whole + 0x10) < 0x94) {
                c = o->y.p.whole < (-0xa2);
                goto tail;
            }
            break;
        case 16:
            c = o->y.p.whole < (-0x15);
            o->w56 = -0x16;
                goto tail;
            break;
        }
        break;
    case 10:
        switch (GAME.selectedSection) {
        case 1:
        case 5:
            o->w56 = -0xe4;
            if (((unsigned char*)&GCD.bd078)[0] == 4) {
                c = o->y.p.whole < (-0xe3);
                goto tail;
            } else {
                ((unsigned char *)&o->da0)[1] = 0;
            }
            break;
        case 3:
        case 7:
            if ((unsigned short)(o->h->p.whole - 0x71) < 0x726) {
                if (GAME.purifiedAreas & 0x40) {
                    c = o->y.p.whole < (-0x8b);
                    o->w56 = -0x8c;
                goto tail;
                } else {
                    c = o->y.p.whole < (-0x3bf);
                    o->w56 = -0x3c0;
                goto tail;
                }
            }
            break;
        }
        break;
    tail:
        if (c) {
            GVARS.driftH = 0;
            GVARS.driftY = 0;
            break;
        }
        goto els;
    case 14:
        if (GAME.selectedSection == 2) {
            o->y.p.whole = -0xe4;
        els:
            GVARS.driftH = 0;
            GVARS.driftY = -0x40;
        }
        break;
    }
}
