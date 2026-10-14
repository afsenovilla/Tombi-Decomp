// FUNC 8011752c 1020 X010
/* score 92 (was 192): each loop has its own function-scope locals (a1..w1, a2..w2, ...; 274 unpinned), then greedy
   register pins (a2=$4 d2=$5 c2=$6 a3=$4 b2=$9 d4=$5 a4=$6 b4=$4). Left: loops 1/3 regs. Earlier (192): masks via one
   reused temp t, shifts in place, separate x/y/z/w, p/n update after the |= stores. */
#include "TOBJ.H"

typedef struct { char p0[0xe]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; } FT3;
typedef struct { char p0[0xe]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; char p3[6]; unsigned short uv3; } FT4;
typedef struct { char p0[0x16]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; } GT3;
typedef struct { char p0[0x1a]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; char p3[6]; unsigned short uv3; } GT4;

void func_8011752C(TObj *o)
{
    char *p;
    short n;
    int a1;
    int b1;
    int c1;
    int d1;
    int x1;
    int y1;
    int z1;
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
            a1 <<= 16; a1 >>= 24; b1 <<= 16; b1 >>= 24; c1 = (short)c1 >> 8;
            if (o->w22 & 1) { x1 = a1 + 0x60; y1 = b1 + 0x60; c1 = c1 + 0x60; }
            else { x1 = a1 - 0x60; y1 = b1 - 0x60; c1 = c1 - 0x60; }
            f->uv0 |= x1 << 8; f->uv1 |= y1 << 8; f->uv2 |= c1 << 8;
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
