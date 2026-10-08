// FUNC 8012038c 68 X003
// MATCHING 8012038c 68
extern short FUN_800482ec(void);
void FUN_8012038c(int a, char *p)
{
    if (*(int *)(p + 0x94) == 0 && FUN_800482ec() != 0)
        *(short *)0x1f80019e = 0;
}
