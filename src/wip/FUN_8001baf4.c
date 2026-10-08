// FUNC 8001baf4 484 MAIN0
extern void InitSubsystems(void), FUN_80017b44(void), FUN_80021858(void), FUN_80021b20(void), FUN_8004bfe4(void), FUN_800263bc(void), FUN_8001eb64(void), FUN_8001bcd8(void);
extern unsigned char *FUN_80018678(void);

#define DAT_1f8001d4 (*(int *)0x1f8001d4)
#define DAT_1f8001c2 (*(unsigned char *)0x1f8001c2)
#define DAT_1f8001cf (*(unsigned char *)0x1f8001cf)
#define DAT_8009c967 (*(unsigned char *)0x8009c967)
#define DAT_8009e375 (*(unsigned char *)0x8009e375)
#define DAT_8009e376 (*(unsigned char *)0x8009e376)
#define DAT_8009e377 (*(unsigned char *)0x8009e377)
#define DAT_8009f085 (*(unsigned char *)0x8009f085)
#define DAT_8009f086 (*(unsigned char *)0x8009f086)
#define DAT_8009f087 (*(unsigned char *)0x8009f087)
#define DAT_800b1410 (*(unsigned char *)0x800b1410)
#define DAT_8009d674 (*(unsigned char *)0x8009d674)
#define DAT_1f8001fc (*(unsigned char *)0x1f8001fc)
#define DAT_8009c968 (*(int *)0x8009c968)
#define DAT_8009d670 (*(int *)0x8009d670)

void FUN_8001baf4(void)
{
    unsigned short u;
    int i;
    unsigned char *p;
    i = DAT_1f8001d4;
    u = *(unsigned short *)(DAT_1f8001d4 + 0x4e);
    if (u == 1) {
        DAT_8009c968 = DAT_8009c968 + 1;
        FUN_8001bcd8();
        if (DAT_1f8001c2 != 0 && (DAT_8009d670 & 8) != 0 && (DAT_8009d670 & 0x800) != 0)
            *(short *)(DAT_1f8001d4 + 0x4e) = 3;
    } else if (u < 2) {
        if (u == 0) {
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
            *(short *)(DAT_1f8001d4 + 0x4e) = *(short *)(DAT_1f8001d4 + 0x4e) + 1;
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
            DAT_8009d670 = 0;
        }
    } else if (u < 4) {
        DAT_8009e375 = 0;
        DAT_8009e376 = 0;
        DAT_8009e377 = 0;
        DAT_8009f085 = 0;
        DAT_8009f086 = 0;
        DAT_8009f087 = 0;
        *(short *)(DAT_1f8001d4 + 0x4c) = 8;
        *(short *)(i + 0x4e) = 0;
    }
}
