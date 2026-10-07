// FUNC 8004065c 140 MAIN0
extern int FUN_8003f200(int, int);
extern short FUN_800402ec(int, int, int, int);
extern int DAT_1f800278;

int FUN_8004065c(int o, short a, short b, unsigned c)
{
    DAT_1f800278 = FUN_8003f200(a, *(short *)(*(int *)(o + 0x44) + 2));
    return FUN_800402ec(o, a, b, c & 1 | 0xffffff00);
}
