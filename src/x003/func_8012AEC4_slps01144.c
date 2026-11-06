// VERSION SLPS_01144 x03 8012AEC4 300
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern unsigned char D_8009EA05;
extern void func_8012AD48(TObj *);
extern void func_8012A66C(TObj *);
extern void ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void func_8012AEC4_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012AD48(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->b0c == 40) {
            switch (o->step) {
            case 0:
                if (o->b68 != 0) o->step++;
                if (D_8009EA05 == 255 && o->step == 0) {
                    o->step = 1;
                    o->state = 70;
                }
                break;
            case 1:
                func_8012A66C(o);
                break;
            }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
