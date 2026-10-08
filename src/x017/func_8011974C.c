// FUNC 8011974c 192 X017
// MATCHING 8011974c 192
#include "TOBJ.H"
extern unsigned char D_8009CDBC;
extern void func_80118C6C(TObj *);
extern void func_80118EA0(TObj *);
extern void FUN_80018980(TObj *);

void func_8011974C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80118C6C(o);
        o->b04++;
        if (D_8009CDBC == 0xff) o->b04 = 3;
        break;
    case 1:
        func_80118EA0(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
