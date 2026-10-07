// FUNC 801229d8 64 X000
// MATCHING 801229d8 64
extern void PlayerSetAnimIfChanged(void *o, int a);
extern void FUN_8001f96c(int a, int b, int c, int d);

void FUN_801229d8(char *o)
{
    PlayerSetAnimIfChanged(o, 0xd);
    FUN_8001f96c(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
}
