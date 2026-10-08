// FUNC 8011762c 152 X005
// MATCHING 8011762c 152
#include "TOBJ.H"
extern void func_8011724C(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8011762C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8011724C(o);
        o->b04++;
        break;
    case 1:
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
