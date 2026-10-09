// FUNC 80133a24 384 X001
// MATCHING 80133a24 384
#include "TOBJ.H"

void func_80133A24(TObj *o)
{
    unsigned char d;
    int v;
    TObj *q;
    unsigned short t, s;

    d = ((TObj *)o->d90)->d84 - o->d84;
    q = *(TObj **)&o->wa8;
    t = q->w7a;
    s = q->w74;
    if (d == 0) o->state = 0;
    switch (o->state) {
    case 0:
        if (d) {
            if (d < 0x80) o->state = 1;
            else o->state = 2;
            o->timer = t;
        }
        return;
    case 1:
        if (d >= 0x80) {
            o->state = 2;
            o->d84 = (unsigned char)(o->d84 - 1);
            return;
        } else if (o->timer) {
            short k = s;
            o->timer--;
            if (k < d) {
                v = ((TObj *)o->d90)->d84 - k;
                o->timer = 0;
            } else return;
        } else {
            o->d84 = (unsigned char)(o->d84 + 1);
            return;
        }
        break;
    case 2:
        if (d < 0x80) {
            o->state = 1;
            o->d84 = (unsigned char)(o->d84 + 1);
            return;
        } else if (o->timer) {
            short k = s;
            o->timer--;
            if (d < 0x100 - k) {
                v = ((TObj *)o->d90)->d84 + k;
                o->timer = 0;
            } else return;
        } else {
            v = o->d84 - 1;
        }
        break;
    default:
        return;
    }
    o->d84 = (unsigned char)v;
}
