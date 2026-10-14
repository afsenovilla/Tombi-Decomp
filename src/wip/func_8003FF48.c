// FUNC 8003ff48 816 MAIN0
#include "spad.h"
/* score 9 (was 99): x and xi are macros on (x_ & 7) (loop-hoisted after the (short)y extension, as in the game), D88/D90 from
   (short)s, priority bumps asm("" : "=r"(v) : "0"(v)) on o/a/d and on lim in case 1, asm("") barrier in case 2,
   compare result f computed before the D80 store (store lands after slt), hit-block stores reordered.
   Left: case 2 stores D90 after the lim arithmetic in the game (ours stores it right after D8C; dropping the case-2
   asm("") or rewriting with c8/c9 locals gives 25-51). Older notes (score 99): */
/* (b57: o/k/xi/x pinned with register asm (o=t1 k=t2 xi=t3 x=a1); the game does the arithmetic on the t3 copy
   and stores D_8009BD80 from a1. Left: param copy order (move t1,a0 after move t5,a2), D84/D88/D90 block regs.
   Short ss copy of D84 still cascades (144); lim pinned to v1 101; int y param pinned 282).
   Older: score 100 (b32: x &= 7; int xi = x; stores D_8009BD80 = xi gives the game andi a1 + move copy). Left: global-alloc order o/k/xi (game o=t1,k=t2,xcopy=t3; ours k=t1,xi=t2,o=t3: -dl shows o 7 refs/128 insns has lowest priority, needs more o refs or fewer k refs). Was 104: control flow and layout match; left: register allocation (game keeps x in a1 plus a t3 copy, raw y in t5, d without copy), frame setup order (game addiu sp before the first lw), D88 computed as (short)s>>8 in game (casts there raise the score). Tried: type brute force of x/y/s/d/lim/a/b, local x copy, inline body wrapper. b46: o needs ~13 refs to outrank k; duplicating the hit block in the else branch is not cross-jumped here (860 B, 257).
   b51: game keeps (D84 << 16) in a2 = a short copy `ss = D_8009BD84` used for D88 = ss >> 8 and D90 = (ss >> 4) & 0xf
   (sra 0x18 / sra 0x14), D8C from the raw lhu (& 0xf); D88 compared through a copy (a0); D6C store after D88 store;
   d (a3) has no copy. Writing it that way gives the right shape locally but scores 112-137 (alloc cascade). */
#include "TOBJ.H"

extern unsigned short *D_1F800278;
extern unsigned short D_1F800280, D_1F800282, D_1F800284;
extern unsigned short D_8009BD68;
extern short D_8009BD6C, D_8009BD70, D_8009BD74, D_8009BD78, D_8009BD7C, D_8009BD80;
extern unsigned short D_8009BD84;
extern short D_8009BD88, D_8009BD8C, D_8009BD90;
#define D D_1F800278

int func_8003FF48(TObj *o_, unsigned short x_, short y)
{
#define x (x_ & 7)
#define xi ((short)(x_ & 7))
    register TObj *o asm("$9") = o_;
    int k;
    short d;
    int lim;
    int s;
    short ss;
    int f;
    unsigned short a;
    short b;
    unsigned short r;

    D_8009BD70 = *D++;
    if (D_8009BD70 == 0)
        return 0;
asm("" : "=r"(o) : "0"(o));
    k = 0;
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
asm("" : "=r"(a) : "0"(a));
        D_8009BD8C = D_8009BD84 & 0xf;
        D_8009BD90 = (D_8009BD84 >> 4) & 0xf;
        if (D_8009BD90 == 0) {
            if (b > 0) {
            plain:
                D_8009BD68 = a;
            } else
                D_8009BD68 = a + b;
        } else {
            r = b * (xi - D_8009BD8C) / D_8009BD90;
            D_8009BD80 = x;
            D_8009BD68 = a + r;
        }
        d = D_8009BD68 - y;
asm("" : "=r"(d) : "0"(d));
        D_8009BD6C = d;
        s = D_8009BD84;
        D_8009BD88 = (short)s >> 8;
        if (D_8009BD88 == 1) {
            lim = s & 0xf;
            D_8009BD8C = lim;
            lim = xi - lim;
            asm("" : "=r"(lim) : "0"(lim));
            lim += 0x10;
            goto common;
        } else if (D_8009BD88 == 2) {
            D_8009BD8C = s & 0xf;
            D_8009BD90 = ((short)s >> 4) & 0xf;
asm("");
            lim = D_8009BD8C + D_8009BD90 - xi + 0x10;
        common:
            f = (-(short)d < lim);
            asm("");
            D_8009BD80 = x;
            if (f && (short)d <= 0) {
            hit:
                o->y.p.frac = 0;
                o->y.p.whole += d;
                (*(unsigned short*)&SPAD.s27e) = D_8009BD78;
                D_1F800280 = D_8009BD74;
                D_1F800282 = D_8009BD7C;
                D_1F800284 = (D_8009BD7C & 0xe00) >> 9;
                return 1;
            }
        } else if ((short)d <= 0)
            goto hit;
    } while (D_8009BD70 != 0);
    return 0;
}
