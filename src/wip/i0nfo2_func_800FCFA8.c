// FUNC 800fcfa8 320 I0NFO2
#include "TOBJ.H"
extern short PLAYER_X, PLAYER_Y;
extern short D_80100788[][2];

void func_800FCFA8(TObj *o)
{
    int i;
    short x = PLAYER_X, y = PLAYER_Y;
    short *px, *py;
    i = 0;
    px = D_80100788[0];
    py = px + 1;
    for (; i < 7; i++, px += 2, py += 2) {
        if (x >= *px && y >= *py && x <= *px + 2 && y <= *py + 2) {
            o->step = 3;
            o->state = 0;
            *(short *)&o->anim = i;
            break;
        }
    }
    if ((unsigned short)(x - 0x64) < 0x1D && (unsigned short)(y - 0x1A4) < 9) {
        o->step = 8;
    } else if ((unsigned short)(x - 0x6E) < 0x13 && (unsigned short)(y - 0x15C) < 7) {
        o->step = 9;
    } else if ((unsigned short)(x - 0x118) < 0x11 && (unsigned short)(y - 0x10C) < 5) {
        o->step = 0xB;
    } else if ((unsigned short)(x - 0x127) < 0x12 && (unsigned short)(y - 0x196) < 5) {
        o->step = 0xA;
    } else {
        return;
    }
    o->state = 0;
}
