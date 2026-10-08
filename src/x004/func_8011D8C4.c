// FUNC 8011d8c4 224 X004
// MATCHING 8011d8c4 224
#include "TOBJ.H"

extern int D_8009F0EC;
extern unsigned short D_8009C962, D_8009C982;
extern unsigned char D_8009CDD6[], D_8009CDD5, D_8009CF00, D_8009CF04;
extern void func_8012B388(TObj *);

void func_8011D8C4(TObj *o)
{
    D_8009F0EC = 0;
    if (D_8009C962 == 2 && D_8009C982 == 2) {
        o->b04 = 5;
        o->step = 9;
    }
    if (D_8009CDD6[0] == 1) func_8012B388(o);
    if (D_8009CDD6[0] == 0xff && D_8009CDD5 == 1 && D_8009CF00 && D_8009CF04) func_8012B388(o);
}
