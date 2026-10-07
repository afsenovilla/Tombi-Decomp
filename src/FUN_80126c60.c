// FUNC 80126c60 68 X000
extern short FUN_800482ec(void);
void FUN_80126c60(int a, char *p)
{
    if (*(int *)(p + 0x94) == 0 && FUN_800482ec() != 0)
        *(short *)0x1f80019e = 0;
}
