// FUNC 8001ffa4 212 MAIN0
// MATCHING 8001ffa4 212
typedef struct AE { char p0[6]; unsigned short v; } AE;
typedef struct { char p0[0x20]; unsigned short w20; char p1[2]; AE *a; char p2[4]; unsigned short t; } TA;

int func_8001FFA4(TA *o)
{
    AE *e, *n;
    unsigned short t;
    o->t = o->t - 1;
    if (o->t != 0) return 0;
    e = o->a;
    switch (e->v & 0xc000) {
    case 0:
        t = o->w20;
        n = e + 1;
        goto tail;
    case 0x4000:
        o->a = e + 1;
        n = *(AE **)(e + 1);
        t = o->w20;
    tail:
        o->a = n;
        o->t = t;
        break;
    case 0x8000:
        o->t = o->w20;
        return 1;
    case 0xc000:
        o->a = e + 1;
        o->a = *(AE **)(e + 1);
        o->t = o->w20;
        return 1;
    }
    return 0;
}
