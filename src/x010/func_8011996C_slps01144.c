// VERSION SLPS_01144 x10 8011996C 360
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
extern unsigned char D_8009EC20;
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void func_80119788_slps01144(TObj *);
extern void ObjFreeDup(TObj *);

void func_8011996C_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009EC20 != 0) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->b69 = 0;
        o->b6b = 0;
        o->w98 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d34 = o->y.p.whole;
        o->d8c = o->animFrame << 5;
        if (o->subtype == 0) {
            o->active = 2;
        } else if (o->subtype == 1) {
            o->box0 = 0x1e;
            o->box1 = 0x26;
            o->box2 = 4;
            o->box3 = 8;
        } else {
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 4;
            o->box3 = 8;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->b69 == 1) {
            o->w98 = 1;
            o->b6b = 1;
        }
        o->b69 = 0;
        if (o->w98 != 0)
            func_80119788_slps01144(o);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
