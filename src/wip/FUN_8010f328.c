// FUNC 8010f328 216 X000
extern unsigned char DAT_8009d2b3;
extern unsigned char DAT_8009c990;
extern unsigned char DAT_8009cf06;
extern unsigned char DAT_8009d006;
extern char *DAT_8009c330;
extern unsigned short DAT_80115468[][10];

static __inline__ void fn(char *o, unsigned b)
{
    unsigned u = DAT_8009d2b3 + b * 4;
    if ((DAT_8009c990 & 3) != 0)
        u = b << 2 | 3;
    if (DAT_8009cf06 != 0)
        u = 8;
    if (DAT_8009d006 != 0)
        u = 8;
    switch (*(unsigned short *)(DAT_8009c330 + 0x20)) {
    case 0 ... 12:
        *(unsigned short *)(o + 0x7e) = DAT_80115468[u][2];
        break;
    case 13:
        *(unsigned short *)(o + 0x7e) = DAT_80115468[u][1];
        break;
    }
}

void FUN_8010f328(char *o)
{
    fn(o, (unsigned char)o[0xc1]);
}
