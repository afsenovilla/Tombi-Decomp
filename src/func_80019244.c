// FUNC 80019244 1144 MAIN0
// MATCHING 80019244 1144
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
extern short D_1F8001F4, D_1F8001F6;

extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
extern short D_8009F3DC;
extern unsigned char D_8009D67C;
extern unsigned char D_8009F0E8;
extern char D_80010000[];
extern char D_80010008[];
extern void titleSequenceTask(void);
void loadCollisionBounds(void);
void SetDispMask(int);
void loadSoundSet(int);
void queueSystemLoadList(int, int);
void *memset(void *, int, int);
void cdSeekStream(int);
void drawDebugText(int, int, int, char *);
void setTask(void (*)(void));
void initWideDisplayEnv(int, int, int, int);
void FUN_80025b98(void);
void initDisplay(int, int, int);
void drawBootLogo(int, int);
void ThreadWaitFrames(int n);

void func_80019244(void)
{
    T80019244 *p;

    D_8009E375 = 0;
    D_8009E376 = 0;
    D_8009E377 = 0;
    D_8009F085 = 0;
    D_8009F086 = 0;
    D_8009F087 = 0;
    D_1F8001D1 = 0;
    SPAD.b1d0 = 0;
    D_8009F3DC = 0;
    D_8009D67C = 1;
    D_8009F0E8 = 0;
    loadCollisionBounds();
    P->w48 = 9;
    P->w4a = 0;
    P->w4c = 0;
    SetDispMask(0);
    for (;;) {
        switch (P->w48) {
        case 0:
            LOAD_COMPLETE_SYM = 0;
            loadSoundSet(0);
            loadSoundSet(1);
            queueSystemLoadList(0, 1);
            D_1F8001C4 = 0;
            D_1F8001C5 = 0;
            memset(((char*)&SPAD.b1a0), 0, 0x24);
            P->w48++;
            break;
        case 1:
            if (LOAD_COMPLETE_SYM) {
                cdSeekStream(0x15);
                P->w48 = 4;
            }
            break;
        case 2:
            SetDispMask(1);
            P->w58 = 0x78;
            P->w48++;
            break;
        case 3:
            (*(int*)&SPAD.p164) = (int)(LOAD_BUFFER_ALT + D_1F8001F4 * 0xc000) & 0xffffff;
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
            setTask(titleSequenceTask);
            break;
        case 9:
            SetDispMask(0);
            LOAD_COMPLETE_SYM = 0;
            queueSystemLoadList(0x5e, 1);
            P->w48 = 10;
            break;
        case 10:
            if (LOAD_COMPLETE_SYM) {
                SetDispMask(0);
                initWideDisplayEnv(0, 0, 0, 0);
                SetDispMask(1);
                P->w58 = 0xf0;
                P->w48 = 0xb;
                P->w4a = 0;
            }
            break;
        case 11:
            (*(int*)&SPAD.p164) = (int)(LOAD_BUFFER_ALT + D_1F8001F4 * 0xc000) & 0xffffff;
            p = P;
            switch (p->w4a) {
            case 0:
                p->w5a = 1;
                p->w4a++;
                break;
            case 1:
                if (++p->w5a >= 0x80) {
                    p->w58 = 0xb4;
                    p->w4a++;
                }
                break;
            case 2:
                if (--p->w58 == -1) {
                    p->w4a++;
                    FUN_80025b98();
                }
                break;
            case 3:
                if (--p->w5a == 0) {
                    SetDispMask(0);
                    initDisplay(0, 0, 0);
                    D_1F8001D1 = 0;
                    D_1F8001F6 = 0;
                    P->w48 = 0;
                    P->w4a = 0;
                }
                break;
            }
            if (P->w48 == 0xb)
                drawBootLogo(*(unsigned char *)&P->w5a, 1);
            break;
        }
        ThreadWaitFrames(1);
    }
}
