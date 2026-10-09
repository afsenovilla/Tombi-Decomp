// FUNC 8011ab70 484 X010
// MATCHING 8011ab70 484
#include "TOBJ.H"

int Rand(void);

void func_8011AB70(TObj *o)
{
    if (o->subtype == 0) {
        if (o->step == 0) {
            o->d8c += 0x20;
            if (o->d8c >= 0x400) goto inc;
        }
        return;
    }
    switch (o->step) {
    case 0:
        o->step++;
        if (o->b6b == 1) {
            o->timer = 4;
        } else {
            o->step = 2;
        }
        break;
    case 1:
        if (--o->timer == -1) goto inc;
        break;
    case 2:
        o->step++;
        if (Rand() & 1) {
            o->velX = 0x10;
        } else {
            o->velX = -0x10;
        }
    case 3:
        o->y.p.whole += 3;
        o->d84 = (o->d84 + o->velX) & 0xfff;
        if (o->y.p.whole - o->d34 < 10) break;
        switch (o->w98) {
        case 1:
            ((TObj *)o->d90)->w98 = 2;
            ((TObj *)o->d94)->w98 = 3;
            break;
        case 2:
            ((TObj *)o->d90)->w98 = 2;
            break;
        case 3:
            ((TObj *)o->d94)->w98 = 3;
            break;
        }
    inc:
        o->step++;
        break;
    case 4:
        if (o->visible == 0) {
            o->b04 = 3;
        } else {
            o->y.p.whole += 2;
        }
        break;
    }
}
