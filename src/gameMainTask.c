// FUNC 800198b4 340 MAIN0
// MATCHING 800198b4 340
#include "spad.h"
extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
extern int D_8009F7E4;
extern unsigned short AREA_VARIANT;
void initLighting(void *p);
void updatePadInput(void);
void gameInitState(void);
void gameStateDispatcher(void);
void gameLoadSaveState(void);
void ThreadWaitFrames(int n);
void gameMainTask(void)
{
    unsigned char *p = *(unsigned char **)0x1F8001D4;
    *(unsigned char *)0x1F8001D1 = 0;
    *(unsigned char *)0x1F8001D0 = 1;
    *(short *)(p + 0x48) = 0;
    *(short *)(p + 0x4a) = 0;
    *(short *)(p + 0x4c) = 0;
    *(short *)(p + 0x4e) = 0;
    p[0x6a] = 0;
    D_8009E375 = 0;
    D_8009E376 = 0;
    D_8009E377 = 0;
    D_8009F085 = 0;
    D_8009F086 = 0;
    D_8009F087 = 0;
    *(short *)0x1F8001DC = -1;
    *(short *)0x1F8001DE = 0;
    D_8009F7E4 = 0;
    *(unsigned char *)0x1F8001CE = 0;
    AREA_VARIANT = 0;
    initLighting(((unsigned char*)&SPAD.b118));
    *(short *)0x1F8001FC = 0;
    for (;;) {
        updatePadInput();
        switch (*(unsigned short *)(*(unsigned char **)0x1F8001D4 + 0x48)) {
        case 0: gameInitState(); break;
        case 1: gameStateDispatcher(); break;
        case 2: gameLoadSaveState(); break;
        }
        ThreadWaitFrames(1);
    }
}
