// FUNC 801247d8 380 X004
// MATCHING 801247d8 380
#include "TOBJ.H"

void FUN_801247d8(TObj *o)
{
    int v;
    short w;
    o->y.raw = ((TObj *)o->d90)->y.raw;
    switch (o->state) {
    case 0:
        o->d8c = *(unsigned char *)(o->d90 + 0x8c);
        o->timer = ((TObj *)o->d90)->timer;
        w = ((TObj *)o->d90)->w22;
        o->b6a = 1;
        o->box2 = 10;
        o->box3 = 10;
        o->w22 = w;
        break;
    case 1:
        o->b6a = 1;
        o->box2 = 10;
        o->box3 = 10;
        o->timer = ((TObj *)o->d90)->timer;
        o->w22 = ((TObj *)o->d90)->w22;
        o->animFrame = ((TObj *)o->d90)->animFrame;
        if (o->timer < 10)
            o->d8c = (o->d8c + 6) & 0xff;
        switch (o->animFrame) {
        case 1:
            o->d8c = (o->d8c + 8) & 0xff;
            break;
        case 3:
            o->d8c = (o->d8c - 8) & 0xff;
            break;
        }
        break;
    case 2:
        if (o->d8c != 0) {
            v = o->d8c - 6;
            if ((unsigned)v >= 0xf5)
                o->d8c = 0;
            else
                o->d8c = v & 0xff;
        }
        break;
    case 3:
        if (((TObj *)o->d90)->state == 4)
            o->b6a = 0;
        break;
    case 4:
        o->d8c = 0;
        o->box2 = 0x20;
        o->box3 = 0x20;
        break;
    }
}
