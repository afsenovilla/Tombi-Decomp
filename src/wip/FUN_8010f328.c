// FUNC 8010f328 216 X000
extern unsigned char DAT_8009d2b3;
extern unsigned char DAT_8009c990;
extern unsigned char DAT_8009cf06;
extern unsigned char DAT_8009d006;
extern char *DAT_8009c330;
extern unsigned short DAT_80115468[][10];

static __inline__ void fn(char *o)
{
    unsigned b = (unsigned char)o[0xc1];
    unsigned u = DAT_8009d2b3 + b * 4;
    if ((DAT_8009c990 & 3) != 0)
        u = b << 2 | 3;
    if (DAT_8009cf06 != 0)
        u = 8;
    if (DAT_8009d006 != 0)
        u = 8;
    {
        int x = *(unsigned short *)(DAT_8009c330 + 0x20);
        if (x >= 0) {
            if (x < 0xd)
                *(unsigned short *)(o + 0x7e) = DAT_80115468[u][2];
            else if (x == 0xd)
                *(unsigned short *)(o + 0x7e) = DAT_80115468[u][1];
        }
    }
}

void FUN_8010f328(char *o)
{
    fn(o);
}
