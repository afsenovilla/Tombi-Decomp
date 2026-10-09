// FUNC 8011610c 328 X006
/* score 22 (o39; was 16 with a different shape): `c = 0x340000` hoisted before the if + `unsigned char flag` fixes the
   whole top (reorg takes the lui from before the branch; the andi at the target can no longer be stolen because the
   fall-through needs v0). The +0x8000 path jumps into the -0x4000 store (`v = ...; goto st;` / `st: o->b.raw = v;`),
   which with `k` reused as the variable gives the game's exact control flow but k lands in a2 and 0x500000 is hoisted
   (score 22 too). With a separate `v`, jump2 cross-jumps the 0x40000 store (`sw v0; j L230`) into the st tail (320 B);
   the game keeps them apart. Tried: volatile st / 0x40000 stores, reusing c/d/flag, unsigned v, v split (+=). */
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
                if (d > 0x3ffff) o->b.raw = 0x40000;
                else o->b.raw = d;
            } else {
                if (o->b.raw < 0) o->b.raw = 0;
                v = o->b.raw + 0x8000;
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
