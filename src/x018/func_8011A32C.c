// FUNC 8011a32c 372 X018
// MATCHING 8011a32c 372
#include "TOBJ.H"

extern void func_8011A1B4(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);
extern void func_8011A038(TObj *);
extern void func_801177D0(TObj *);
extern void func_80117C38(TObj *);
extern void func_80117FAC(TObj *);
extern void func_801182FC(TObj *);
extern void func_8011858C(TObj *);
extern void func_80118C84(TObj *);
extern void func_80118F14(TObj *);
extern void func_801191DC(TObj *);
extern void func_801194D4(TObj *);
extern void func_8011973C(TObj *);

void func_8011A32C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8011A1B4(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_8011A038(o);
            break;
        case 1:
            func_801177D0(o);
            break;
        case 2:
            func_80117C38(o);
            break;
        case 3:
            func_80117FAC(o);
            break;
        case 4:
            func_801182FC(o);
            break;
        case 5:
            func_8011858C(o);
            break;
        case 6:
            func_80118C84(o);
            break;
        case 7:
            func_80118F14(o);
            break;
        case 8:
            func_801191DC(o);
            break;
        case 9:
            func_801194D4(o);
            break;
        case 10:
            func_8011973C(o);
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
