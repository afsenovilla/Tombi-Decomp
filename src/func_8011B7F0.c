// FUNC 8011b7f0 208 X000
// MATCHING 8011b7f0 208
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"
extern void *D_8013B168[];
extern void ObjCullRegister(TObj *o);
extern void ObjFree(TObj *o);

void func_8011B7F0(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        if (GCD.bcdac != 0xff) {
            o->b04 = 3;
        } else {
            o->b04 = s + 1;
            o->w1e = 8;
            o->d3c = ((int*)&SPAD.i2d4)[0];
            o->b0d = 0;
            o->anim = D_8013B168[o->b0c];
        }
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
