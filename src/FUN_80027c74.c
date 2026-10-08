// FUNC 80027c74 612 MAIN0
// MATCHING 80027c74 612
typedef struct O { char p0[0x14]; int y; char p1[0x2c-0x18]; short h2c; short h2e; char p2[0x34-0x30]; int *d34;
  char p3[0x3c-0x38]; unsigned char b3c; char p4[0x44-0x3d]; unsigned short h44; char p5[0x58-0x46]; unsigned short w58; } O;
typedef struct G { char p0[0x9c]; unsigned char f9c; } G;
extern G DAT_800a6038;
extern unsigned short DAT_800a6066;
extern unsigned char DAT_800a60d4, DAT_800a60d6;
extern int DAT_800a6068;
extern int *DAT_800a6078;

void FUN_80027c74(O *o)
{
    unsigned short u;
    int base, t, d;
    unsigned char neg;
    G *g;
    u = DAT_800a6066 & 1;
    base = *o->d34 - 0x100000;
    g = &DAT_800a6038;
    if (DAT_800a60d4)
        u = o->w58;
    o->w58 = u;
    if (u)
        base += 0x200000;
    if (DAT_800a60d6 == 3)
        t = DAT_800a6068 << 16;
    else
        t = *DAT_800a6078;
    d = t - base;
    neg = d < 0;
    if (d > 0x20000) {
        o->b3c = 0;
        if (d < o->y) {
            if (d < 0x40000)
                o->y = d;
            else {
                o->y = 0x40000;
                o->b3c = 1;
            }
        } else {
            if (o->y < 0)
                o->y = 0;
            o->y += 0x2000;
        }
        *o->d34 += o->y;
        if (o->h2e < ((short *)o->d34)[1]) {
            *o->d34 = o->h2e << 16;
            o->y = 0;
            o->h44 |= 1;
        }
        return;
    }
    if (d < -0x20000) {
        o->b3c = 0;
        if (o->y < d) {
            if (d > -0x40000)
                o->y = d;
            else {
                o->y = -0x40000;
                o->b3c = 1;
            }
        } else {
            if (o->y > 0)
                o->y = 0;
            o->y -= 0x2000;
        }
        *o->d34 += o->y;
        if (((short *)o->d34)[1] < o->h2c) {
            *o->d34 = o->h2c << 16;
            o->y = 0;
            o->h44 |= 2;
        }
        return;
    }
    o->b3c = 0;
    if (!g->f9c)
        o->b3c = 1;
    o->y = d;
    *o->d34 += d;
    if (neg) {
        if (((short *)o->d34)[1] < o->h2c) {
            *o->d34 = o->h2c << 16;
            o->y = 0;
            o->h44 |= 2;
        }
    }
L:
    if (o->h2e < ((short *)o->d34)[1]) {
        *o->d34 = o->h2e << 16;
        o->y = 0;
        o->h44 |= 1;
    }
}
