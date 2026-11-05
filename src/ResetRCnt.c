// FUNC 8006f0e0 52 MAIN0
// MATCHING 8006f0e0 52
// VERSION SLPS_01144 slps01144 800586CC 56
// VERSION SLPM_80165 slpm80165 800512D8 56
// VERSION PCPX_96084 tomba 800876CC 56
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
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8006F0E0 (ResetRCnt.c): JP C of src/ResetRCnt.c (ResetRCnt) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern volatile short *DAT_800982cc;
int func_800586CC(int a)
{
    int i = (unsigned short)a;
    if (i > 2)
        return 0;
    DAT_800982cc[i * 8] = 0;
    return 1;
}
#elif defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 8006F0E0 (ResetRCnt.c): JP C of src/ResetRCnt.c (ResetRCnt) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
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
