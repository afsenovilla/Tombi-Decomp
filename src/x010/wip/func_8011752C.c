// FUNC 8011752c 1020 X010
/* score 381: layout and loop/giv structure match (o in a t-register, uv pointer = p + offset); remaining:
   scheduling/regs inside each loop body (game stores each masked uv right after its andi through v0,
   then does the sign shifts), loop counter/pointer registers. Same family as X004 func_8011942C.
   Tried: raw-offset macro version, prim structs, local type search. */
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
            f->uv0 = a & 0xff; f->uv1 = b & 0xff; f->uv2 = c & 0xff;
            a = (short)a >> 8; b = (short)b >> 8; c = (short)c >> 8;
            if (o->w22 & 1) { a += 0x60; b += 0x60; c += 0x60; }
            else { a -= 0x60; b -= 0x60; c -= 0x60; }
            p += sizeof(FT3);
            n--;
            f->uv0 |= a << 8; f->uv1 |= b << 8; f->uv2 |= c << 8;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            FT4 *f = (FT4 *)p;
            a = f->uv0; b = f->uv1; c = f->uv2; d = f->uv3;
            f->uv0 = a & 0xff; f->uv1 = b & 0xff; f->uv2 = c & 0xff; f->uv3 = d & 0xff;
            a = (short)a >> 8; b = (short)b >> 8; c = (short)c >> 8; d = (short)d >> 8;
            if (o->w22 & 1) { a += 0x60; b += 0x60; c += 0x60; d += 0x60; }
            else { a -= 0x60; b -= 0x60; c -= 0x60; d -= 0x60; }
            p += sizeof(FT4);
            n--;
            f->uv0 |= a << 8; f->uv1 |= b << 8; f->uv2 |= c << 8; f->uv3 |= d << 8;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            GT3 *f = (GT3 *)p;
            a = f->uv0; b = f->uv1; c = f->uv2;
            f->uv0 = a & 0xff; f->uv1 = b & 0xff; f->uv2 = c & 0xff;
            a = (short)a >> 8; b = (short)b >> 8; c = (short)c >> 8;
            if (o->w22 & 1) { a += 0x60; b += 0x60; c += 0x60; }
            else { a -= 0x60; b -= 0x60; c -= 0x60; }
            p += sizeof(GT3);
            n--;
            f->uv0 |= a << 8; f->uv1 |= b << 8; f->uv2 |= c << 8;
        }
        n = *(int *)p;
        p += 4;
        while (n) {
            GT4 *f = (GT4 *)p;
            a = f->uv0; b = f->uv1; c = f->uv2; d = f->uv3;
            f->uv0 = a & 0xff; f->uv1 = b & 0xff; f->uv2 = c & 0xff; f->uv3 = d & 0xff;
            a = (short)a >> 8; b = (short)b >> 8; c = (short)c >> 8; d = (short)d >> 8;
            if (o->w22 & 1) { a += 0x60; b += 0x60; c += 0x60; d += 0x60; }
            else { a -= 0x60; b -= 0x60; c -= 0x60; d -= 0x60; }
            p += sizeof(GT4);
            n--;
            f->uv0 |= a << 8; f->uv1 |= b << 8; f->uv2 |= c << 8; f->uv3 |= d << 8;
        }
        break;
    }
}
