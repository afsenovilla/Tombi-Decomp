// FUNC 8012418c 484 X010
// MATCHING 8012418c 484
#include "TOBJ.H"

void func_8012418C(TObj *o)
{
    switch (o->subtype) {
    case 0:
        switch (o->velX & 3) {
        case 0:
            o->velH += 8;
            if (o->velH > 0x180) o->velH = 0x180;
            break;
        case 1:
            o->velH -= 8;
            if (o->velH < 0) o->velH = 0;
            break;
        case 2:
            o->velH += 8;
            if (o->velH > 0x80) o->velH = 0x80;
            break;
        case 3:
            o->velH -= 8;
            if (o->velH < 0x40) o->velH = 0x40;
            break;
        }
        break;
    case 1:
        switch (o->velX & 3) {
        case 0:
            o->velH += 8;
            if (o->velH > 0x280) o->velH = 0x280;
            break;
        case 1:
            o->velH -= 8;
            if (o->velH < 0) o->velH = 0;
            break;
        case 2:
            o->velH += 8;
            if (o->velH > 0x1c0) o->velH = 0x1c0;
            break;
        case 3:
            o->velH -= 8;
            if (o->velH < 0x100) o->velH = 0x100;
            break;
        }
        break;
    }
}
