// FUNC 8011752c 1020 X010
/* score 192 (o27, was 381): masks through one reused temp t (stores interleave like the game), shifts in place
   (a <<= 16; a >>= 24), separate x/y/z/w for the +-0x60 results, p/n update after the |= stores. Left: register
   choice of the loaded/shifted/adjusted values per loop (game loop1: a0,v1,a1 -> a0,a3,a1 -> v1,t0,t1) and o in t4.
   Exhaustive per-loop search over shift form (in place / (short)x>>8) and adj target (new var / in place),
   with function-scope and block-local vars, stalls at 192. */
#include "TOBJ.H"

typedef struct { char p0[0xe]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; } FT3;
typedef struct { char p0[0xe]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; char p3[6]; unsigned short uv3; } FT4;
typedef struct { char p0[0x16]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; } GT3;
typedef struct { char p0[0x1a]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6]; unsigned short uv2; char p3[6]; unsigned short uv3; } GT4;

void func_8011752C(TObj *o)
{
    char *p;
    short n;
    int a, b, c, d;
    int x, y, z, w;

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
            a = f->uv0; b = f->uv1; c = f->uv2;
            { int t; t = a & 0xff; f->uv0 = t; t = b & 0xff; f->uv1 = t; t = c & 0xff; f->uv2 = t; }
            a <<= 16; a >>= 24; b <<= 16; b >>= 24; c = (short)c >> 8;
            if (o->w22 & 1) { x = a + 0x60; y = b + 0x60; c = c + 0x60; }
            else { x = a - 0x60; y = b - 0x60; c = c - 0x60; }
            f->uv0 |= x << 8; f->uv1 |= y << 8; f->uv2 |= c << 8;
            p += sizeof(FT3);
            n--;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            FT4 *f = (FT4 *)p;
            a = f->uv0; b = f->uv1; c = f->uv2; d = f->uv3;
            { int t; t = a & 0xff; f->uv0 = t; t = b & 0xff; f->uv1 = t; t = c & 0xff; f->uv2 = t; t = d & 0xff; f->uv3 = t; }
            a <<= 16; a >>= 24; b <<= 16; b >>= 24; c <<= 16; c >>= 24; d <<= 16; d >>= 24;
            if (o->w22 & 1) { x = a + 0x60; y = b + 0x60; z = c + 0x60; w = d + 0x60; }
            else { x = a - 0x60; y = b - 0x60; z = c - 0x60; w = d - 0x60; }
            f->uv0 |= x << 8; f->uv1 |= y << 8; f->uv2 |= z << 8; f->uv3 |= w << 8;
            p += sizeof(FT4);
            n--;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            GT3 *f = (GT3 *)p;
            a = f->uv0; b = f->uv1; c = f->uv2;
            { int t; t = a & 0xff; f->uv0 = t; t = b & 0xff; f->uv1 = t; t = c & 0xff; f->uv2 = t; }
            a <<= 16; a >>= 24; b <<= 16; b >>= 24; c <<= 16; c >>= 24;
            if (o->w22 & 1) { x = a + 0x60; y = b + 0x60; z = c + 0x60; }
            else { x = a - 0x60; y = b - 0x60; z = c - 0x60; }
            f->uv0 |= x << 8; f->uv1 |= y << 8; f->uv2 |= z << 8;
            p += sizeof(GT3);
            n--;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            GT4 *f = (GT4 *)p;
            a = f->uv0; b = f->uv1; c = f->uv2; d = f->uv3;
            { int t; t = a & 0xff; f->uv0 = t; t = b & 0xff; f->uv1 = t; t = c & 0xff; f->uv2 = t; t = d & 0xff; f->uv3 = t; }
            a <<= 16; a >>= 24; b <<= 16; b >>= 24; c <<= 16; c >>= 24; d <<= 16; d >>= 24;
            if (o->w22 & 1) { x = a + 0x60; y = b + 0x60; z = c + 0x60; w = d + 0x60; }
            else { x = a - 0x60; y = b - 0x60; z = c - 0x60; w = d - 0x60; }
            f->uv0 |= x << 8; f->uv1 |= y << 8; f->uv2 |= z << 8; f->uv3 |= w << 8;
            p += sizeof(GT4);
            n--;
        }
        break;
    }
}
