// FUNC 800fc240 468 X002
// MATCHING 800fc240 468
#include "TOBJ.H"

extern unsigned char D_8009C938;
extern unsigned char *D_8009C330;
extern int D_8009C960;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CE3D;

int func_800FC240(TObj *o)
{
    short r;

    if (D_8009C938 != 0) {
        return 0;
    }
    if (o->velY < 0x680) {
        return 0;
    }
    D_8009C330[8] = 0;
    *((unsigned char *)o + 0xd1) = 0;
    o->active = 3;
    *(short *)((char *)o + 0xe0) = 0x8c;
    *(short *)((char *)o + 0xb2) = 0;
    o->b04 = 1;
    if (D_8009C960 == 0x2000e) {
        o->step = 0x3e;
        o->state = 0;
            o->substep = 0;
    } else if ((D_8009C960 & 0x3ffff) == 0x3000a) {
        r = 0;
        if (D_8009D2C3 & 0x40) {
            if (o->y.p.whole >= -0x8b) {
                r = 1;
                o->w56 = -0x8c;
                if (o->y.p.whole >= -0x7b) {
                    if (D_8009CE3D == 0xff) {
                        r = 2;
                    } else {
                        o->y.p.whole = -0x8c;
                    }
                }
            }
        } else {
            if (o->y.p.whole >= -0x3bf) {
                r = 1;
                o->w56 = -0x3c0;
                if (o->y.p.whole >= -0x3af) {
                    if (D_8009CE3D == 0xff) {
                        r = 2;
                    } else {
                        o->y.p.whole = -0x3c0;
                    }
                }
            }
        }
        switch (r) {
        case 0:
            o->b9c = 2;
            *(unsigned char *)&o->wac = 1;
            o->step = 2;
            o->state = 3;
            o->substep = 0;
            break;
        case 1:
            o->step = 0x3d;
            o->state = 0;
            o->substep = 0;
            break;
        case 2:
            o->step = 0x3e;
            o->state = 0;
            o->substep = 0;
            break;
        default:
            return 1;
        }
    } else {
        o->b9c = 2;
        *(unsigned char *)&o->wac = 1;
        o->step = 2;
        o->state = 3;
        o->substep = 0;
    }
    return 1;
}
