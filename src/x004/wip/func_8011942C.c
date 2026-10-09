// FUNC 8011942c 540 X004
/* score 186: logic and block layout match; registers differ throughout: game keeps o in t2 (move t2,a0 at entry)
   and uses a0 for the prim base (da0+4) and temps, -0x100 mask in t3, copies of (uv0 & 0xff) in v1/a3.
   Tried: whole body as static inline (TObj * and void * param), switch for the 0xd0/0xff test, per-local type search. */
#include "TOBJ.H"

typedef struct {
    char p0[0xc]; unsigned short uv0; char p1[6]; unsigned short uv1; char p2[6];
    unsigned short uv2; char p3[6]; unsigned short uv3; char p4[2];
} FT4;

void func_8011942C(TObj *o)
{
    short n;
    FT4 *f;
    unsigned short *p;
    int u;
    int a;
    int b;
    int c;
    short t;
    int v0;
    short v1;
    short v2;
    short v3;

    switch (o->state) {
    case 0:
        o->state++;
        o->timer = 10;
        o->w22 = 0;
        break;
    case 1:
        if (--o->timer == 0) {
            o->timer = 10;
            o->w22++;
            p = (unsigned short *)(o->da0 + 4);
            n = *p;
            f = (FT4 *)(p + 3);
            do {
                u = f->uv0;
                t = u & 0xff;
                if (t == 0xd0 || t == 0xff) {
                    a = f->uv1;
                    b = f->uv2;
                    c = f->uv3;
                    f->uv0 = t;
                    f->uv1 = a & 0xff;
                    f->uv2 = b & 0xff;
                    f->uv3 = c & 0xff;
                    v0 = (short)u >> 8;
                    v1 = (short)a >> 8;
                    v2 = (short)b >> 8;
                    v3 = (short)c >> 8;
                    if (o->w22 & 1) {
                        v0 += 0x60; v1 += 0x60; v2 += 0x60; v3 += 0x60;
                    } else {
                        v0 -= 0x60; v1 -= 0x60; v2 -= 0x60; v3 -= 0x60;
                    }
                    f->uv0 |= v0 << 8;
                    f->uv1 |= v1 << 8;
                    f->uv2 |= v2 << 8;
                    f->uv3 |= v3 << 8;
                } else {
                    a = f->uv1;
                    b = f->uv2;
                    c = f->uv3;
                    f->uv0 = u & ~0xff;
                    f->uv1 = a & ~0xff;
                    f->uv2 = b & ~0xff;
                    f->uv3 = c & ~0xff;
                    v1 = a & 0xff;
                    v2 = b & 0xff;
                    v3 = c & 0xff;
                    if (o->w22 & 1) {
                        t += 0x30; v1 += 0x30; v2 += 0x30; v3 += 0x30;
                    } else {
                        t -= 0x30; v1 -= 0x30; v2 -= 0x30; v3 -= 0x30;
                    }
                    f->uv0 |= t;
                    f->uv1 |= v1;
                    f->uv2 |= v2;
                    f->uv3 |= v3;
                    return;
                }
                f++;
            } while (--n);
        }
        break;
    }
}
