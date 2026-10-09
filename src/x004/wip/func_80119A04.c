// FUNC 80119a04 1712 X004
/* score 711 (first draft, 1660 of 1712 B): same UV-scroll family as func_80119648 (score 306); four poly lists per state (strides 0x20/0x28/0x28/0x34) with int counts tested as short. Loop bodies have the same and+sh ordering / register problem as func_80119648; solve that one first */
#include "TOBJ.H"
typedef struct { char p0[0xe]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; } E20;
typedef struct { char p0[0xe]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; char p3[6]; unsigned short f3; } E28;
typedef struct { char p0[0x16]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; } E28b;
typedef struct { char p0[0x1a]; unsigned short f0; char p1[6]; unsigned short f1; char p2[6]; unsigned short f2; char p3[6]; unsigned short f3; } E34;

#define PM3(e)                                                   \
    a = e->f0; b = e->f1; c = e->f2;                             \
    e->f0 = a & ~0xff; e->f1 = b & ~0xff; e->f2 = c & ~0xff;     \
    a &= 0xff; b &= 0xff; c &= 0xff;                             \
    if (o->w22 & 3) { a -= 0x30; b -= 0x30; c -= 0x30; }         \
    else { a += 0x90; b += 0x90; c += 0x90; }

#define OR3(e) e->f0 |= a; e->f1 |= b; e->f2 |= c;

#define PM4(e)                                                                    \
    a = e->f0; b = e->f1; c = e->f2; d = e->f3;                                   \
    e->f0 = a & ~0xff; e->f1 = b & ~0xff; e->f2 = c & ~0xff; e->f3 = d & ~0xff;  \
    a &= 0xff; b &= 0xff; c &= 0xff; d &= 0xff;                                   \
    if (o->w22 & 3) { a -= 0x30; b -= 0x30; c -= 0x30; d -= 0x30; }               \
    else { a += 0x90; b += 0x90; c += 0x90; d += 0x90; }

#define OR4(e) e->f0 |= a; e->f1 |= b; e->f2 |= c; e->f3 |= d;

#define K3(e)                                                    \
    a = e->f0; b = e->f1; c = e->f2;                             \
    e->f0 = a & ~0xff; e->f1 = b & ~0xff; e->f2 = c & ~0xff;     \
    a &= 0xff; b &= 0xff; c &= 0xff;                             \
    k = (o->w22 & 3) * 0x30;                                     \
    e->f0 |= a + k; e->f1 |= b + k; e->f2 |= c + k;

#define K4(e)                                                                     \
    a = e->f0; b = e->f1; c = e->f2; d = e->f3;                                   \
    e->f0 = a & ~0xff; e->f1 = b & ~0xff; e->f2 = c & ~0xff; e->f3 = d & ~0xff;  \
    a &= 0xff; b &= 0xff; c &= 0xff; d &= 0xff;                                   \
    k = (o->w22 & 3) * 0x30;                                                      \
    e->f0 |= a + k; e->f1 |= b + k; e->f2 |= c + k; e->f3 |= d + k;

void func_80119A04(TObj *o)
{
    char *p;
    int n;
    int a, b, c, d;
    int k;

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
        if ((short)n) {
            do { E20 *e = (E20 *)p; PM3(e) p += 0x20; OR3(e) } while ((short)--n);
        }
        n = *(int *)p;
        p += 4;
        if ((short)n) {
            do { E28 *e = (E28 *)p; PM4(e) p += 0x28; OR4(e) } while ((short)--n);
        }
        n = *(int *)p;
        p += 4;
        if ((short)n) {
            do { E28b *e = (E28b *)p; PM3(e) p += 0x28; OR3(e) } while ((short)--n);
        }
        n = *(int *)p;
        p += 4;
        if ((short)n) {
            do { E34 *e = (E34 *)p; PM4(e) OR4(e) p += 0x34; } while ((short)--n);
        }
        break;
    case 2:
        p = (char *)o->da0;
        n = *(int *)p;
        p += 4;
        if ((short)n) {
            do { E20 *e = (E20 *)p; p += 0x20; K3(e) } while ((short)--n);
        }
        n = *(int *)p;
        p += 4;
        if ((short)n) {
            do { E28 *e = (E28 *)p; p += 0x28; K4(e) } while ((short)--n);
        }
        n = *(int *)p;
        p += 4;
        if ((short)n) {
            do { E28b *e = (E28b *)p; p += 0x28; K3(e) } while ((short)--n);
        }
        n = *(int *)p;
        p += 4;
        if ((short)n) {
            do { E34 *e = (E34 *)p; K4(e) p += 0x34; } while ((short)--n);
        }
        break;
    }
}
