// FUNC 80124954 388 X004
// MATCHING 80124954 388
#include "TOBJ.H"

extern unsigned short D_8009C962;
extern void playSFX(int);

void func_80124954(TObj *o)
{
    o->y.raw = ((TObj *)o->d90)->y.raw;
    switch (o->state) {
    case 0:
        o->d8c = *(unsigned char *)&((TObj *)o->d90)->d8c;
        o->timer = ((TObj *)o->d90)->timer;
        o->w22 = ((TObj *)o->d90)->timer;
        break;
    case 1:
        o->timer = ((TObj *)o->d90)->timer;
        o->w22 = ((TObj *)o->d90)->timer;
        o->animFrame = ((TObj *)o->d90)->animFrame;
        if (o->timer < 10) o->d8c = (o->d8c - 6) & 0xff;
        switch (o->animFrame) {
        case 1: o->d8c = (o->d8c + 8) & 0xff; break;
        case 3: o->d8c = (o->d8c - 8) & 0xff; break;
        }
        break;
    case 2:
        if (o->d8c) {
            if (o->d8c < 6 || o->d8c > 0xfa) o->d8c = 0;
            else o->d8c = (o->d8c + 6) & 0xff;
            if (o->d8c == 0) {
                if (D_8009C962 < 4) playSFX(0xf8);
                else playSFX(0xfa);
            }
        }
        break;
    case 3:
        break;
    case 4:
        o->d8c = 0;
        break;
    }
}
