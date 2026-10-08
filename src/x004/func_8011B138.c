// FUNC 8011b138 224 X004
// MATCHING 8011b138 224
#include "TOBJ.H"

void func_8011B138(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state = ((TObj *)o->d90)->state;
        if (o->state == 3) {
            o->velY = 0x600;
            o->d88 += 0x600;
        }
        break;
    case 1:
        o->d88 = (o->d88 + 0x20) & 0xfff;
        o->velY = (o->velY + 0x20) & 0xfff;
        if (o->velY >= 0x600) o->state++;
        break;
    case 2:
        break;
    case 3:
        o->d88 = (o->d88 - 0x20) & 0xfff;
        o->velY = (o->velY - 0x20) & 0xfff;
        if (o->velY == 0) o->state++;
        break;
    case 4:
        o->state = ((TObj *)o->d90)->state;
        break;
    }
}
