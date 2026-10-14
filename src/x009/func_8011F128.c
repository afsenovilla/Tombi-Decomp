// FUNC 8011f128 280 X009
// MATCHING 8011f128 280
#include "gcd.h"
#include "TOBJ.H"

extern unsigned char D_8009D119;
extern unsigned char D_8007A7F0[];
extern void (*D_8007A890[])(TObj *);
extern void initItemObject(TObj *);
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void freeObjectLayer5(TObj *);

void func_8011F128(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D119 != 0 || GCD.bcf06 != 0) {
            o->b04 = 3;
            break;
        }
        initItemObject(o);
        o->active = 2;
        o->b04++;
        break;
    case 1:
        if (ObjCullRegister(o))
            AnimAdvance(o);
        break;
    case 2:
        D_8007A890[D_8007A7F0[o->subtype]](o);
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
