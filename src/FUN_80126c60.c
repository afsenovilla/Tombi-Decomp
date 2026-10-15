// FUNC 80126c60 68 X000
// MATCHING 80126c60 68
extern short pushObjectOutOfObjectBoxB(void);
void FUN_80126c60(int a, char *p)
{
    if (*(int *)(p + 0x94) == 0 && pushObjectOutOfObjectBoxB() != 0)
        *(short *)0x1f80019e = 0;
}
