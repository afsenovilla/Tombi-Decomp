// FUNC 800edb20 452 I0NFO3
// MATCHING 800edb20 452
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void func_800ECC9C(TObj *o);
extern void func_800ECE94(TObj *o);
extern void func_800ED088(TObj *o);
extern void func_800ED2BC(TObj *o);
extern void func_800ED7A8(TObj *o);
extern unsigned char D_800A60A3;
extern void **D_800EED64[][3];

static inline void setAnim(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = D_800EED64[o->subtype][0][f];
    func_8001FE94(o, n);
    S16(o, 0xD2) = f;
}

void func_800EDB20(TObj *o)
{
    switch (o->step) {
    case 0:
        switch (U16(o, 0xB8)) {
        case 0:
            switch (o->state) {
            case 0:
                setAnim(o, 0, 0);
                o->state++;
            case 1:
                func_8001FEC0(o);
                break;
            }
            break;
        case 1:
            func_800ECC9C(o);
            break;
        case 2:
            func_800ECE94(o);
            break;
        case 3:
            func_800ED088(o);
            break;
        case 4:
            func_800ED2BC(o);
            break;
        }
        if (o->b68 != 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_800ED7A8(o);
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
