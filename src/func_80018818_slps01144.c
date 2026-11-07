// VERSION SLPS_01144 slps01144 80018818 596
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "globals.h"
typedef struct {
    char pad[0x48];
    unsigned short w48, w4a, w4c;
    char pad2[0x58 - 0x4e];
    short w58, w5a;
} T80019244;

extern T80019244 *TASK_C;
#define P TASK_C
extern unsigned char D_1F8001D1, LOAD_COMPLETE_SYM, D_1F8001C4, D_1F8001C5;
extern short FRAME_BUFFER_INDEX;

extern unsigned char D_8009FF05, D_8009FF06, D_8009FF07;
extern unsigned char D_800A0C15, D_800A0C16, D_800A0C17;
extern short BUTTON_CONFIG_INDEX;
extern unsigned char D_8009F20C;
extern unsigned char D_800A0C78;
extern char D_80010000[];
extern char D_80010008[];
extern void func_80018A6C_slps01144(void);
void loadButtonConfig(void);
void SetDispMask(int);
void func_8002073C(int);
void func_8001D584(int);
void *memset(void *, int, int);
void drawDebugText(int, int, int, char *);
void setTask(void (*)(void));
void ThreadWaitFrames(int n);

void func_80018818_slps01144(void)
{
    T80019244 *p;

    D_8009FF05 = 0;
    D_8009FF06 = 0;
    D_8009FF07 = 0;
    D_800A0C15 = 0;
    D_800A0C16 = 0;
    D_800A0C17 = 0;
    D_1F8001D1 = 0;
    SPAD.b1d0 = 0;
    BUTTON_CONFIG_INDEX = 0;
    D_8009F20C = 1;
    D_800A0C78 = 0;
    loadButtonConfig();
    P->w48 = 0;
    P->w4a = 0;
    P->w4c = 0;
    SetDispMask(0);
    for (;;) {
        switch (P->w48) {
        case 0:
            LOAD_COMPLETE_SYM = 0;
            func_8002073C(0);
            D_1F8001C4 = 0;
            D_1F8001C5 = 0;
            memset(((char*)&SPAD.b1a0), 0, 0x24);
            P->w48++;
            break;
        case 1:
            if (LOAD_COMPLETE_SYM) {
                func_8001D584(0x15);
                P->w48 = 4;
            }
            break;
        case 2:
            SetDispMask(1);
            P->w58 = 0x78;
            P->w48++;
            break;
        case 3:
            (*(int*)&SPAD.p164) = (int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
            p = P;
            if (--p->w58 == -1)
                p->w48++;
            drawDebugText(0x50, 0x60, 0, D_80010000);
            drawDebugText(0x50, 0x70, 0, D_80010008);
            break;
        case 4:
            P->w48 = 0;
            P->w4a = 1;
            P->w4c = 0;
            setTask(func_80018A6C_slps01144);
            break;
        }
        ThreadWaitFrames(1);
    }
}
#endif
