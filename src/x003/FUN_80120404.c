// FUNC 80120404 52 X003
// MATCHING 80120404 52
extern short FUN_800482ec();

void FUN_80120404(void)
{
    if (FUN_800482ec() != 0)
        *(short *)0x1f80019e = 0;
}
