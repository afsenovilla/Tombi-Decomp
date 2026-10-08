// FUNC 80127214 364 X000
// MATCHING 80127214 364
typedef struct AE { char p0[2]; unsigned short i; char p1[2]; unsigned short v; } AE;
typedef struct { char p0[0x24]; AE *a; char p1[4]; unsigned short t; char p2[0x3e]; unsigned short b0, b1, b2, b3; } TA;
extern unsigned char D_80138FD8[];

static __inline__ void box(TA *o, int idx)
{
    unsigned char *b = D_80138FD8 + idx * 4;
    o->b0 = *b++;
    o->b1 = *b++;
    o->b2 = *b;
    o->b3 = b[1];
}

int AnimAdvanceWithBox(TA *o)
{
    AE *e, *n; unsigned short v; int k;
    o->t = o->t - 1;
    if (o->t != 0) return 0;
    e = o->a;
    v = e->v;
    k = v & 0xc000;
    switch (k) {
    case 0:
        o->a = e + 1;
        box(o, e[1].i);
        o->t = o->a->v & 0x3fff;
        break;
    case 0x4000:
        o->a = e + 1;
        n = *(AE **)(e + 1);
        o->a = n;
        box(o, n->i);
        o->t = o->a->v & 0x3fff;
        break;
    case 0x8000:
        o->t = v & 0x3fff;
        return 1;
    case 0xc000:
        o->a = e + 1;
        n = *(AE **)(e + 1);
        o->a = n;
        box(o, n->i);
        o->t = o->a->v & 0x3fff;
        return 1;
    }
    return 0;
}
