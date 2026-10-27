// FUNC 8003ff48 816 MAIN0
// MATCHING 8003ff48 816
#include "spad.h"
/* matched: case 2 builds lim on the s & 0xf temp (as case 1) and stores D90 last; s priority bump in case 1. */
#include "TOBJ.H"

extern unsigned short *COLLISION_SEG_PTR;
extern unsigned short D_1F800280, COLLISION_HIT_FLAGS, D_1F800284;
extern unsigned short TILE_COL_SURFACE_Y;
extern short D_8009BD6C, TILE_COL_COUNT, TILE_COL_TOP, TILE_COL_HEIGHT, TILE_COL_FLAGS, D_8009BD80;
extern unsigned short D_8009BD84;
extern short D_8009BD88, D_8009BD8C, D_8009BD90;
#define D COLLISION_SEG_PTR

int tileCollideColumnAt(TObj *o_, unsigned short x_, short y)
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

    TILE_COL_COUNT = *D++;
    if (TILE_COL_COUNT == 0)
        return 0;
asm("" : "=r"(o) : "0"(o));
    k = 0;
    do {
        TILE_COL_COUNT--;
        TILE_COL_FLAGS = *D++;
        if (TILE_COL_FLAGS & 0x10) {
            D += 3;
            continue;
        }
        if (!(TILE_COL_FLAGS & 1)) {
            D += 3;
            k++;
            continue;
        }
        TILE_COL_TOP = *D++;
        TILE_COL_HEIGHT = *D++;
        D_8009BD84 = *D++;
        if (k != 0) {
            if (TILE_COL_HEIGHT > 0) {
                if (TILE_COL_TOP + TILE_COL_HEIGHT + 0x20 < y)
                    return 0;
            } else {
                if (TILE_COL_TOP + 0x20 < y)
                    return 0;
            }
        }
        k++;
        a = TILE_COL_TOP;
        b = TILE_COL_HEIGHT;
        if (b == 0)
            goto plain;
asm("" : "=r"(a) : "0"(a));
        D_8009BD8C = D_8009BD84 & 0xf;
        D_8009BD90 = (D_8009BD84 >> 4) & 0xf;
        if (D_8009BD90 == 0) {
            if (b > 0) {
            plain:
                TILE_COL_SURFACE_Y = a;
            } else
                TILE_COL_SURFACE_Y = a + b;
        } else {
            r = b * (xi - D_8009BD8C) / D_8009BD90;
            D_8009BD80 = x;
            TILE_COL_SURFACE_Y = a + r;
        }
        d = TILE_COL_SURFACE_Y - y;
asm("" : "=r"(d) : "0"(d));
        D_8009BD6C = d;
        s = D_8009BD84;
        D_8009BD88 = (short)s >> 8;
        if (D_8009BD88 == 1) {
asm("" : "=r"(s) : "0"(s));
            lim = s & 0xf;
            D_8009BD8C = lim;
            lim = xi - lim;
            asm("" : "=r"(lim) : "0"(lim));
            lim += 0x10;
            goto common;
        } else if (D_8009BD88 == 2) {
            lim = s & 0xf;
            f = ((short)s >> 4) & 0xf;
            D_8009BD8C = lim;
            lim = lim + f;
            lim = lim - xi;
            lim += 0x10;
            D_8009BD90 = f;
        common:
            f = (-(short)d < lim);
            asm("");
            D_8009BD80 = x;
            if (f && (short)d <= 0) {
            hit:
                o->y.p.frac = 0;
                o->y.p.whole += d;
                (*(unsigned short*)&SPAD.s27e) = TILE_COL_HEIGHT;
                D_1F800280 = TILE_COL_TOP;
                COLLISION_HIT_FLAGS = TILE_COL_FLAGS;
                D_1F800284 = (TILE_COL_FLAGS & 0xe00) >> 9;
                return 1;
            }
        } else if ((short)d <= 0)
            goto hit;
    } while (TILE_COL_COUNT != 0);
    return 0;
}
