// FUNC 80127380 332 X000
typedef struct AE { char p0[2]; unsigned short w2; unsigned short w4; unsigned short v; } AE;
typedef struct H { char p0[2]; unsigned short x; } H;
typedef struct O {
    char p0[0x16]; unsigned short y;
    char p1[0x24 - 0x18]; AE *a;
    char p2[0x2c - 0x28]; unsigned short t; unsigned short af;
    char p4[0x40 - 0x30]; H *h;
    char p5[0x6c - 0x44]; unsigned short w6c, w6e, w70, w72;
} O;
extern unsigned char DAT_80138fd8[];

int FUN_80127380(O *o)
{
    AE *e, *n; unsigned short v, w; int k;
    unsigned char *tb;
    int dx, dy; unsigned u;
    o->t = o->t - 1;
    if (o->t != 0) return 0;
    e = o->a;
    v = e->v;
    k = v & 0xc000;
    switch (k) {
    case 0:
        o->a = e + 1;
        w = e[1].w2;
        goto tail;
    case 0x4000:
        o->a = e + 1;
        n = *(AE **)(e + 1);
        o->a = n;
        w = n->w2;
    tail:
        tb = DAT_80138fd8;
        tb = (w << 2) + tb;
        o->w6c = *tb++;
        o->w6e = *tb++;
        o->w70 = *tb;
        o->w72 = tb[1];
        o->t = o->a->v & 0x3fff;
        u = o->a->w4;
        dx = u & 0xff;
        dy = u >> 8;
        if (o->af & 1) dx = -dx;
        o->h->x += dx;
        o->y += dy;
        return 0;
    case 0x8000:
        goto fin;
    case 0xc000:
    fin:
        o->t = v & 0x3fff;
        return 1;
    }
    return 0;
}
