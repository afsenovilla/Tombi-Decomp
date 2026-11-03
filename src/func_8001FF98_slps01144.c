// near-twin of PAL CD_initintr (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 8001FF98 100
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define LOAD_COMPLETE (*(unsigned char *)0x1F8001CE)
extern int *D_800855EC[];
extern unsigned short D_801FD8E0;
extern void func_800200C4(int);
extern void func_8001F574(void);
extern void ThreadCreate(int, void (*)(void));

void func_8001FF98(int a, int b)
{
    func_800200C4(D_800855EC[a][b]);
    LOAD_COMPLETE = 0;
    if (D_801FD8E0 == 0)
        ThreadCreate(2, func_8001F574);
}
#endif
