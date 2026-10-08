// FUNC 80129a9c 200 X004
// MATCHING 80129a9c 200
#include "TOBJ.H"
extern void func_80129914(TObj *);
extern void func_80129650(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_80129A9C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80129914(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->step == 0) goto run;
        if (o->step == 1) {
        run:
            func_80129650(o);
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
