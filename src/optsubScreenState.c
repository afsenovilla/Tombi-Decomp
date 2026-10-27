// FUNC 8004e914 500 MAIN0
// MATCHING 8004e914 500
#include "globals.h"
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; short w4e; } S;
extern S *TASK_C;
extern short FRAME_BUFFER_INDEX;
extern int NEXT_PRIM;
extern unsigned char LOAD_COMPLETE_SYM;
extern void FUN_8001f110(int);
extern void FUN_8005f060(int);
extern void loadSoundSet(int);
extern void queueSystemLoadList(int, int);
extern void FUN_800e827c(void);
extern void startBgmTrack(int);
extern int FUN_800e8284(void);
extern void FUN_800e8618(void);
extern void FUN_800e8700(void);

void optsubScreenState(void)
{
    S *g;
    NEXT_PRIM = (FRAME_BUFFER_INDEX * 0xc000 + (int)LOAD_BUFFER_ALT) & 0xffffff;
    switch (TASK_C->w4a) {
    case 0:
        FUN_8001f110(0);
        FUN_8005f060(0);
        LOAD_COMPLETE_SYM = 0;
        loadSoundSet(3);
        queueSystemLoadList(8, 1);
        TASK_C->w4a++;
        break;
    case 1:
        if (LOAD_COMPLETE_SYM != 0) {
            FUN_8005f060(1);
            FUN_800e827c();
            startBgmTrack(0);
            g = TASK_C;
            g->w4c = 0;
            g->w4a++;
        }
        break;
    case 2:
        switch (TASK_C->w4c) {
        case 0:
            TASK_C->w4e = 0;
            TASK_C->w4c++;
        case 1:
            if (FUN_800e8284() != 0)
                TASK_C->w4a++;
            break;
        case 2:
            FUN_800e8618();
            break;
        case 3:
            FUN_800e8700();
            break;
        }
        break;
    case 3:
        FUN_8001f110(0);
        FUN_8005f060(0);
        LOAD_COMPLETE_SYM = 0;
        loadSoundSet(2);
        queueSystemLoadList(2, 1);
        TASK_C->w4a++;
        break;
    case 4:
        if (LOAD_COMPLETE_SYM != 0) {
            g = TASK_C;
            g->w48 = 4;
            g->w4a = 0;
        }
        break;
    }
}
