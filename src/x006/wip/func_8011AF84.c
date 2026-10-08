/* score 10: only the first addu differs: game `addu v0,v0,v1` (base + idx<<2, base in v0); here idx<<2 + base.
   Every form that puts base first (int arithmetic, (int)b + (i<<2)) flips local-alloc and swaps v0/v1 everywhere.
   Tried: ~300 variants (int/short idx temps, pointer vs int base, operand orders, inline helper, two array externs). */
// FUNC 8011af84 48 X006
#include "TOBJ.H"
extern int D_1F800334;

void func_8011AF84(TObj *o)
{
    int *b = (int *)*(volatile int *)&D_1F800334;
    int t = *(volatile int *)&D_1F800334;
    o->da0 = t + (b + o->wac)[1];
}
