// FUNC 8001fec0 228 MAIN0
// MATCHING 8001fec0 228
typedef struct AE { char p0[6]; unsigned short v; } AE;
typedef struct { char p0[0x24]; AE *a; char p1[4]; unsigned short t; } TA;

int AnimAdvance(TA *o)
{
    AE *e, *n; unsigned short v, w; int k;
    o->t = o->t - 1;
    if (o->t != 0) return 0;
    e = o->a;
    v = e->v;
    k = v & 0xc000;
    switch (k) {
    case 0:
        o->a = e + 1;
        w = e[1].v;
        goto tail;
    case 0x4000:
        o->a = e + 1;
        n = *(AE **)(e + 1);
        o->a = n;
        w = n->v;
    tail:
        o->t = w & 0x3fff;
        return 0;
    case 0x8000:
        o->t = v & 0x3fff;
        return 1;
    case 0xc000:
        o->a = e + 1;
        n = *(AE **)(e + 1);
        o->a = n;
        o->t = n->v & 0x3fff;
        return 1;
    }
    return 0;
}
