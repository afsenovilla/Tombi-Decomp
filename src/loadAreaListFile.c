// FUNC 8004f3ec 164 MAIN0
// MATCHING 8004f3ec 164
#include "globals.h"
extern unsigned char LOAD_COMPLETE_SYM;
extern unsigned short DAT_801fd8e0;
extern void queueLoadList(int);
extern void ThreadCreate(int, void *);
extern void ThreadWaitFrames(int);
extern char LAB_8004eb08[];

void loadAreaListFile(int a)
{
    if (LOADED_LIST_FILE_SYM != a + 1) {
        LOADED_LIST_FILE_SYM = a + 1;
        queueLoadList(LDAR_LOAD_LISTS[a]);
        LOAD_COMPLETE_SYM = 0;
        ThreadCreate(2, LAB_8004eb08);
        while (LOAD_COMPLETE_SYM == 0) {
            if (DAT_801fd8e0 == 0)
                break;
            ThreadWaitFrames(1);
        }
    }
}
