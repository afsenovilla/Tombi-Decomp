/* score 30: constants now match (k = 0x500000 kept in a register and reused by the DF0 reset, -0x800000 reloaded) and the +0x8000 path jumps into the -0x4000 store via goto. Left: game keeps `move a2,zero` (flag = 0) as the 2nd insn instead of the bgez delay slot; jump opt merges the `store:` label with the b = 0x40000 tail (game keeps them apart); regs of the +0x8000 add swapped. Tried flag types, statement forms of both adds. */
// FUNC 8011610c 328 X006
#include "TOBJ.H"
extern int D_1F800190;
extern int D_1F8000F0;

void func_8011610C(TObj *o)
{
    unsigned char flag = 0;
    int k;
    int v;
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
                v = o->b.raw + 0x8000;
                goto store;
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
            store:
                o->b.raw = v;
            }
        } else {
            o->b.raw = d;
        }
    }
    *(volatile int *)&D_1F8000F0 += o->b.raw;
}
