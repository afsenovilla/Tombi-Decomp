// VERSION BETA_980203 beta980203 80019980 1216
#if defined(VERSION_BETA_980203)
// Beta version: the USA title loop (func_80019E68_scus94236 of src/loopTitleScreen.c) with the menu and blinking text drawn at x 0x50 / 0x30 (USA 0x30 / 0x38).
// USA version of PAL main0 8004E41C (loopTitleScreen.c): PAL C of src/loopTitleScreen.c (0.971 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "gv6.h"
#include "globals.h"
typedef struct {
    char p0[0x48]; short w48; unsigned short w4a; unsigned short w4c; char p4e[0x58 - 0x4e]; short w58;
    char p5a[0x68 - 0x5a]; unsigned char b68, b69, b6a, b6b;
} G;
extern G *TASK_C;
extern unsigned char MOVIE_PLAY_STATE;
extern short FRAME_BUFFER_INDEX;
extern unsigned short JOYPAD_STATE, TICK_COUNT;
extern int NEXT_PRIM;
extern unsigned short D_80076E80[];
extern void gameMainTask();
extern void SetDispMask(int);
extern void initDisplay(int, int, int);
extern void func_800E7DA4(void);
extern void startBgmTrack(int);
extern void func_800E889C(int, int, int);
extern void func_800E7DDC(int *);
extern void playSFX(int);
extern void stopBgm(int);
extern void playSFXAndSetNote(int, int);
extern void setTask(void *);
extern void func_800E8E78(int, int);
extern void updateSound(void);
void loopTitleScreen(int *p)
{
    int d;
    switch (TASK_C->w4a) {
    case 0:
        SetDispMask(0);
        TASK_C->w4c = 0;
        TASK_C->w4a++;
    case 1:
        if (MOVIE_PLAY_STATE == 0) {
            initDisplay(0, 0, 0);
            TASK_C->w4a++;
        }
        break;
    case 2:
        func_800E7DA4();
        SetDispMask(1);
        startBgmTrack(0);
        func_800E889C(0x50, 0xc0, 1);
        TASK_C->b69 = 0;
        TASK_C->b6a = D_80076E80[TASK_C->b69];
        TASK_C->b6b = D_80076E80[TASK_C->b69];
        TASK_C->w58 = 0x3cc;
        TASK_C->w4a++;
        break;
    case 3:
        NEXT_PRIM = (int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
        if (--TASK_C->w58 <= 0) {
            TASK_C->w4a++;
        }
        func_800E7DDC(p);
        if (*(volatile unsigned short *)&GV6.s70 != 0) {
            TASK_C->w58 = 0x3cc;
        }
        switch (TASK_C->w4c) {
        case 0:
            if ((JOYPAD_STATE & 0x80) && TASK_C->b69 != 0) {
                TASK_C->b69--;
                TASK_C->b6b = D_80076E80[TASK_C->b69];
                TASK_C->w4c++;
                playSFX(8);
            }
            if ((JOYPAD_STATE & 0x20) && TASK_C->b69 < 2) {
                TASK_C->b69++;
                TASK_C->b6b = D_80076E80[TASK_C->b69];
                TASK_C->w4c++;
                playSFX(8);
            }
            if (JOYPAD_STATE & 0x4008) {
                stopBgm(0);
                switch (TASK_C->b69) {
                case 0:
                    TASK_C->b68 = 0;
                    playSFXAndSetNote(10, 10);
                    setTask(gameMainTask);
                    break;
                case 1:
                    TASK_C->b68 = 1;
                    playSFXAndSetNote(10, 10);
                    setTask(gameMainTask);
                    break;
                case 2:
                    TASK_C->w48 = 2;
                    TASK_C->w4a = 0;
                    playSFXAndSetNote(10, 10);
                    break;
                }
            }
            break;
        case 1:
            if (TASK_C->b6a != TASK_C->b6b) {
                d = -4;
                if (TASK_C->b6a < TASK_C->b6b) d = 4;
                TASK_C->b6a += d;
                if (TASK_C->b6b == TASK_C->b6a) {
                    TASK_C->w4c--;
                }
            }
            break;
        }
        func_800E889C(0x50, 0xc0, 1);
        if (TICK_COUNT & 0x20) {
            func_800E889C(0x30, 0x90, 0);
        }
        func_800E8E78(TASK_C->b6a, TASK_C->b69);
        updateSound();
        break;
    case 4:
        stopBgm(0);
        *p = 0;
        TASK_C->w48 = 3;
        TASK_C->w4a = 0;
        break;
    }
}
#endif
