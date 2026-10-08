// FUNC 8003ff48 816 MAIN0
/* score 104: control flow and layout match; left: register allocation (game keeps x in a1 plus a t3 copy, raw y in t5, d without copy), frame setup order (game addiu sp before the first lw), D88 computed as (short)s>>8 in game (casts there raise the score). Tried: type brute force of x/y/s/d/lim/a/b, local x copy, inline body wrapper. */
#include "TOBJ.H"

extern unsigned short *D_1F800278;
extern unsigned short D_1F80027E, D_1F800280, D_1F800282, D_1F800284;
extern unsigned short D_8009BD68;
extern short D_8009BD6C, D_8009BD70, D_8009BD74, D_8009BD78, D_8009BD7C, D_8009BD80;
extern unsigned short D_8009BD84;
extern short D_8009BD88, D_8009BD8C, D_8009BD90;
#define D D_1F800278

int func_8003FF48(TObj *o, unsigned short x, short y)
{
    int k;
    short d;
    int lim;
    int s;
    unsigned short a;
    short b;
    int r;

    D_8009BD70 = *D++;
    if (D_8009BD70 == 0)
        return 0;
    k = 0;
    x &= 7;
    do {
        D_8009BD70--;
        D_8009BD7C = *D++;
        if (D_8009BD7C & 0x10) {
            D += 3;
            continue;
        }
        if (!(D_8009BD7C & 1)) {
            D += 3;
            k++;
            continue;
        }
        D_8009BD74 = *D++;
        D_8009BD78 = *D++;
        D_8009BD84 = *D++;
        if (k != 0) {
            if (D_8009BD78 > 0) {
                if (D_8009BD74 + D_8009BD78 + 0x20 < y)
                    return 0;
            } else {
                if (D_8009BD74 + 0x20 < y)
                    return 0;
            }
        }
        k++;
        a = D_8009BD74;
        b = D_8009BD78;
        if (b == 0)
            goto plain;
        D_8009BD8C = D_8009BD84 & 0xf;
        D_8009BD90 = (D_8009BD84 >> 4) & 0xf;
        if (D_8009BD90 == 0) {
            if (b > 0) {
            plain:
                D_8009BD68 = a;
            } else
                D_8009BD68 = a + b;
        } else {
            r = b * (x - D_8009BD8C) / D_8009BD90;
            D_8009BD80 = x;
            D_8009BD68 = a + r;
        }
        d = D_8009BD68 - y;
        D_8009BD6C = d;
        s = D_8009BD84;
        D_8009BD88 = s >> 8;
        if (D_8009BD88 == 1) {
            D_8009BD8C = s & 0xf;
            lim = x - D_8009BD8C + 0x10;
            goto common;
        } else if (D_8009BD88 == 2) {
            D_8009BD8C = s & 0xf;
            D_8009BD90 = (s >> 4) & 0xf;
            lim = D_8009BD8C + D_8009BD90 - x + 0x10;
        common:
            D_8009BD80 = x;
            if (-(short)d < lim && (short)d <= 0) {
            hit:
                o->y.p.frac = 0;
                o->y.p.whole += d;
                D_1F800282 = D_8009BD7C;
                D_1F80027E = D_8009BD78;
                D_1F800280 = D_8009BD74;
                D_1F800284 = (D_8009BD7C & 0xe00) >> 9;
                return 1;
            }
        } else if ((short)d <= 0)
            goto hit;
    } while (D_8009BD70 != 0);
    return 0;
}
