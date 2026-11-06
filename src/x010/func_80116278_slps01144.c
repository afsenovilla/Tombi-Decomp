// VERSION SLPS_01144 x10 80116278 480
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

extern unsigned char D_8009EE6B, D_8009E4E0;
extern void playSFX(int);
extern void ObjListPush_1F800230(TObj *);
extern void func_80116458_slps01144(TObj *);
extern void freeObjectLayer8(TObj *);
#define S176 (*(short *)0x1F800176)
#define S1F8 (*(unsigned short *)0x1F8001F8)

void func_80116278_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        switch (o->subtype) {
        case 0:
        case 1:
        case 5:
            break;
        case 2:
            if (D_8009EE6B & 0x40) o->b04 = 3;
            break;
        case 3:
        case 4:
            if (!(D_8009EE6B & 0x40)) o->b04 = 3;
            break;
        }
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800230(o);
        if (D_8009E4E0) {
            o->b04 = 2;
            o->step = 0;
            break;
        }
        switch (o->subtype) {
        case 0:
            if (S176 < 2761) break;
            func_80116458_slps01144(o);
            break;
        case 1:
            if ((S1F8 & 0x1f) == 0) playSFX(0xaf);
        case 2:
        case 5:
            func_80116458_slps01144(o);
            break;
        case 3:
        case 4:
            break;
        }
        break;
    case 2:
        o->visible = 1;
        ObjListPush_1F800230(o);
        if (o->step) break;
        if (o->w22 & 1) {
            o->timer = 1;
            func_80116458_slps01144(o);
        }
        o->step++;
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
#endif
