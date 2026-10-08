// FUNC 80129250 336 X004
// MATCHING 80129250 336
#include "TOBJ.H"
extern unsigned char D_8009CE22, D_8009C940, D_8009C941, D_8009C990, D_8009CE3D;
extern short D_800A604E;
extern void func_80128114(TObj *);
extern void func_80128304(TObj *);
extern void func_8012866C(TObj *);
extern void FUN_8001fec0(TObj *);

void func_80129250(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CE22 == 0) break;
        if (D_8009C940 != 0 && D_8009C941 == 0x94) {
            o->step = 2;
            o->state = 0;
        }
        if (D_800A604E >= -0xa8 && D_8009C990 != 1 && D_8009CE3D != 0 && D_8009CE3D != 0xff) {
            o->step = 3;
            o->state = 0;
        }
        if (o->b68 != 0) {
            o->animFrame = o->b68 & 1;
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_80128114(o);
        break;
    case 2:
        func_80128304(o);
        break;
    case 3:
        func_8012866C(o);
        break;
    }
    FUN_8001fec0(o);
}
