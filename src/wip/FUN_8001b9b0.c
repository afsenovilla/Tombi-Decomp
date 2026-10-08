// FUNC 8001b9b0 324 MAIN0
extern char DAT_800b3e28[];
extern short DAT_1f8001f4;
extern short DAT_1f8001c6;
extern unsigned short DAT_1f8001f8;
extern unsigned int DAT_1f800164;
extern void FUN_8001ca58(void);
extern void FUN_800310d0(void);
extern void FUN_8001d36c(void);
extern void FUN_8004c0dc(void);
extern void FUN_800edeec(void);
extern void FUN_80027124(void);
extern void FUN_8002b020(void);
extern void FUN_800ed4d0(void);
extern void FUN_80030f38(void);
extern void FUN_80017678(void);
extern void FUN_8005042c(void);
extern void SoundUpdate(void);

void FUN_8001b9b0(void)
{
    DAT_1f800164 = (unsigned int)(DAT_800b3e28 + DAT_1f8001f4 * 0xc000) & 0xffffff;
    FUN_8001ca58();
    if (DAT_1f8001c6 == 0) {
        DAT_1f8001f8 = DAT_1f8001f8 + 1;
        FUN_800310d0();
        if (DAT_1f8001c6 == 0) {
            FUN_8001d36c();
            FUN_8004c0dc();
            FUN_800edeec();
        }
    }
    if (DAT_1f8001c6 != 1)
        FUN_80027124();
    if (DAT_1f8001c6 == 0) {
        FUN_8002b020();
        if (DAT_1f8001c6 == 0) {
            FUN_800ed4d0();
            FUN_80030f38();
        }
    }
    if (DAT_1f8001c6 != 2)
        FUN_8005042c();
    else
        FUN_80017678();
    SoundUpdate();
}
