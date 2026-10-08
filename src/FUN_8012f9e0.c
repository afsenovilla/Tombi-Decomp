// FUNC 8012f9e0 348 X000
// MATCHING 8012f9e0 348
#include "tobj.h"
extern void f(int);
#define P ((TObj *)o->d90)
void FUN_8012f9e0(TObj *o)
{
    unsigned int u;
    int i;
    o->y = P->y;
    switch (o->state) {
    case 0:
        o->d8c = P->d8c & 0xff;
        o->timer = P->timer;
        o->w22 = P->timer;
        break;
    case 1:
        o->timer = P->timer;
        o->w22 = P->timer;
        o->animFrame = P->animFrame;
        if (o->timer < 10) o->d8c = (o->d8c - 6) & 0xff;
        switch (o->animFrame) {
        case 1: u = o->d8c + 8; break;
        case 3: u = o->d8c - 8; break;
        default: return;
        }
        o->d8c = u & 0xff;
        break;
    case 2:
        i = o->d8c;
        if (i != 0) {
            if ((unsigned)(i - 6) >= 0xf5) o->d8c = 0;
            else o->d8c = (i + 6) & 0xff;
            if (o->d8c == 0) f(0x2c);
        }
        break;
    case 3:
        break;
    case 4:
        o->d8c = 0;
    }
}
