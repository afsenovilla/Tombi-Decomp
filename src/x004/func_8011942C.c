// FUNC 8011942c 540 X004
// MATCHING 8011942c 540
/* Branch 1: the first element's +-0x60 goes to a fresh local r0 (separate pseudo from v0); c pinned in $5,
   branch 2 keeps its own locals (a2/b2/c2, w1-w3). */
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
    register int c asm("$5");
    short t;
    int v0;
    short v1;
    short v2;
    short v3;
    int a2;
    int b2;
    int c2;
    short w1;
    short w2;
    short w3;
    int r0;

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
            p = (unsigned short *)o->da0;
            p += 2;
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
                        r0 = v0 + 0x60; v1 += 0x60; v2 += 0x60; v3 += 0x60;
                    } else {
                        r0 = v0 - 0x60; v1 -= 0x60; v2 -= 0x60; v3 -= 0x60;
                    }
                    f->uv0 |= r0 << 8;
                    f->uv1 |= v1 << 8;
                    f->uv2 |= v2 << 8;
                    f->uv3 |= v3 << 8;
                } else {
                    a2 = f->uv1;
                    b2 = f->uv2;
                    c2 = f->uv3;
                    f->uv0 = u & ~0xff;
                    f->uv1 = a2 & ~0xff;
                    f->uv2 = b2 & ~0xff;
                    f->uv3 = c2 & ~0xff;
                    w1 = a2 & 0xff;
                    w2 = b2 & 0xff;
                    w3 = c2 & 0xff;
                    if (o->w22 & 1) {
                        t += 0x30; w1 += 0x30; w2 += 0x30; w3 += 0x30;
                    } else {
                        t -= 0x30; w1 -= 0x30; w2 -= 0x30; w3 -= 0x30;
                    }
                    f->uv0 |= t;
                    f->uv1 |= w1;
                    f->uv2 |= w2;
                    f->uv3 |= w3;
                    return;
                }
                f++;
            } while (--n);
        }
        break;
    }
}
