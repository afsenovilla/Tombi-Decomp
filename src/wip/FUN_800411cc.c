// FUNC 800411cc 116 MAIN0
extern int FUN_8003f200(int, int);
extern short FUN_80040f78(int, int, int);
extern int DAT_1f800278;
int FUN_800411cc(int o, short a, int b)
{
    DAT_1f800278 = FUN_8003f200(a, *(short *)(*(int *)(o + 0x44) + 2));
    return (short)FUN_80040f78(o, a, (short)b);
}
