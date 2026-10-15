// FUNC 800f4f70 452 I0NFO2
// MATCHING 800f4f70 452
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void func_800F44DC(TObj *o);
extern void func_800F46D4(TObj *o);
extern void func_800F48C8(TObj *o);
extern void func_800F4AFC(TObj *o);
extern void func_800F4D3C(TObj *o);
extern unsigned char D_800A60A3;
extern void **D_800FEF6C[][3];

static inline void setAnim(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = D_800FEF6C[o->subtype][0][f];
    func_8001FE94(o, n);
    S16(o, 0xD2) = f;
}

void func_800F4F70(TObj *o)
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
            func_800F44DC(o);
            break;
        case 2:
            func_800F46D4(o);
            break;
        case 3:
            func_800F48C8(o);
            break;
        case 4:
            func_800F4AFC(o);
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
        func_800F4D3C(o);
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
