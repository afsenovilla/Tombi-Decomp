// FUNC 8011610c 328 X006
// MATCHING 8011610c 328
#include "TOBJ.H"
extern volatile int D_1F800190;
extern int D_1F8000F0;

void func_8011610C(TObj *o)
{
    unsigned char flag = 0;
    int k, c, v;
    int d = D_1F800190 - 0x800000;
    d -= D_1F8000F0;
    k = 0x500000;
    d += k;
    c = 0x340000;

    if (d < 0) {
        d += c;
        flag = 1;
        if (d >= 0) {
            o->b.raw = 0;
            return;
        }
    }
    if (flag == 0) {
        if (d > 0x10000) {
            if (d < o->b.raw) {
                if (d > 0x3ffff) { v = 0x40000; goto st; }
                o->b.raw = d;
            } else {
                if (o->b.raw < 0) o->b.raw = 0;
                d = 0x8000;
                v = o->b.raw + d;
                goto st;
            }
        } else {
            D_1F8000F0 = D_1F800190 + k - 0x800000;
            return;
        }
    } else {
        if (d < -0x40000) {
            if (o->b.raw < d) {
                if (d > -0x40000) o->b.raw = d;
                else o->b.raw = -0x40000;
            } else {
                if (o->b.raw > 0) o->b.raw = 0;
                v = o->b.raw - 0x4000;
            st:
                o->b.raw = v;
            }
        } else {
            o->b.raw = d;
        }
    }
    *(volatile int *)&D_1F8000F0 += o->b.raw;
}
