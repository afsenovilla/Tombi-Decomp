// FUNC 800f3a6c 444 I0NFO2
// MATCHING 800f3a6c 444
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void func_800F328C(TObj *o);
extern void func_800F3468(TObj *o);
extern void func_800F3640(TObj *o);
extern void func_800F3858(TObj *o);
extern void func_800F3058(TObj *o);
extern unsigned char D_800A60A3;
extern void **D_800FEEFC[][3];

void func_800F3A6C(TObj *o)
{
    switch (o->step) {
    case 0:
        switch (U16(o, 0xB8)) {
        case 0:
            switch (o->state) {
            case 0:
                o->anim = D_800FEEFC[o->subtype][0][o->animFrame];
                func_8001FE94(o, 0);
                o->state++;
            case 1:
                func_8001FEC0(o);
                break;
            }
            break;
        case 1:
            func_800F328C(o);
            break;
        case 2:
            func_800F3468(o);
            break;
        case 3:
            func_800F3640(o);
            break;
        case 4:
            func_800F3858(o);
            break;
        case 5:
        case 6:
            break;
        }
        if (o->b68 != 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_800F3058(o);
        break;
    case 2:
        func_8001FEC0(o);
        break;
    }
    if (D_800A60A3 != 0) {
        if (D_800A60A3 == U8(o, 0xB6)) {
            o->b6b = 0x80;
        } else {
            o->b6b = 0;
        }
    } else if (U16(o, 0xB6) != 0) {
        o->b6b = 0;
    } else {
        o->b6b = 0x80;
    }
}
