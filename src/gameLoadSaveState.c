// FUNC 80019b0c 480 MAIN0
// MATCHING 80019b0c 480
#include "globals.h"
typedef struct { char pad[0x48]; unsigned short w48, w4a, w4c, w4e; } S1d4;
extern S1d4 *TASK_C;
extern unsigned char LOAD_COMPLETE_SYM;
extern unsigned char DAT_1f8001d0;
extern short FRAME_BUFFER_INDEX;
extern int NEXT_PRIM;
extern short DAT_800a45ea;
extern unsigned char DAT_8009c974;
extern unsigned char GAME_KEEP_BGM;
extern void FUN_8004dd8c(void);
extern void FUN_8005f060(int);
extern void stopPadVibration(void);
extern void loadSoundSet(int);
extern void queueSystemLoadList(int, int);
extern void clearMenuParams(void);
extern void startBgmTrack(int);
extern int FUN_800e9888(void);
extern void FUN_800ea37c(void);
extern void FUN_8001dbdc(void);
extern void FUN_8001f110(int);
extern void setTask(void (*)(void));

void gameLoadSaveState(void)
{
    int r;
    switch (TASK_C->w4a) {
    case 0:
        FUN_8005f060(0);
        stopPadVibration();
        LOAD_COMPLETE_SYM = 0;
        loadSoundSet(3);
        queueSystemLoadList(5, 1);
        TASK_C->w4a++;
        break;
    case 1:
        if (LOAD_COMPLETE_SYM == 0)
            break;
        clearMenuParams();
        DAT_800a45ea = 5;
        startBgmTrack(0);
        FUN_8005f060(1);
        TASK_C->w4a++;
        break;
    case 2:
        NEXT_PRIM = (int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
        r = FUN_800e9888();
        if (r == 1)
            TASK_C->w4a++;
        else if (r == -1)
            TASK_C->w4a = 4;
        FUN_800ea37c();
        FUN_8001dbdc();
        break;
    case 3:
        DAT_8009c974 = 0;
        GAME_KEEP_BGM = 0;
        FUN_8001f110(0);
        TASK_C->w48 = 1;
        TASK_C->w4a = 1;
        TASK_C->w4c = 0;
        TASK_C->w4e = 0;
        break;
    case 4:
        FUN_8001f110(0);
        DAT_1f8001d0 = 0;
        TASK_C->w48 = 1;
        TASK_C->w4a = 0;
        TASK_C->w4c = 0;
        TASK_C->w4e = 0;
        setTask(FUN_8004dd8c);
        break;
    }
}
