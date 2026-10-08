// FUNC 800ed024 564 X001
// MATCHING 800ed024 564
#include "TOBJ.H"
extern int FUN_800202b4(TObj *);
extern void FUN_80018838(TObj *);
extern short DAT_1f8001c8;
extern unsigned int DAT_8009c978[];
extern void (*DAT_80114c58[])(TObj *);

void FUN_800ed024(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->box0 = 0x15;
        o->ba4 = 0;
        o->box1 = 0x2a;
        o->b68 = 0;
        o->b69 = 0;
        o->step = 0;
        o->state = 0;
        o->b04++;
        o->box2 = 0x36;
        o->box3 = 0x36;
        o->w1e = o->b6b;
        switch (DAT_1f8001c8) {
        case 0:
            o->ba7 = 0x10;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            break;
        case 1:
            o->ba7 = 0x11;
            o->d84 = 0;
            o->d88 = -0x400;
            o->d8c = 0;
            break;
        case 2:
            o->ba7 = 0x11;
            o->d84 = 0;
            o->d88 = 0x800;
            o->d8c = 0;
            break;
        case 3:
            o->ba7 = 0x11;
            o->d84 = 0;
            o->d88 = 0x400;
            o->d8c = 0;
            break;
        }
        o->b0f = 0;
        o->b0a = o->ba7;
        if (o->subtype < 32 ? (DAT_8009c978[0] & (1 << o->subtype)) : (DAT_8009c978[1] & (1 << (o->subtype - 32)))) {
            o->_pad0e[0] = 1;
            o->b0d = 0x20;
        } else {
            o->_pad0e[0] = 0;
        }
        o->w98 = 0;
        break;
    case 1:
        if (FUN_800202b4(o) && o->state == 0) {
            if (o->b69 != 0) o->step = 1;
            else if (o->b68 != 0) o->step = 2;
        }
        if (o->step != 0) DAT_80114c58[o->step](o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
