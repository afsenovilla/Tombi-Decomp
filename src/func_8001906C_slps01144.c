// JP-only (slps01144), written from the disassembly
// VERSION SLPS_01144 slps01144 8001906C 984
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
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
extern void SetDispMask(int);
extern void func_80016244(int, int, int);
extern void func_800EA090(void);
extern void func_8001EF50(int);
extern void drawDebugText(int, int, int, void *);
extern void func_800EA0C8(int *);
extern void playSFX(int);
extern void stopBgm(int);
extern void func_8001E910(int, int);
extern void setTask(void *);
extern void func_80019608_slps01144();
extern void func_8001DAF0(void);
extern volatile unsigned short D_8009F200[];
extern char D_800100A0[], D_800100BC[];
extern char *D_80071434[];
extern unsigned char D_80071440[], D_80071441[];
extern void setTask(void *);
void func_8001906C(int *p)
{
    int j, i;
    char **q;
    switch (TASK_C->w4a) {
    case 0:
        SetDispMask(0);
        TASK_C->w4a++;
    case 1:
        if (MOVIE_PLAY_STATE == 0) {
            func_80016244(0, 0, 0);
            TASK_C->w4a++;
        }
        break;
    case 2:
        func_800EA090();
        SetDispMask(1);
        func_8001EF50(0);
        drawDebugText(0x38, 0xc0, 0, D_800100A0);
        TASK_C->b69 = 0;
        TASK_C->w58 = 0x3cc;
        TASK_C->w4a++;
        break;
    case 3:
        NEXT_PRIM = (int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
        if (--TASK_C->w58 <= 0) {
            TASK_C->w4a++;
        }
        func_800EA0C8(p);
        if (D_8009F200[0] != 0) {
            TASK_C->w58 = 0x3cc;
        }
        if (JOYPAD_STATE & 0x1000) {
            if (TASK_C->b69 != 0) {
                TASK_C->b69--;
                playSFX(8);
            }
        }
        if (JOYPAD_STATE & 0x4000) {
            if (TASK_C->b69 < 2) {
                TASK_C->b69++;
                playSFX(8);
            }
        }
        if (JOYPAD_STATE & 0x800) {
            stopBgm(0);
            switch (TASK_C->b69) {
            case 0:
                TASK_C->b68 = 0;
                goto common;
            case 1:
                TASK_C->b68 = 1;
            common:
                func_8001E910(10, 10);
                setTask(func_80019608_slps01144);
                break;
            case 2:
                TASK_C->w48 = 2;
                TASK_C->w4a = 0;
                func_8001E910(10, 10);
                break;
            }
        }
        drawDebugText(0x38, 0xc0, 0, D_800100A0);
        i = 0;
        q = D_80071434;
        for (j = 0; i < 3; i++) {
            drawDebugText(D_80071440[j] * 8, D_80071441[j] * 8, i == TASK_C->b69 ? 3 : 0, *q++);
            j += 2;
        }
        if (*(unsigned short *)0x1F8001F8 & 8) {
            drawDebugText(0x50, 0x90, 0, D_800100BC);
        }
        func_8001DAF0();
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
