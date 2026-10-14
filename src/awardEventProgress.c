// FUNC 8005a7a0 264 MAIN0
// MATCHING 8005a7a0 264
#include "globals.h"
extern unsigned char SCRIPT_FLAGS[];
extern void FUN_80026a10(int);
extern void FUN_8005aa74(int, int, int, int);
extern void printEventMessage(int, int);
extern void FUN_8001e4f0(int);
extern void spawnItemPickupObject(int);
extern void playJingle(int);
extern void FUN_8001f620(void);

unsigned char awardEventProgress(int a, int b, int c)
{
    if (b == 0) {
        FUN_80026a10(AP_TABLE[EVENT_STARTED_AP_TABLE[a]]);
        if (a != 10) {
            FUN_8005aa74(a, 0, 0x3c, c);
            printEventMessage(a, 0);
            FUN_8001e4f0(0x2a);
            spawnItemPickupObject(0);
        }
    } else {
        FUN_80026a10(AP_TABLE[EVENT_COMPLETE_AP_TABLE[a]]);
        if (a != 10) {
            FUN_8005aa74(a, 1, 1, c);
            printEventMessage(a, 1);
            playJingle(2);
            FUN_8001f620();
        }
    }
    return SCRIPT_FLAGS[a];
}
