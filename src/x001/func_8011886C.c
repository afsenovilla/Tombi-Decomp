// FUNC 8011886c 300 X001
// MATCHING 8011886c 300
#include "TOBJ.H"

extern short D_8007A3F0[];
extern short D_8007A5F0[];
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern int ObjCullRegister(TObj *);

void func_8011886C(TObj *o)
{
    int n;
    int amp;

    n = o->d88;
    amp = D_8007A5F0[n];
    o->d88 = n + 1;
    o->h->raw += (D_8007A5F0[o->d84] * ((short)amp >> 3)) >> 4;
    o->y.raw += (D_8007A3F0[o->d84] * ((short)amp >> 3)) >> 4;
    if (o->timer < 0x1e) {
        if ((D_1F8001F8 + D_1F800198) & 1) {
            ObjCullRegister(o);
        }
    } else if (ObjCullRegister(o) == 0) {
        o->b04 = 3;
    }
    if (--o->timer == -1) {
        o->b04 = 3;
    }
}
