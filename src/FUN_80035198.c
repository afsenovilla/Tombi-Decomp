// FUNC 80035198 348 MAIN0
// MATCHING 80035198 348
extern int FUN_8005efe4(void);
extern unsigned short FUN_8005e420(int, int);
extern short *DAT_8009c338;
extern short DAT_8009d600[];
extern unsigned short *DAT_800a6078, *DAT_800a607c;
extern unsigned short DAT_800a604e[];

static __inline__ short getm(void)
{
    if (FUN_8005efe4() == 1) return 0x80;
    if (FUN_8005efe4() == 2) return 0x80;
    return 0x20;
}

void FUN_80035198(unsigned char *o)
{
    short *p;
    DAT_8009c338 = (short *)(o + 0xac);
    if (o[6] == 0) {
        if (FUN_8005efe4() != 1)
            FUN_8005efe4();
        p = DAT_8009c338;
        *(short *)(o + 0xb0) = 0;
        p[4] = getm();
        DAT_8009c338[3] = FUN_8005e420(0x80, 0x1ef);
        DAT_8009c338[5] = FUN_8005e420(0x80, 0x1f0);
        DAT_8009d600[0] = 0xf;
        DAT_8009d600[1] = 8;
        DAT_8009d600[2] = 0x5a;
        *(signed char *)(o + 0xf) = -9;
        o[0xa] = 0;
        (*(unsigned short **)(o + 0x40))[1] = DAT_800a6078[1];
        *(unsigned short *)(o + 0x16) = DAT_800a604e[0] - 0x10;
        (*(unsigned short **)(o + 0x44))[1] = DAT_800a607c[1];
        *(short *)(o + 0x80) = 0;
        *(short *)(o + 0x82) = 0;
        *(short *)(o + 0x2e) = 0;
        o[1] = 1;
        o[6] = 0;
    }
}
