// FUNC 8011880c 400 X000
#define H(p, o) (*(unsigned short *)((char *)(p) + (o)))
#define SH(p, o) (*(short *)((char *)(p) + (o)))

static __inline__ void f(unsigned char *o)
{
    unsigned short a, b, c, d, w;
    switch (o[6]) {
    case 0:
        SH(o, 0x82) = 0x80;
        SH(o, 0x7e) = -8;
        SH(o, 0x22) = 0;
        H(o, 0xb6) += o[0xc] * 0x48;
        H(o, 0xbe) += o[0xc] * 0x48;
        H(o, 0xc6) += o[0xc] * 0x48;
        H(o, 0xce) += o[0xc] * 0x48;
        o[6] = (o[0xc] & 1) + 1;
        break;
    case 1:
        a = H(o, 0xb4); b = H(o, 0xbc); c = H(o, 0xc4); d = H(o, 0xcc);
        w = H(o, 0x22) + 1;
        H(o, 0x22) = w;
        H(o, 0xb4) = a - 1;
        H(o, 0xbc) = b - 1;
        H(o, 0xc4) = c + 1;
        H(o, 0xcc) = d + 1;
        if ((short)w >= 0x50)
            o[6] = o[6] + 1;
        break;
    case 2:
        a = H(o, 0xb4); b = H(o, 0xbc); c = H(o, 0xc4); d = H(o, 0xcc);
        w = H(o, 0x22) - 1;
        H(o, 0x22) = w;
        H(o, 0xb4) = a + 1;
        H(o, 0xbc) = b + 1;
        H(o, 0xc4) = c - 1;
        H(o, 0xcc) = d - 1;
        if ((short)w <= -0x50)
            o[6] = o[6] - 1;
        break;
    }
}

void FUN_8011880c(unsigned char *o)
{
    f(o);
}
