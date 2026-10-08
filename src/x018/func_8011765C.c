// FUNC 8011765c 240 X018
// MATCHING 8011765c 240
#include "TOBJ.H"

extern unsigned char D_8009CE16, D_8009CE18, D_8009CFD9, D_8009CFDA;
extern void func_80117484(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8011765C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80117484(o);
        if (D_8009CE16 == 0xff) D_8009CFD9 = 1;
        else D_8009CFD9 = 0;
        if (D_8009CE18 == 0xff) D_8009CFDA = 1;
        else D_8009CFDA = 0;
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
