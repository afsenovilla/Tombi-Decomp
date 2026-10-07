// FUNC 80126d04 68 X000
// MATCHING 80126d04 68
extern short FUN_800482ec();

void FUN_80126d04(int a, char *p)
{
    if (*(int *)(p + 0x94) == 0 && FUN_800482ec() != 0)
        *(short *)0x1f80019e = 0;
}
