// VERSION SLPS_01144 x01 8011FE6C 400
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

#define S176 (*(short *)0x1F800176)
extern int D_8013BDC8[];
extern void func_8011FD44_slps01144(TObj *);
extern void func_80017F08(TObj *);

void func_8011FE6C_slps01144(TObj *o)
{
    unsigned short t;
    int x;

    switch (o->b04) {
    case 0:
        o->w1e = 10;
        o->b0d = 0;
        o->anim = (void *)D_8013BDC8[o->b0c];
        o->d3c = SPAD.i2d4;
        o->b0f = 0;
        if (o->subtype == 0) {
            x = o->a.raw;
            o->d30 = x;
            *(int *)((char *)o + 0x5c) = x;
        } else {
            o->d30 = o->a.p.whole;
        }
        t = o->b.p.whole;
        o->d34 = o->y.p.whole;
        o->b.p.whole = 0;
        o->animFrame = t;
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (S176 >= 1069) o->step++;
            func_8011FD44_slps01144(o);
            break;
        case 1:
            if (S176 < 1068) {
                if (o->subtype == 0) o->d30 = *(int *)((char *)o + 0x5c) + 0x1400000;
                o->step--;
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80017F08(o);
        break;
    }
}
#endif
