// FUNC 80119a04 1712 X004
// MATCHING 80119a04 1712
/* Same UV-scroll family as func_80119648: block-local short values, one reused temp t for the masked
   halfwords, short counts loaded from int words. The results a2..d2 are function-scope here (shared by the
   four lists): that lowers their allocation priority below the hoisted -0x100 mask (game regs a3/t0/t1). */
#include "TOBJ.H"
typedef struct { char p0[0xe]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; } E20;
typedef struct { char p0[0xe]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; char p3[6]; unsigned short f3; } E28;
typedef struct { char p0[0x16]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; } E28b;
typedef struct { char p0[0x1a]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; char p3[6]; unsigned short f3; } E34;

#define SPLIT3(e)             \
    a = e->f0;                \
    b = e->f1;                \
    c = e->f2;                \
    t = a & ~0xff;            \
    e->f0 = t;                \
    t = b & ~0xff;            \
    e->f1 = t;                \
    t = c & ~0xff;            \
    e->f2 = t;                \
    a &= 0xff;                \
    b &= 0xff;                \
    c &= 0xff;

#define SPLIT4(e)             \
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

#define PM3(e)                \
    SPLIT3(e)                 \
    if (o->w22 & 3) {         \
        a2 = a - 0x30;        \
        b2 = b - 0x30;        \
        c2 = c - 0x30;        \
    } else {                  \
        a2 = a + 0x90;        \
        b2 = b + 0x90;        \
        c2 = c + 0x90;        \
    }

#define OR3(e)                \
    e->f0 |= a2;              \
    e->f1 |= b2;              \
    e->f2 |= c2;

#define PM4(e)                \
    SPLIT4(e)                 \
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
    }

#define OR4(e)                \
    e->f0 |= a2;              \
    e->f1 |= b2;              \
    e->f2 |= c2;              \
    e->f3 |= d2;

#define K3(e)                 \
    SPLIT3(e)                 \
    k = (o->w22 & 3) * 0x30;  \
    e->f0 |= a + k;           \
    e->f1 |= b + k;           \
    e->f2 |= c + k;

#define K4(e)                 \
    SPLIT4(e)                 \
    k = (o->w22 & 3) * 0x30;  \
    e->f0 |= a + k;           \
    e->f1 |= b + k;           \
    e->f2 |= c + k;           \
    e->f3 |= d + k;

void func_80119A04(TObj *o)
{
    char *p;
    short n;
    int t;
    short a2, b2, c2, d2;

    switch (o->state) {
    case 0:
        o->state++;
        o->timer = 5;
        o->w22 = 0;
        break;
    case 1:
        if (--o->timer != 0) break;
        o->timer = 5;
        o->w22++;
        p = (char *)o->da0;
        n = *(int *)p;
        p += 4;
        if (n) {
            do {
                E20 *e = (E20 *)p;
                short a, b, c;
                PM3(e)
                OR3(e)
                p += 0x20;
            } while (--n);
        }
        n = *(int *)p;
        p += 4;
        if (n) {
            do {
                E28 *e = (E28 *)p;
                short a, b, c, d;
                PM4(e)
                OR4(e)
                p += 0x28;
            } while (--n);
        }
        n = *(int *)p;
        p += 4;
        if (n) {
            do {
                E28b *e = (E28b *)p;
                short a, b, c;
                PM3(e)
                OR3(e)
                p += 0x28;
            } while (--n);
        }
        n = *(int *)p;
        p += 4;
        if (n) {
            do {
                E34 *e = (E34 *)p;
                short a, b, c, d;
                PM4(e)
                OR4(e)
                p += 0x34;
            } while (--n);
        }
        break;
    case 2:
        p = (char *)o->da0;
        n = *(int *)p;
        p += 4;
        if (n) {
            do {
                E20 *e = (E20 *)p;
                short a, b, c;
                int k;
                K3(e)
                p += 0x20;
            } while (--n);
        }
        n = *(int *)p;
        p += 4;
        if (n) {
            do {
                E28 *e = (E28 *)p;
                short a, b, c, d;
                int k;
                K4(e)
                p += 0x28;
            } while (--n);
        }
        n = *(int *)p;
        p += 4;
        if (n) {
            do {
                E28b *e = (E28b *)p;
                short a, b, c;
                int k;
                K3(e)
                p += 0x28;
            } while (--n);
        }
        n = *(int *)p;
        p += 4;
        if (n) {
            do {
                E34 *e = (E34 *)p;
                short a, b, c, d;
                int k;
                K4(e)
                p += 0x34;
            } while (--n);
        }
        break;
    }
}
