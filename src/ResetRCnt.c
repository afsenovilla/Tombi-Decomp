// FUNC 8006f0e0 52 MAIN0
// MATCHING 8006f0e0 52
// VERSION SLPS_01144 slps01144 800586CC 56
#if defined(VERSION_SLPS_01144)
extern volatile short *DAT_800982cc;
int func_800586CC(int a)
{
    int i = (unsigned short)a;
    if (i > 2)
        return 0;
    DAT_800982cc[i * 8] = 0;
    return 1;
}
#else
extern short *DAT_800982cc;
int ResetRCnt(int a)
{
    int i = (unsigned short)a;
    if (i > 2)
        return 0;
    DAT_800982cc[i * 8] = 0;
    return 1;
}
#endif
