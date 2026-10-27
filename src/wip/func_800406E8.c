// FUNC 800406e8 496 MAIN0
#include "spad.h"
/* score 98 with maspsx --expand-div (ncheck lacks it: game div has break 7/6 checks). Left: prologue (P load before addiu sp),
   game copies a2->t6, a1->v1 at entry and uses raw y in d = h - y */
typedef struct S { char p[0x14]; short w14; unsigned short w16; } S;
extern unsigned short *COLLISION_SEG_PTR;
extern short COLLISION_HIT_HEIGHT;
extern short D_1F800280;
int func_800406E8(S *o, short x, short y)
{
    char pad;
    short n;
    unsigned short fl, w;
    int a;
    short b;
    int lo, len, h, d;
    n = *COLLISION_SEG_PTR++;
    if (n == 0)
        return 0;
    do {
        fl = *COLLISION_SEG_PTR++;
        n--;
        if (!(fl & 2)) goto skip;
        if (fl & 0x10) {
        skip:
            COLLISION_SEG_PTR += 3;
            continue;
        }
        a = *COLLISION_SEG_PTR++;
        b = *COLLISION_SEG_PTR++;
        w = *COLLISION_SEG_PTR++;
        lo = w & 0xf;
        len = (w >> 4) & 0xf;
        if (len == 0) continue;
        if ((short)y < (short)a - 0x10) continue;
        if ((x & 7) < lo) continue;
        if (lo + len < (x & 7)) continue;
        if (b == 0)
            h = a;
        else
            h = a + b * (((short)x - lo) % len) / len;
        d = h - y;
        if ((short)d > 0) {
            o->w14 = 0;
            COLLISION_HIT_HEIGHT = b;
            D_1F800280 = a;
            (*(short*)&SPAD.s282) = fl;
            o->w16 += d;
            return 1;
        }
    } while (n != 0);
    return 0;
}
