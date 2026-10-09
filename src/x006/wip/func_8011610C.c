// FUNC 8011610c 328 X006
/* score 16: volatile D_1F800190 keeps flag = 0 (move a2,zero) at the top (reorg cannot hoist it past the volatile load) and the start matches.
   Left: game tests the flag with andi 0xff (uchar flag), but any uchar/& 0xff form makes reorg steal that andi from the bgez target
   instead of the lui 0x340000 fallthrough (score 50); the +0x8000 path should jump into the -0x4000 store (goto form of old wip, 41 with int flag). */
#include "TOBJ.H"
extern volatile int D_1F800190;
extern int D_1F8000F0;

void func_8011610C(TObj *o)
{
    int flag = 0;
    int k;
    int d = D_1F800190 - 0x800000;
    d -= D_1F8000F0;
    k = 0x500000;
    d += k;

    if (d < 0) {
        d += 0x340000;
        flag = 1;
        if (d >= 0) {
            o->b.raw = 0;
            return;
        }
    }
    if (flag == 0) {
        if (d > 0x10000) {
            if (d < o->b.raw) {
                if (d > 0x3ffff) o->b.raw = 0x40000;
                else o->b.raw = d;
            } else {
                if (o->b.raw < 0) o->b.raw = 0;
                o->b.raw = o->b.raw + 0x8000;
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
                o->b.raw = o->b.raw - 0x4000;
            }
        } else {
            o->b.raw = d;
        }
    }
    *(volatile int *)&D_1F8000F0 += o->b.raw;
}
