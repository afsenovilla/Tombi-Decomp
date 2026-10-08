// FUNC 8010f0f4 352 X003
// MATCHING 8010f0f4 352
typedef struct T { short lim; unsigned short inc; char pad[16]; } T;
typedef struct O {
    char p0[0x7e]; short w7e;
    char p1[0xb2 - 0x80]; short wb2;
    char p2[0xc1 - 0xb4]; unsigned char bc1;
} O;
typedef struct G { char p0[0x20]; unsigned short w20; } G;
extern unsigned char D_8009D2B3, D_8009C990, D_8009CF06, D_8009D006;
extern G *D_8009C330;
extern T D_80115466[];

static __inline__ int getk(O *o)
{
    int b = o->bc1;
    int u = D_8009D2B3 + b * 4;
    if ((D_8009C990 & 3) != 0)
        u = (unsigned char)b << 2 | 3;
    if (D_8009CF06 != 0)
        u = 8;
    if (D_8009D006 != 0)
        u = 8;
    return u;
}

void ObjGravityStep(O *o)
{
    short a; short k;
    a = o->wb2 < 0 ? -o->wb2 : o->wb2;
    k = getk(o);
    switch (D_8009C330->w20) {
    case 0 ... 12:
        o->w7e += D_80115466[k].inc;
        break;
    default:
        o->w7e += D_80115466[k].inc - (a >> 7) - 8;
        break;
    }
    if (D_80115466[k].lim < o->w7e)
        o->w7e = D_80115466[k].lim;
    if (o->w7e < -D_80115466[k].lim)
        o->w7e = -D_80115466[k].lim;
}
