// VERSION BETA_980203 beta980203 80021C34 88
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// Beta version of PAL queueSystemLoadList (main0 8004FA80): one argument, no LDSYS reload; the body is the beta loadWssFile (80021490) with one index: func_800200C4(D_800855EC[a]).
// Beta version of PAL main0 8004F38C (loadWssFile.c): JP C of src/func_8001FF98_slps01144.c (loadWssFile) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#define LOAD_COMPLETE (*(unsigned char *)0x1F8001CE)
extern int D_800855EC[];
extern unsigned short D_801FD8E0;
extern void func_800200C4(int);
extern void func_8001F574(void);
extern void ThreadCreate(int, void (*)(void));

void func_8001FF98(int a)
{
    func_800200C4(D_800855EC[a]);
    LOAD_COMPLETE = 0;
    if (D_801FD8E0 == 0)
        ThreadCreate(2, func_8001F574);
}
#endif
