// FUNC 801043f4 96 X013
// MATCHING 801043f4 96
extern void FUN_8001e4f0(int);
extern void PlayerSetAnimIfChanged(void *o, int a);
extern void FUN_8001f96c(int a, int b, int c, int d);

void FUN_801043f4(char *o)
{
    *(short *)(o + 0x20) = 0;
    FUN_8001e4f0(9);
    PlayerSetAnimIfChanged(o, 0xd);
    FUN_8001f96c(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
    o[6]++;
}
