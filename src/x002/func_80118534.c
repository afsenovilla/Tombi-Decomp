// FUNC 80118534 336 X002
// MATCHING 80118534 336
#include "TOBJ.H"

extern void func_8011836C(TObj *);
extern void func_80117D70(TObj *);
extern void func_80118028(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_80118534(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8011836C(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (o->b68 == 0) break;
            switch (o->b0c) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                o->step = 2;
                o->state = 0;
                break;
            case 7:
                o->step = 1;
                o->state = 0;
                break;
            case 6:
            case 8:
                break;
            }
            break;
        case 1:
            func_80117D70(o);
            break;
        case 2:
            func_80118028(o);
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
