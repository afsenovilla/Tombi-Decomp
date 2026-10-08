// FUNC 8011f518 156 X010
// MATCHING 8011f518 156
#include "TOBJ.H"

extern int func_8011F05C(TObj *, TObj *, int, int);

void func_8011F518(TObj *a, TObj *b)
{
    switch (b->subtype) {
    case 0:
        func_8011F05C(a, b, 0, b->d30);
        break;
    case 1:
    case 2:
        if (!func_8011F05C(a, b, b->subtype, b->d30))
            func_8011F05C(a, b, 2, (b->d30 + 0x400) & 0xfff);
        break;
    default:
        func_8011F05C(a, b, 2, b->d30);
        break;
    }
}
