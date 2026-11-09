// FUNC 8004e200 212 MAIN0
// MATCHING 8004e200 212
// VERSION SLPS_01144 slps01144 80018EB4 200
// VERSION BETA_980203 beta980203 800197C8 200
#if defined(VERSION_SLPS_01144)
extern unsigned short *TASK_C;
extern char LOAD_COMPLETE_SYM;
extern void SetDispMask(int);
extern void cdSeekStream(int);
extern void loadSoundSet(int);
extern void queueSystemLoadList(int, int);

void func_80018EB4(void)
{
    unsigned short *o = TASK_C;
    short s = o[0x25];
    unsigned short v = s;
    switch (v) {
    case 0:
        SetDispMask(0);
        LOAD_COMPLETE_SYM = 0;
        loadSoundSet(2);
        TASK_C[0x25] = TASK_C[0x25] + 1;
        break;
    case 1:
        if (LOAD_COMPLETE_SYM != 0) {
            o[0x25] = s + 1;
            cdSeekStream(0);
        }
        break;
    case 2:
        o[0x24] = 4;
        o[0x25] = 0;
        break;
    }
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 8004E200 (bootLoadMovieResources.c): PAL C of src/bootLoadMovieResources.c (bootLoadMovieResources, twin score 0.913), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
extern unsigned short *TASK_C;
extern char LOAD_COMPLETE_SYM;
extern void SetDispMask(int);
extern void cdSeekStream(int);
extern void loadSoundSet(int);
extern void queueSystemLoadList(int, int);

void bootLoadMovieResources(void)
{
    unsigned short *o = TASK_C;
    short s = o[0x25];
    unsigned short v = s;
    switch (v) {
    case 0:
        SetDispMask(0);
        LOAD_COMPLETE_SYM = 0;
        loadSoundSet(2);
        TASK_C[0x25] = TASK_C[0x25] + 1;
        break;
    case 1:
        if (LOAD_COMPLETE_SYM != 0) {
            o[0x25] = s + 1;
            cdSeekStream(0);
        }
        break;
    case 2:
        o[0x24] = 4;
        o[0x25] = 0;
        break;
    }
}
#else
extern unsigned short *TASK_C;
extern char LOAD_COMPLETE_SYM;
extern void SetDispMask(int);
extern void cdSeekStream(int);
extern void loadSoundSet(int);
extern void queueSystemLoadList(int, int);

void bootLoadMovieResources(void)
{
    unsigned short *o = TASK_C;
    short s = o[0x25];
    unsigned short v = s;
    switch (v) {
    case 0:
        SetDispMask(0);
        LOAD_COMPLETE_SYM = 0;
        loadSoundSet(2);
        queueSystemLoadList(2, 1);
        TASK_C[0x25] = TASK_C[0x25] + 1;
        break;
    case 1:
        if (LOAD_COMPLETE_SYM != 0) {
            o[0x25] = s + 1;
            cdSeekStream(0);
        }
        break;
    case 2:
        o[0x24] = 4;
        o[0x25] = 0;
        break;
    }
}
#endif
