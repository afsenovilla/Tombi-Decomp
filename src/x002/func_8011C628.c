// FUNC 8011c628 168 X002
// MATCHING 8011c628 168
#include "TOBJ.H"
extern void func_80119A1C(TObj *);
extern void func_8011C4EC(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8011C628(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80119A1C(o);
        o->b04++;
        break;
    case 1:
        func_8011C4EC(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
