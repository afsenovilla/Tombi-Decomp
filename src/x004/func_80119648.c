// FUNC 80119648 956 X004
// MATCHING 80119648 956
/* UV scroll of the poly lists at o->da0. Loop values are block-local shorts with separate results (a2..d2),
   and the masked halfword goes through one reused temp t, which keeps the and+sh pairs in source order. */
#include "TOBJ.H"
typedef struct { char p0[0xe]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; char p3[6]; unsigned short f3; } E28;
typedef struct { char p0[0x1e]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; char p3[6]; unsigned short f3; } E34h;

#define SPLIT(e)              \
    a = e->f0;                \
    b = e->f1;                \
    c = e->f2;                \
    d = e->f3;                \
    t = a & ~0xff;            \
    e->f0 = t;                \
    t = b & ~0xff;            \
    e->f1 = t;                \
    t = c & ~0xff;            \
    e->f2 = t;                \
    t = d & ~0xff;            \
    e->f3 = t;                \
    a &= 0xff;                \
    b &= 0xff;                \
    c &= 0xff;                \
    d &= 0xff;

#define BODY_PM(e)            \
    SPLIT(e)                  \
    if (o->w22 & 3) {         \
        a2 = a - 0x30;        \
        b2 = b - 0x30;        \
        c2 = c - 0x30;        \
        d2 = d - 0x30;        \
    } else {                  \
        a2 = a + 0x90;        \
        b2 = b + 0x90;        \
        c2 = c + 0x90;        \
        d2 = d + 0x90;        \
    }                         \
    e->f0 |= a2;              \
    e->f1 |= b2;              \
    e->f2 |= c2;              \
    e->f3 |= d2;

void func_80119648(TObj *o)
{
    char *p;
    short n;
    int t;

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
            short a, b, c, d, a2, b2, c2, d2;
            BODY_PM(e)
            p += 0x28;
        } while (--n);
        p += 4;
        n = *(unsigned short *)p;
        do {
            E34h *e = (E34h *)p;
            short a, b, c, d, a2, b2, c2, d2;
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
            short a, b, c, d;
            int k;
            SPLIT(e)
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
            short a, b, c, d, a2, b2, c2, d2;
            BODY_PM(e)
            p += 0x34;
        } while (--n);
        break;
    }
}
