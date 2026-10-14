// FUNC 80119498 740 X011
// MATCHING 80119498 740
#include "gcd.h"
#include "TOBJ.H"

extern int AnimAdvance(TObj *o);
extern void func_8011708C(TObj *o);
extern void func_80117560(TObj *o);
extern void func_801177DC(TObj *o);
extern void func_80117C34(TObj *o);
extern void func_80117EF8(TObj *o);
extern void func_80118140(TObj *o);
extern void func_80118328(TObj *o);
extern void func_801185EC(TObj *o);
extern void func_801187D4(TObj *o);
extern void func_801189F8(TObj *o);
extern void func_80118BE0(TObj *o);
extern void func_80118DC8(TObj *o);
extern void func_80119034(TObj *o);
extern void func_8011921C(TObj *o);

void func_80119498(TObj *o)
{
    switch (o->subtype) {
    case 0:
        switch (o->step) {
        case 0:
            if (GCD.bcdf1 != 0xff || GCD.bce0c == 0xff) {
                o->b04 = 2;
                break;
            }
            if (GCD.bce0c) o->step = 2;
            else o->step = 1;
            o->state = 0;
            break;
        case 1:
            func_8011708C(o);
            break;
        case 2:
            func_80117560(o);
            break;
        case 3:
            func_801177DC(o);
            break;
        }
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (GCD.bcde5) o->step = 2;
            else o->step = 1;
            o->state = 0;
            break;
        case 1:
            func_80117C34(o);
            break;
        case 2:
            func_80117EF8(o);
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            switch (GCD.bce0a) {
            case 0:
                o->step = 1;
                break;
            case 0xff:
                if (GCD.bcdf1 != 0xff) o->step = 6;
                else if (GCD.bcdf3) o->step = 8;
                else o->step = 7;
                break;
            default:
                switch (GCD.bce0d) {
                case 0:
                    o->step = 2;
                    break;
                case 0xff:
                    o->step = 4;
                    break;
                default:
                    o->step = 3;
                    break;
                }
                break;
            }
            o->state = 0;
            break;
        case 1: func_80118140(o); break;
        case 2: func_80118328(o); break;
        case 3: func_801185EC(o); break;
        case 4: func_801187D4(o); break;
        case 5: func_801189F8(o); break;
        case 6: func_80118BE0(o); break;
        case 7: func_80118DC8(o); break;
        case 8: func_80119034(o); break;
        }
        break;
    case 3:
        func_8011921C(o);
        break;
    }
    AnimAdvance(o);
}
