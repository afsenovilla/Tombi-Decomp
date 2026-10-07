// FUNC 8006f0e0 52 MAIN0
// MATCHING 8006f0e0 52
extern short *DAT_800982cc;
int FUN_8006f0e0(int a)
{
    int i = (unsigned short)a;
    if (i > 2)
        return 0;
    DAT_800982cc[i * 8] = 0;
    return 1;
}
