// FUNC 8011af84 48 X006
// MATCHING 8011af84 48
#include "TOBJ.H"
extern int D_1F800334;

void func_8011AF84(TObj *o)
{
    int *pp = &D_1F800334;
    int i;
    char *b;

    b = (char *)*pp;
    i = o->wac << 2;
    b += i;
    i = *(int *volatile *)pp;
    i += ((int *)b)[1];
    o->da0 = i;
}
