// FUNC 8011af84 48 X006
// MATCHING 8011af84 48
#include "spad.h"
#include "TOBJ.H"

void setModelPtrFromWac(TObj *o)
{
    int *pp = &SPAD.i334;
    int i;
    char *b;

    b = (char *)*pp;
    i = o->wac << 2;
    b += i;
    i = *(int *volatile *)pp;
    i += ((int *)b)[1];
    o->da0 = i;
}
