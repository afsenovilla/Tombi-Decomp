// FUNC 8006a254 12 MAIN0
// MATCHING 8006a254 12
void _padSetAct(char *p, int a, char b)
{
    *(int *)(p + 0x28) = a;
    p[0x34] = b;
}
