// FUNC 80019d88 528 MAIN0
// MATCHING 80019d88 528
typedef struct P { char p0[0x4a]; unsigned short w4a; unsigned short w4c; unsigned short w4e; } P;
extern char *DAT_1f8001d4;
extern unsigned char DAT_1f8001b4, DAT_1f8001ce, DAT_1f8001cc, DAT_1f8001cd, DAT_1f8001d3;
extern unsigned short DAT_1f8001fc;
extern int DAT_8009f7e4;
extern unsigned char DAT_8009c974;
extern unsigned short DAT_8009c960, DAT_8009c962, DAT_8009c982;
extern unsigned short DAT_8009d2a8, DAT_8009d2aa, DAT_8009d2ac;
extern void FUN_8004fa80(int, int);
extern void FUN_8001c218(int);
extern void FUN_8001be1c(void);
extern void ThreadCreate(int, void *);
extern char LAB_8001d6a4[];

void FUN_80019d88(void)
{
    unsigned char *p974;
    switch (*(unsigned short *)(DAT_1f8001d4 + 0x4c)) {
    case 0:
        FUN_8004fa80(9, 1);
        goto inc;
    case 1:
        if (DAT_1f8001ce != 0)
            goto inc;
        return;
    case 2:
        if (DAT_1f8001b4 != 0)
            goto L7;
        DAT_8009f7e4 = 0;
        DAT_1f8001ce = 0;
        FUN_8001c218(1);
        goto inc;
    case 3:
        FUN_8001be1c();
        if (DAT_1f8001ce == 0)
            return;
        goto inc;
    case 4:
        DAT_1f8001cc = 1;
        DAT_1f8001cd = 1;
        ThreadCreate(1, LAB_8001d6a4);
    inc:
        (*(unsigned short *)(DAT_1f8001d4 + 0x4c))++;
        return;
    case 5:
        if (DAT_1f8001cc != 0) {
            if ((DAT_1f8001fc & 0x4008) == 0)
                return;
            DAT_1f8001d3 = 1;
            DAT_1f8001fc = 0;
            *(unsigned short *)(DAT_1f8001d4 + 0x4c) = 6;
            return;
        }
        goto L7;
    case 6:
        if (DAT_1f8001cc != 0)
            return;
    L7:
        *(unsigned short *)(DAT_1f8001d4 + 0x4c) = 7;
        return;
    case 7:
        p974 = &DAT_8009c974;
        *p974 = 1;
        *(unsigned short *)(DAT_1f8001d4 + 0x4c) = 1;
        if (DAT_1f8001b4 != 0) {
            *(unsigned short *)(DAT_1f8001d4 + 0x4c) = 0;
            *p974 = 0;
        }
        *(unsigned short *)(DAT_1f8001d4 + 0x4a) = 1;
        *(unsigned short *)(DAT_1f8001d4 + 0x4e) = 0;
        DAT_8009d2a8 = DAT_8009c960;
        DAT_8009d2aa = DAT_8009c962;
        DAT_8009d2ac = DAT_8009c982;
        return;
    }
}
