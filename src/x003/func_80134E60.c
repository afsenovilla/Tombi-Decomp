// FUNC 80134e60 260 X003
// MATCHING 80134e60 260
#include "TOBJ.H"

extern unsigned char D_8009CDAB;
extern unsigned char D_8009CFCD;
extern void func_80134CF8(TObj *);
extern void func_801346C8(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_80134E60(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80134CF8(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (D_8009CDAB == 1 && D_8009CFCD == 0) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            func_801346C8(o);
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
