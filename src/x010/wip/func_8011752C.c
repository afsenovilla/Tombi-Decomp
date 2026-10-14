// FUNC 8011752c 1020 X010
/* score 80 (was 92): loop 1 now shifts b out of place (d1 = b1 >> 24, game v1 -> a3) and writes the adds to fresh
   x1/y1/z1, all loop-1 locals pinned (a1=$4 b1=$3 c1=$5 d1=$7 x1=$3 y1=$8 z1=$9). Left: the game delays the LAST
   element's sra into the branch delay slot, ours delays the out-of-place one; y1/z1 pins make the |= shifts in place.
   Loops 2-4 follow the same game pattern (loop 2: c out of place -> t1; loop 3: b -> a3; loop 4: c -> t1, results
   x in v1, then t0, t1), but the same rewrite there plus a fresh greedy pin search scored worse (76-126).
   Earlier (92): each loop has its own function-scope locals, greedy pins for loops 2/4. (192): one reused mask temp t,
   shifts in place, separate x/y/z/w, p/n update after the |= stores. */
#include "TOBJ.H"

typedef struct { char p0[0xe]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; } FT3;
typedef struct { char p0[0xe]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; char p3[6]; unsigned short uv3; } FT4;
typedef struct { char p0[0x16]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; } GT3;
typedef struct { char p0[0x1a]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; char p3[6]; unsigned short uv3; } GT4;

void func_8011752C(TObj *o)
{
    char *p;
    short n;
    register int a1 asm("$4");
    register int b1 asm("$3");
    register int c1 asm("$5");
    register int d1 asm("$7");
    register int x1 asm("$3");
    register int y1 asm("$8");
    register int z1 asm("$9");
    int w1;
    int t1;
    register int a2 asm("$4");
    register int b2 asm("$9");
    register int c2 asm("$6");
    register int d2 asm("$5");
    int x2;
    int y2;
    int z2;
    int w2;
    int t2;
    register int a3 asm("$4");
    int b3;
    int c3;
    int d3;
    int x3;
    int y3;
    int z3;
    int w3;
    int t3;
    register int a4 asm("$6");
    register int b4 asm("$4");
    int c4;
    register int d4 asm("$5");
    int x4;
    int y4;
    int z4;
    int w4;
    int t4;

    switch (o->state) {
    case 0:
        o->state++;
        o->timer = 6;
        o->w22 = 0;
        break;
    case 1:
        if (--o->timer != 0) break;
        o->timer = 10;
        o->w22++;
        p = (char *)o->da0;
        n = *(int *)p;
        p += 4;
        while (n) {
            FT3 *f = (FT3 *)p;
            a1 = f->uv0; b1 = f->uv1; c1 = f->uv2;
            { t1 = a1 & 0xff; f->uv0 = t1; t1 = b1 & 0xff; f->uv1 = t1; t1 = c1 & 0xff; f->uv2 = t1; }
            a1 <<= 16; a1 >>= 24; b1 <<= 16; d1 = b1 >> 24; c1 <<= 16; c1 >>= 24;
            if (o->w22 & 1) { x1 = a1 + 0x60; y1 = d1 + 0x60; z1 = c1 + 0x60; }
            else { x1 = a1 - 0x60; y1 = d1 - 0x60; z1 = c1 - 0x60; }
            f->uv0 |= x1 << 8; f->uv1 |= y1 << 8; f->uv2 |= z1 << 8;
            p += sizeof(FT3);
            n--;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            FT4 *f = (FT4 *)p;
            a2 = f->uv0; b2 = f->uv1; c2 = f->uv2; d2 = f->uv3;
            { t2 = a2 & 0xff; f->uv0 = t2; t2 = b2 & 0xff; f->uv1 = t2; t2 = c2 & 0xff; f->uv2 = t2; t2 = d2 & 0xff; f->uv3 = t2; }
            a2 <<= 16; a2 >>= 24; b2 <<= 16; b2 >>= 24; c2 <<= 16; c2 >>= 24; d2 <<= 16; d2 >>= 24;
            if (o->w22 & 1) { x2 = a2 + 0x60; y2 = b2 + 0x60; z2 = c2 + 0x60; w2 = d2 + 0x60; }
            else { x2 = a2 - 0x60; y2 = b2 - 0x60; z2 = c2 - 0x60; w2 = d2 - 0x60; }
            f->uv0 |= x2 << 8; f->uv1 |= y2 << 8; f->uv2 |= z2 << 8; f->uv3 |= w2 << 8;
            p += sizeof(FT4);
            n--;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            GT3 *f = (GT3 *)p;
            a3 = f->uv0; b3 = f->uv1; c3 = f->uv2;
            { t3 = a3 & 0xff; f->uv0 = t3; t3 = b3 & 0xff; f->uv1 = t3; t3 = c3 & 0xff; f->uv2 = t3; }
            a3 <<= 16; a3 >>= 24; b3 <<= 16; b3 >>= 24; c3 <<= 16; c3 >>= 24;
            if (o->w22 & 1) { x3 = a3 + 0x60; y3 = b3 + 0x60; z3 = c3 + 0x60; }
            else { x3 = a3 - 0x60; y3 = b3 - 0x60; z3 = c3 - 0x60; }
            f->uv0 |= x3 << 8; f->uv1 |= y3 << 8; f->uv2 |= z3 << 8;
            p += sizeof(GT3);
            n--;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            GT4 *f = (GT4 *)p;
            a4 = f->uv0; b4 = f->uv1; c4 = f->uv2; d4 = f->uv3;
            { t4 = a4 & 0xff; f->uv0 = t4; t4 = b4 & 0xff; f->uv1 = t4; t4 = c4 & 0xff; f->uv2 = t4; t4 = d4 & 0xff; f->uv3 = t4; }
            a4 <<= 16; a4 >>= 24; b4 <<= 16; b4 >>= 24; c4 <<= 16; c4 >>= 24; d4 <<= 16; d4 >>= 24;
            if (o->w22 & 1) { x4 = a4 + 0x60; y4 = b4 + 0x60; z4 = c4 + 0x60; w4 = d4 + 0x60; }
            else { x4 = a4 - 0x60; y4 = b4 - 0x60; z4 = c4 - 0x60; w4 = d4 - 0x60; }
            f->uv0 |= x4 << 8; f->uv1 |= y4 << 8; f->uv2 |= z4 << 8; f->uv3 |= w4 << 8;
            p += sizeof(GT4);
            n--;
        }
        break;
    }
}
