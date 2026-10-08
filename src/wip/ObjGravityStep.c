// FUNC 8010f0f4 352 X000
// FLAGS -O2 -G0 -fno-schedule-insns2
typedef struct T { short lim; unsigned short inc; char pad[16]; } T;
typedef struct O {
    char p0[0x7e]; short w7e;
    char p1[0xb2 - 0x80]; short wb2;
    char p2[0xc1 - 0xb4]; unsigned char bc1;
} O;
typedef struct G { char p0[0x20]; unsigned short w20; } G;
extern unsigned char DAT_8009d2b3, DAT_8009c990, DAT_8009cf06, DAT_8009d006;
extern G *DAT_80096330;
extern T DAT_80115466[];

void ObjGravityStep(O *o)
{
    int a, i, k, b;
    unsigned char c;
    char pad;
    b = o->wb2;
    a = b;
    c = o->bc1;
    k = DAT_8009d2b3;
    if (b < 0) a = -a;
    k += c << 2;
    if (DAT_8009c990 & 3) k = (c << 2) | 3;
    if (DAT_8009cf06) k = 8;
    if (DAT_8009d006) k = 8;
    i = k;
    if (DAT_80096330->w20 < 13 && (short)DAT_80096330->w20 >= 0)
        o->w7e += DAT_80115466[i].inc;
    else
        o->w7e = o->w7e - 8 + (DAT_80115466[i].inc - ((short)a >> 7));
    if (DAT_80115466[i].lim < o->w7e)
        o->w7e = DAT_80115466[i].lim;
    if (o->w7e < -DAT_80115466[i].lim)
        o->w7e = -DAT_80115466[i].lim;
}
