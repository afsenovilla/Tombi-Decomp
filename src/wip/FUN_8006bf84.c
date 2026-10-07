// FUNC 8006bf84 32 MAIN0
void FUN_8006bf84(int a)
{
    int t = *(volatile unsigned short *)0x1f801120;
    *(int *)0x800a0a18 = a;
    *(int *)0x8009c33c = t;
}
