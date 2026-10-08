// FUNC 8011c494 312 X009
// MATCHING 8011c494 312
#include "TOBJ.H"

void FUN_8011c494(TObj *o)
{
    switch (o->b0c) {
    case 0:
        o->d84 = (o->d84 + 0x20) & 0xfff;
        if (o->d84 >= 0x400)
            o->step++;
        break;
    case 1:
        o->d84 = (o->d84 - 0x20) & 0xfff;
        if (o->d84 <= 0xc00)
            o->step++;
        break;
    case 2:
        o->y.raw += 0x8000;
        o->d84 += 8;
        if (--o->timer == -1)
            o->step++;
        break;
    case 3:
        if (--o->timer == -1)
            o->step++;
        break;
    case 4:
        o->d8c = (o->d8c + 0x20) & 0xfff;
        if (o->d8c >= 0x400)
            o->step++;
        break;
    case 5:
        o->d8c = (o->d8c - 0x20) & 0xfff;
        if (o->d8c <= 0xc00)
            o->step++;
        break;
    }
}
