// FUNC 80136090 340 X000
// MATCHING 80136090 340
extern void FUN_800202b4(unsigned char *), FUN_8001fe6c(unsigned char *), FUN_80018790(unsigned char *), FUN_80135db0(unsigned char *);
extern void FUN_80135b0c(unsigned char *);
extern int FUN_8001fec0(unsigned char *);
extern unsigned char DAT_8009cdac;
extern int DAT_1f8002d4;
extern unsigned char *PTR_8013b15c, *PTR_8013b19c;

void FUN_80136090(unsigned char *o)
{
    unsigned char s;
    int t, w;
    unsigned char *u;
    s = o[4];
    switch (s) {
    case 0:
        if (DAT_8009cdac != 0xff)
            o[4] = 3;
        else {
            o[4] = s + 1;
            o[0] = 2;
            *(short *)(o + 0x6c) = 0x18;
            *(short *)(o + 0x6e) = 0x30;
            *(short *)(o + 0x70) = 0x18;
            *(short *)(o + 0x72) = 0x30;
            w = DAT_1f8002d4;
            o[0xd] = 0;
            *(int *)(o + 0x3c) = w;
            if (o[3] == 0) {
                *(short *)(o + 0x1e) = 8;
                *(unsigned char **)(o + 0x24) = PTR_8013b15c;
            } else {
                t = *(short *)(o + 0x16);
                u = PTR_8013b19c;
                *(short *)(o + 0x1e) = 0xb;
                *(int *)(o + 0x34) = t;
                *(unsigned char **)(o + 0x24) = u;
            }
            FUN_8001fe6c(o);
        }
        break;
    case 1:
        FUN_800202b4(o);
        if (o[3] == 0) {
            FUN_8001fec0(o);
            FUN_80135b0c(o);
        } else
            FUN_80135db0(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
