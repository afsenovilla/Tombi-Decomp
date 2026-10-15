// FUNC 80124fa4 48 X000
// MATCHING 80124fa4 48
extern void pushObjectOutOfObjectBoxE();

void FUN_80124fa4(int a, char *p)
{
    if (*(int *)(p + 0x94) == 0)
        pushObjectOutOfObjectBoxE();
}
