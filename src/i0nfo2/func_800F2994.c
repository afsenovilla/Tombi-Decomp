// FUNC 800f2994 444 I0NFO2
// MATCHING 800f2994 444
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void func_800F2164(TObj *o);
extern void func_800F2350(TObj *o);
extern void func_800F2538(TObj *o);
extern void func_800F2760(TObj *o);
extern void func_800F1EA4(TObj *o);
extern unsigned char D_800A60A3;
extern void **D_800FEEC0[][3];

void func_800F2994(TObj *o)
{
    switch (o->step) {
    case 0:
        switch (U16(o, 0xB8)) {
        case 0:
            switch (o->state) {
            case 0:
                o->anim = D_800FEEC0[o->subtype][0][(short)o->animFrame];
                func_8001FE94(o, 0);
                o->state++;
            case 1:
                func_8001FEC0(o);
                break;
            }
            break;
        case 1:
            func_800F2164(o);
            break;
        case 2:
            func_800F2350(o);
            break;
        case 3:
            func_800F2538(o);
            break;
        case 4:
            func_800F2760(o);
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
        func_800F1EA4(o);
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
