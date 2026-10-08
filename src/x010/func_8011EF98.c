// FUNC 8011ef98 196 X010
// MATCHING 8011ef98 196
#include "TOBJ.H"
extern unsigned char D_8009D2C3;
extern int func_8011E990(TObj *, TObj *, int, int);

int func_8011EF98(TObj *a, TObj *b)
{
    if (!(D_8009D2C3 & 0x40)) return 0;
    switch (b->subtype) {
    case 0:
        return func_8011E990(a, b, 0, b->d30);
    case 1:
    case 2:
        if (func_8011E990(a, b, 1, b->d30)) return 1;
        return func_8011E990(a, b, 2, (b->d30 + 0x400) & 0xfff);
    default:
        return func_8011E990(a, b, 2, b->d30);
    }
}
