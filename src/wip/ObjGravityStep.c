// FUNC 8010f0f4 352 X000
/* score 102: switch con case 0 ... 12 reproduce slti/bltz. Falta el frame de 0x10 (addiu sp al inicio) y regs (o=a1, a=a3). */
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

static __inline__ void inl(O *o)
{
    int a, k, t;
    unsigned char c;
    a = o->wb2;
    c = o->bc1;
    k = D_8009D2B3;
    if (a < 0) a = -a;
    k += c << 2;
    if (D_8009C990 & 3) k = (c << 2) | 3;
    if (D_8009CF06) k = 8;
    if (D_8009D006) k = 8;
    switch (D_8009C330->w20) {
    case 0 ... 12:
        o->w7e += D_80115466[k].inc;
        break;
    default:
        o->w7e = o->w7e - 8 + (D_80115466[k].inc - ((short)a >> 7));
        break;
    }
    if (D_80115466[k].lim < o->w7e)
        o->w7e = D_80115466[k].lim;
    if (o->w7e < -D_80115466[k].lim)
        o->w7e = -D_80115466[k].lim;
}

void ObjGravityStep(O *o)
{
    inl(o);
}
