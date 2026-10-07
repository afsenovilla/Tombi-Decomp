// FUNC 8006a564 56 MAIN0
// MATCHING 8006a564 56
int FUN_8006a564(unsigned char *p)
{
    int a = ((p[0xe3] + 1) >> 1) * 4;
    int b = ((p[0xe9] * 5 + 3) & 0xffc) + 4;
    return a + b + *(unsigned short *)(p + 0xec);
}
