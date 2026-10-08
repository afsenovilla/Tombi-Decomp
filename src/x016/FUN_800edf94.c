// FUNC 800edf94 504 X016
// MATCHING 800edf94 504
#include "TOBJ.H"
extern void FUN_800202b4(TObj *);
extern void FUN_800eddfc(TObj *);
extern void FUN_800edc14(TObj *);
extern void FUN_80018838(TObj *);
extern unsigned short DAT_8009c960;

void FUN_800edf94(TObj *o)
{
    switch (o->b04) {
    case 0:
        switch (o->b0c) {
        case 1:
            o->b0a = 0x11;
            o->box0 = 0;
            o->box1 = 0;
            o->box2 = 0;
            o->box3 = 0;
            o->active = 2;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            break;
        case 2:
            if (DAT_8009c960 == 0) o->da0 = 0;
        case 0:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 8;
            o->box3 = 0x10;
            o->b0a = 0x11;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            break;
        }
        o->b04++;
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->b0c) {
        case 1:
            FUN_800eddfc(o);
            break;
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            if (o->visible != 0) {
                switch (o->step) {
                case 0:
                    if (o->state == 0) o->state++;
                    if (o->b69 & 2) {
                        o->step = 1;
                        o->state = 0;
                    }
                    break;
                case 1:
                    FUN_800edc14(o);
                    break;
                }
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
