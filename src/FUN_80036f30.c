// FUNC 80036f30 372 MAIN0
// MATCHING 80036f30 372
#include "TOBJ.H"
extern void FUN_8003566c(TObj *);
extern void FUN_80036cb8(TObj *);

void FUN_80036f30(TObj *o)
{
    switch (o->step) {
    case 0:
        FUN_8003566c(o);
        if (o->state == 6 || o->state == 7)
            break;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3: o->d8c = 0; break;
        case 4: o->d8c = 0x20; break;
        case 5: o->d8c = 0xe0; break;
        case 6: o->d8c = 0x40; break;
        case 7: o->d8c = 0xc0; break;
        }
        break;
    case 1:
        FUN_8003566c(o);
        if (o->state == 6 || o->state == 7)
            break;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3: o->d8c = 0; break;
        case 4: o->d8c = 0x20; break;
        case 5: o->d8c = 0xe0; break;
        case 6: o->d8c = 0x40; break;
        case 7: o->d8c = 0xc0; break;
        }
        break;
    case 2:
        FUN_8003566c(o);
        if (o->state == 6 || o->state == 7)
            break;
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3: o->d8c = 0; break;
        case 4: o->d8c = 0x20; break;
        case 5: o->d8c = 0xe0; break;
        case 6: o->d8c = 0x40; break;
        case 7: o->d8c = 0xc0; break;
        }
        break;
    case 3:
    case 4:
        FUN_80036cb8(o);
        break;
    }
}
