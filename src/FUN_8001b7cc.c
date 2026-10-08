// FUNC 8001b7cc 484 MAIN0
// MATCHING 8001b7cc 484
typedef struct Th { char pad[0x4c]; short b; unsigned short c; } Th;
extern Th *DAT_1f8001d4;
extern char DAT_1f8001c2, DAT_1f8001cf;
extern unsigned short DAT_1f8001fc;
extern int DAT_8009c968[];
extern unsigned char DAT_8009c967;
extern volatile unsigned short DAT_8009d670[], DAT_8009d674;
extern char DAT_800b1410;
extern char DAT_8009e375, DAT_8009e376, DAT_8009e377, DAT_8009f085, DAT_8009f086, DAT_8009f087;
extern void InitSubsystems(void);
extern void FUN_80017b44(void);
extern void FUN_80021858(void);
extern void FUN_80021b20(void);
extern void FUN_8004bfe4(void);
extern void FUN_800263bc(void);
extern void FUN_8001eb64(void);
extern void FUN_8001b9b0(void);
extern unsigned char *FUN_80018678(void);

void FUN_8001b7cc(void)
{
    Th *t = DAT_1f8001d4;
    Th *u;
    unsigned char *e;
    char pad[4];
    switch (t->c) {
    case 0:
        InitSubsystems();
        FUN_80017b44();
        FUN_80021858();
        FUN_80021b20();
        FUN_8004bfe4();
        FUN_800263bc();
        DAT_800b1410 = 0;
        DAT_1f8001cf = 1;
        if (DAT_8009c967 != 1) FUN_8001eb64();
        u = DAT_1f8001d4;
        u->c = u->c + 1;
        e = FUN_80018678();
        if (e) {
            e[0] = 1;
            e[2] = 10;
            e[3] = 0;
            *(short *)(e + 0x12) = 0;
            *(short *)(e + 0x16) = 0;
            *(short *)(e + 0x1a) = 0;
        }
        DAT_8009d674 = 0;
        DAT_1f8001fc = 0;
        DAT_8009d670[0] = DAT_8009d674;
        break;
    case 1:
        DAT_8009c968[0] = DAT_8009c968[0] + 1;
        FUN_8001b9b0();
        if (DAT_1f8001c2 != 0 && (DAT_8009d670[0] & 8) != 0 && (DAT_8009d670[0] & 0x800) != 0)
            DAT_1f8001d4->c = 3;
        break;
    case 2:
    case 3:
        DAT_8009e375 = 0;
        DAT_8009e376 = 0;
        DAT_8009e377 = 0;
        DAT_8009f085 = 0;
        DAT_8009f086 = 0;
        DAT_8009f087 = 0;
        DAT_1f8001d4->b = 8;
        DAT_1f8001d4->c = 0;
        break;
    }
}
