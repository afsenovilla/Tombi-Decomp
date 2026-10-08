// FUNC 8001baf4 484 MAIN0
// MATCHING 8001baf4 484
typedef struct { char p[0x4c]; short w4c; unsigned short w4e; } GS;
extern GS *DAT_1f8001d4;
extern unsigned char DAT_1f8001c2, DAT_1f8001cf, DAT_8009c967, DAT_800b1410;
extern unsigned char DAT_8009e375, DAT_8009e376, DAT_8009e377, DAT_8009f085, DAT_8009f086, DAT_8009f087;
extern volatile unsigned short DAT_8009d674;
extern unsigned short DAT_1f8001fc, DAT_8009d670;
extern int DAT_8009c968;
extern void InitSubsystems(void), FUN_80017b44(void), FUN_80021858(void), FUN_80021b20(void), FUN_8004bfe4(void), FUN_800263bc(void), FUN_8001eb64(void), FUN_8001bcd8(void);
extern unsigned char *FUN_80018678(void);

void FUN_8001baf4(void)
{
    unsigned char *p;
    volatile unsigned short *k;
    char pad[8];

    switch (DAT_1f8001d4->w4e) {
    case 0:
        InitSubsystems();
        FUN_80017b44();
        FUN_80021858();
        FUN_80021b20();
        FUN_8004bfe4();
        FUN_800263bc();
        DAT_800b1410 = 0;
        DAT_1f8001cf = 1;
        if (DAT_8009c967 != 1)
            FUN_8001eb64();
        DAT_1f8001d4->w4e++;
        p = FUN_80018678();
        if (p != 0) {
            p[0] = 1;
            p[2] = 0xd;
            p[3] = 0;
            *(short *)(p + 0x12) = 0;
            *(short *)(p + 0x16) = 0;
            *(short *)(p + 0x1a) = 0;
        }
        DAT_8009d674 = 0;
        DAT_1f8001fc = 0;
        *(volatile unsigned short *)&DAT_8009d670 = DAT_8009d674;
        break;
    case 1:
        { int *g = &DAT_8009c968; *g = *g + 1; }
        FUN_8001bcd8();
        if (DAT_1f8001c2 != 0) {
            k = &DAT_8009d670;
            if ((*k & 8) && (*k & 0x800))
                DAT_1f8001d4->w4e = 3;
        }
        break;
    case 2:
    case 3:
        DAT_8009e375 = 0;
        DAT_8009e376 = 0;
        DAT_8009e377 = 0;
        DAT_8009f085 = 0;
        DAT_8009f086 = 0;
        DAT_8009f087 = 0;
        DAT_1f8001d4->w4c = 8;
        DAT_1f8001d4->w4e = 0;
        break;
    }
}
