/* score 32: control flow and stores match; differs: the first block (game keeps `move a2,zero` at the top and loads -0x800000 into v0 before the two globals),
   the DF0 reset path (game reuses the 0x500000 register and adds -0x800000 separately, no constant folding) and one cross-jumped
   `b += 0x8000` tail. Tried: temps for the subtraction, inline with a flag param, do-while barrier, split statements. */
// FUNC 8011610c 328 X006
#include "TOBJ.H"
extern int D_1F800190;
extern int D_1F8000F0;

void func_8011610C(TObj *o)
{
    unsigned char flag = 0;
    int x = D_1F800190 - 0x800000;
    int d = x - D_1F8000F0 + 0x500000;

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
                o->b.raw += 0x8000;
            }
        } else {
            D_1F8000F0 = D_1F800190 + 0x500000 - 0x800000;
            return;
        }
    } else {
        if (d < -0x40000) {
            if (o->b.raw < d) {
                if (d > -0x40000) o->b.raw = d;
                else o->b.raw = -0x40000;
            } else {
                if (o->b.raw > 0) o->b.raw = 0;
                o->b.raw -= 0x4000;
            }
        } else {
            o->b.raw = d;
        }
    }
    *(volatile int *)&D_1F8000F0 += o->b.raw;
}
