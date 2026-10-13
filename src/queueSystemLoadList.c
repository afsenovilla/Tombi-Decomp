// FUNC 8004fa80 232 MAIN0
// MATCHING 8004fa80 232
extern unsigned char LOADED_LIST_FILE_SYM;
extern unsigned char LOAD_COMPLETE_SYM;
extern int LDSYS_LOAD_LIST;
extern int SYSTEM_LOAD_LISTS[];
extern unsigned short DAT_801fd8e0;
extern void queueLoadList(int);
extern void FUN_8004eb08(void);
extern void ThreadCreate(int, void (*)(void));
extern void ThreadWaitFrames(int);
void queueSystemLoadList(int a, int b)
{
    if (LOADED_LIST_FILE_SYM != 0) {
        LOADED_LIST_FILE_SYM = 0;
        queueLoadList(LDSYS_LOAD_LIST);
        LOAD_COMPLETE_SYM = 0;
        ThreadCreate(2, FUN_8004eb08);
        while (LOAD_COMPLETE_SYM == 0 && DAT_801fd8e0 != 0)
            ThreadWaitFrames(1);
    }
    queueLoadList(SYSTEM_LOAD_LISTS[a]);
    if (b != 0) {
        LOAD_COMPLETE_SYM = 0;
        ThreadCreate(2, FUN_8004eb08);
    }
}
