// FUNC 800fcfa8 320 I0NFO2
// MATCHING 800fcfa8 320
#include "TOBJ.H"
#include "raw7.h"
extern short D_800A604A, D_800A604E;
extern short D_80100788[];

void func_800FCFA8(TObj *o)
{
    int i;
    short *p, *q, *b;
    short px = D_800A604A, py = D_800A604E;
    int pad[1];
    i = 0;
    b = D_80100788;
    q = b + 1;
    p = b;
    for (; i < 7; q += 2, i++, p += 2) {
        if (px >= p[0] && py >= q[0] && p[0] + 2 >= px && q[0] + 2 >= py) {
            o->step = 3;
            o->state = 0;
            S16(o, 0x24) = i;
            break;
        }
    }
    if ((unsigned short)(px - 0x64) < 0x1D && (unsigned short)(py - 0x1A4) < 9) {
        o->step = 8;
        o->state = 0;
        return;
    }
    if ((unsigned short)(px - 0x6E) < 0x13 && (unsigned short)(py - 0x15C) < 7) {
        o->step = 9;
        o->state = 0;
    } else if ((unsigned short)(px - 0x118) < 0x11 && (unsigned short)(py - 0x10C) < 5) {
        o->step = 0xB;
        o->state = 0;
    } else if ((unsigned short)(px - 0x127) < 0x12 && (unsigned short)(py - 0x196) < 5) {
        o->step = 0xA;
        o->state = 0;
    }
}
