// VERSION SLPS_01144 x14 8011631C 356
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern void *D_80124DEC;
extern unsigned char D_8009E51D;
extern unsigned char D_8009E4E4;
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void func_8004CF38(int);
extern void ObjFree(TObj *);

void func_8011631C_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 16;
        o->b0d = 0;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->anim = D_80124DEC;
        AnimLoadDuration(o);
        break;
    case 1:
        if (ObjCullRegister(o)) {
            AnimAdvance(o);
        }
        if (o->subtype == 1) {
            switch (o->step) {
            case 0:
                if (D_8009E51D == 0) {
                    D_8009E4E4 = 2;
                    *(short *)&o->animFrame = 0x60;
                    func_8004CF38(0x60);
                    o->step++;
                }
                break;
            case 1:
                func_8004CF38(*(short *)&o->animFrame);
                if (D_8009E51D == 3) {
                    o->step++;
                }
                break;
            case 2:
                break;
            }
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
