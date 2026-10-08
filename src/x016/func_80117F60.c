// FUNC 80117f60 276 X016
// MATCHING 80117f60 276
#include "TOBJ.H"

extern Fix16 *D_800A6078;
extern void func_80117E04(TObj *);
extern void func_80117B40(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_80117F60(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80117E04(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (o->h->p.whole < D_800A6078->p.whole) {
                D_800A6078->p.whole = o->h->p.whole;
            }
            if (o->b68 != 0) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            func_80117B40(o);
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
