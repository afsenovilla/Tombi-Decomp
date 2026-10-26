// FUNC 800ea258 356 I0NFO3
// MATCHING 800ea258 356
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FEC0(TObj *o);
extern TObj *PLAYER_EXT;
extern signed char D_8009C975[];

void tunnelPlayerFallExitStep(TObj *o)
{
    TObj *p;
    short s;
    switch (o->state) {
    case 0:
        o->active = 4;
        PLAYER_EXT->timer = 0;
        o->d8c = 0;
        S16(o, 0xE0) = 0;
        o->timer = 0x1E;
        o->w22 = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        o->state++;
        break;
    case 1:
        func_8001FEC0(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        s = --o->timer;
        if (s <= 0) {
            o->timer = 0x3C;
            D_8009C975[0] = 3;
            o->state++;
        }
        break;
    case 2:
        func_8001FEC0(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        if (o->timer != 0) {
            if (--o->timer <= 0) {
                p = *(TObj **)0x1F8001D4;
                p->w4c = 7;
                p->w4e = 0;
            }
        }
        break;
    }
}
