// FUNC 8012c858 292 X003
// MATCHING 8012c858 292
#include "TOBJ.H"
extern void func_8012C68C(TObj *);
extern void func_8012C00C(TObj *);
extern void ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_80018790(TObj *);

void func_8012C858(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012C68C(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->b0c == 0x29) {
            switch (o->step) {
            case 0:
                if (o->b68) {
                    o->step++;
                }
                break;
            case 1:
                func_8012C00C(o);
                break;
            }
        }
        if (o->anim && o->active) {
            AnimAdvance(o);
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
