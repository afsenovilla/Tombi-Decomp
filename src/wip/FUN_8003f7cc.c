// FUNC 8003f7cc 768 MAIN0
#include "TOBJ.H"
#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
extern short FUN_8004065c(TObj *, short, short, int);
extern unsigned short DAT_1f80016a;

#define HIT(f) \
    if ((f) == (o->animFrame & 1)) { \
        o->ba6 = (f) ? 3 : 2; \
    }

int FUN_8003f7cc(TObj *o)
{
    short d;
    int dx, dx2, f, f2, lo, hi, g;
    int a;
    short r;
    char pad[8];

    if (B(o, 0xaa) != 0) return 0;
    if (o->active == 5) return 0;
    d = o->h->p.whole - DAT_1f80016a;
    if (d == 0) {
        if (!(o->animFrame & 1)) goto P;
        goto L;
    }
    if (d < 0) goto L;
P:
    dx = 8;
    dx2 = -8;
    f = 0;
    f2 = 1;
    goto M;
L:
    dx = -8;
    dx2 = 8;
    f = 1;
    f2 = 0;
M:
    a = o->box2;
    lo = a;
    if (lo >= 8) lo = 8;
    hi = a;
    if (hi >= 14) hi = 14;
    r = FUN_8004065c(o, o->h->p.whole + dx, o->y.p.whole - lo, f);
    if (r) {
        HIT(f)
        goto N;
    }
    r = FUN_8004065c(o, o->h->p.whole + dx, o->y.p.whole + hi, f);
    if (r) {
        HIT(f)
        goto N;
    }
    r = FUN_8004065c(o, o->h->p.whole + dx, o->y.p.whole + lo, f);
    if (r) {
        HIT(f)
    }
N:
    f = f2;
    if (FUN_8004065c(o, o->h->p.whole + dx2, o->y.p.whole + hi, f)) {
        HIT(f)
        goto E;
    }
    if (FUN_8004065c(o, o->h->p.whole + dx2, o->y.p.whole + lo, f)) {
        HIT(f)
    }
E:
    return r;
}
