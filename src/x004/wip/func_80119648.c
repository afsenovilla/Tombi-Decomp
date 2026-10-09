// FUNC 80119648 956 X004
/* score 306: control flow and size match; the loop bodies differ in scheduling/regs: the game keeps and+sh pairs in order (as with -fno-schedule-insns, which gives the same body shape but other regs), and the -0x30/+0x90 results of c/d land in new regs t0/t1. Tried: per-field statement order, &= forms, int/short/ushort types for a..d and n, separate result vars, -fno-strength-reduce */
#include "TOBJ.H"
typedef struct { char p0[0xe]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; char p3[6]; unsigned short f3; } E28;
typedef struct { char p0[0x1e]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; char p3[6]; unsigned short f3; } E34h;

#define BODY_PM(e)                  \
    a = e->f0;                      \
    b = e->f1;                      \
    c = e->f2;                      \
    d = e->f3;                      \
    e->f0 = a & ~0xff;              \
    e->f1 = b & ~0xff;              \
    e->f2 = c & ~0xff;              \
    e->f3 = d & ~0xff;              \
    a &= 0xff;                      \
    b &= 0xff;                      \
    c &= 0xff;                      \
    d &= 0xff;                      \
    if (o->w22 & 3) {               \
        a -= 0x30;                  \
        b -= 0x30;                  \
        c -= 0x30;                  \
        d -= 0x30;                  \
    } else {                        \
        a += 0x90;                  \
        b += 0x90;                  \
        c += 0x90;                  \
        d += 0x90;                  \
    }                               \
    e->f0 |= a;                    \
    e->f1 |= b;                    \
    e->f2 |= c;                    \
    e->f3 |= d;

void func_80119648(TObj *o)
{
    char *p;
    int n;
    int a, b, c, d;
    int k;

    switch (o->state) {
    case 0:
        o->state++;
        o->timer = 10;
        o->w22 = 0;
        break;
    case 1:
        if (--o->timer != 0) break;
        o->timer = 10;
        o->w22++;
        p = (char *)o->da0;
        p += 4;
        n = *(unsigned short *)p;
        p += 4;
        do {
            E28 *e = (E28 *)p;
            BODY_PM(e)
            p += 0x28;
        } while (--n);
        p += 4;
        n = *(unsigned short *)p;
        do {
            E34h *e = (E34h *)p;
            BODY_PM(e)
            p += 0x34;
        } while (--n);
        break;
    case 2:
        p = (char *)o->da0;
        p += 4;
        n = *(unsigned short *)p;
        p += 4;
        do {
            E28 *e = (E28 *)p;
            a = e->f0;
            b = e->f1;
            c = e->f2;
            d = e->f3;
            e->f0 = a & ~0xff;
            e->f1 = b & ~0xff;
            e->f2 = c & ~0xff;
            e->f3 = d & ~0xff;
            a &= 0xff;
            b &= 0xff;
            c &= 0xff;
            d &= 0xff;
            k = (o->w22 & 3) * 0x30;
            e->f0 |= a + k;
            e->f1 |= b + k;
            e->f2 |= c + k;
            e->f3 |= d + k;
            p += 0x28;
        } while (--n);
        p += 4;
        n = *(unsigned short *)p;
        do {
            E34h *e = (E34h *)p;
            BODY_PM(e)
            p += 0x34;
        } while (--n);
        break;
    }
}
