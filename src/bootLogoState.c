// FUNC 8004df00 768 MAIN0
// MATCHING 8004df00 768
#include "globals.h"
typedef struct { char p0[0x48]; short w48; unsigned short w4a; unsigned short w4c; char p4e[0x58 - 0x4e]; short w58; } G;
extern G *TASK_C;
extern unsigned char LOAD_COMPLETE_SYM, MOVIE_PLAY_STATE, MOVIE_ID;
extern short FRAME_BUFFER_INDEX;
extern int NEXT_PRIM;
extern void func_8001D6A4();
extern void SetDispMask(int);
extern void loadSoundSet(int);
extern void queueSystemLoadList(int, int);
extern void func_800E8A14(void);
extern void func_800E8A2C(void);
extern void initDisplay(int, int, int);
extern void initWideDisplayEnv(int, int, int, int);
extern void ThreadCreate(int, void *);
extern void stopBgm(int);
extern void cdSeekStream(int);
void bootLogoState(void)
{
    switch (TASK_C->w4a) {
    case 0:
        switch (TASK_C->w4c) {
        case 0:
            SetDispMask(0);
            LOAD_COMPLETE_SYM = 0;
            loadSoundSet(1);
            queueSystemLoadList(1, 1);
            TASK_C->w4c++;
            break;
        case 1:
            if (LOAD_COMPLETE_SYM != 0) {
                TASK_C->w4a = 2;
                TASK_C->w4c = 0;
                func_800E8A14();
            }
            break;
        }
        break;
    case 1:
        SetDispMask(0);
        func_800E8A14();
        goto next;
    case 2:
        TASK_C->w4a++;
        initDisplay(0xff, 0xff, 0xff);
        MOVIE_PLAY_STATE = 1;
        MOVIE_ID = 0x15;
        ThreadCreate(1, func_8001D6A4);
        break;
    case 3:
        if (MOVIE_PLAY_STATE != 0) break;
        goto next;
    case 4:
        SetDispMask(0);
        initWideDisplayEnv(0xff, 0xff, 0xff, 0xc);
        SetDispMask(1);
        NEXT_PRIM = (int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
        func_800E8A2C();
        TASK_C->w58 = 0x78;
        TASK_C->w4a++;
        break;
    case 5:
        NEXT_PRIM = (int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
        func_800E8A2C();
        if (--TASK_C->w58 > 0) break;
        stopBgm(0);
        SetDispMask(0);
        initDisplay(0, 0, 0);
        LOAD_COMPLETE_SYM = 0;
        loadSoundSet(2);
        queueSystemLoadList(2, 1);
    next:
        TASK_C->w4a++;
        break;
    case 6:
        if (LOAD_COMPLETE_SYM != 0) {
            TASK_C->w4a++;
            cdSeekStream(0);
        }
        break;
    case 7:
        TASK_C->w48 = 3;
        TASK_C->w4a = 0;
        break;
    }
}
