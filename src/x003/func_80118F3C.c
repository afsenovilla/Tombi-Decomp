// FUNC 80118f3c 464 X003
// MATCHING 80118f3c 464
#include "TOBJ.H"

extern unsigned char D_8009D2C3;
extern int D_1F8002D4[];
extern void *D_80138EB0[];
extern void func_80118DF8(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void func_80118F3C(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D2C3 & 2) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->w1e = 8;
        o->b0d = 0x80;
        o->d3c = D_1F8002D4[0];
        o->anim = D_80138EB0[o->b0c];
        o->d64 = 0x1400;
        break;
    case 1:
        switch (o->step) {
        case 0:
            o->step++;
            func_80118DF8(o);
            break;
        case 1:
            switch (o->subtype) {
            case 0:
            case 2:
                o->a.raw += 0x180000;
                break;
            case 1:
            case 4:
                o->a.raw += 0x160000;
                o->y.raw += -0x80000;
                break;
            case 3:
                o->a.raw += 0x160000;
                o->y.raw += 0x60000;
                break;
            case 5:
            case 6:
                o->a.raw += 0xf0000;
                break;
            case 7:
                o->a.raw += 0xa0000;
                break;
            }
            if (!ObjCullRegister(o)) o->step = 0;
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
