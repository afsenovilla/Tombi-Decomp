// FUNC 8001b49c 484 MAIN0
// MATCHING 8001b49c 484
typedef struct G { char pad[0x4c]; unsigned short w4c, w4e; } G;
extern G *DAT_1f8001d4;
extern int DAT_8009c968[];
extern unsigned char DAT_1f8001c2;
extern volatile unsigned short DAT_8009d670[];
extern volatile unsigned short DAT_8009d674;
extern unsigned short DAT_1f8001fc;
extern unsigned char DAT_800b1410;
extern unsigned char DAT_1f8001cf;
extern unsigned char DAT_8009c967;
extern unsigned char DAT_8009e375, DAT_8009e376, DAT_8009e377;
extern unsigned char DAT_8009f085, DAT_8009f086, DAT_8009f087;
extern void FUN_800175f0(void);
extern void FUN_80017b44(void);
extern void FUN_80021858(void);
extern void FUN_80021b20(void);
extern void FUN_8004bfe4(void);
extern void FUN_800263bc(void);
extern void FUN_8001eb64(void);
extern void FUN_8001b680(void);
extern unsigned char *FUN_80018678(void);

void FUN_8001b49c(void)
{
    unsigned char *q;
    char pad[8];
    switch (DAT_1f8001d4->w4e) {
    case 0:
        FUN_800175f0();
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
        q = FUN_80018678();
        if (q != 0) {
            q[0] = 1;
            q[2] = 5;
            q[3] = 0;
            *(short *)(q + 0x12) = 0;
            *(short *)(q + 0x16) = 0;
            *(short *)(q + 0x1a) = 0;
        }
        DAT_8009d674 = 0;
        DAT_1f8001fc = 0;
        DAT_8009d670[0] = DAT_8009d674;
        break;
    case 1:
        DAT_8009c968[0]++;
        FUN_8001b680();
        if (DAT_1f8001c2 != 0 && (DAT_8009d670[0] & 8) && (DAT_8009d670[0] & 0x800))
            DAT_1f8001d4->w4e = 3;
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
