// FUNC 8012a8b8 344 X004
// MATCHING 8012a8b8 344
#include "TOBJ.H"

extern unsigned char D_8009CF2D;
extern short D_1F800246;
extern int AnimAdvance(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void func_8012A31C(TObj *o);
extern void func_8012A740(TObj *o);
extern void FUN_80018790(TObj *o);

void func_8012A8B8(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012A740(o);
        break;
    case 1:
        ObjCullRegister(o);
        AnimAdvance(o);
        switch (o->step) {
        case 0:
            if (D_8009CF2D != 0) break;
            if (o->b68 == 0) break;
            if (D_1F800246 < 2) {
                o->step++;
                o->state = 0;
            } else {
                o->b68 = 0;
            }
            break;
        case 1:
            func_8012A31C(o);
            break;
        case 3:
            AnimAdvance(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
