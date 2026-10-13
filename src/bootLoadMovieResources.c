// FUNC 8004e200 212 MAIN0
// MATCHING 8004e200 212
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
