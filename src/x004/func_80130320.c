// FUNC 80130320 260 X004
// MATCHING 80130320 260
#include "TOBJ.H"

extern unsigned char D_8009CDAB;
extern unsigned char D_8009CFCB;
extern void func_801301B0(TObj *);
extern void func_8012FBF4(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_80130320(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_801301B0(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (D_8009CDAB == 1 && D_8009CFCB == 0) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            func_8012FBF4(o);
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
