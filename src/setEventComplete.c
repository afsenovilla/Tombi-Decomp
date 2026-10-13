// FUNC 8005a9a4 188 MAIN0
// MATCHING 8005a9a4 188
#include "globals.h"
extern unsigned char SCRIPT_FLAGS[];
extern void FUN_80026a10(int a);
extern void FUN_8005aa74(int a, int b, int c, int d);
extern void printEventMessage(int a, int b);
extern void playJingle(int a);
extern void FUN_8001f620(void);

int setEventComplete(int i, int arg)
{
    int t;
    if (SCRIPT_FLAGS[i] != 0xff) {
        t = EVENT_COMPLETE_AP_TABLE[i];
        SCRIPT_FLAGS[i] = 0xff;
        FUN_80026a10(((int *)(EVENT_COMPLETE_AP_TABLE - 0xe8))[t]); /* tabla en 0x80080504: base simbolo+offset para que no se adelante al sb */
        if (i != 10) {
            FUN_8005aa74(i, 1, 1, arg);
            printEventMessage(i, 1);
            playJingle(2);
            FUN_8001f620();
        }
    }
    return SCRIPT_FLAGS[i];
}
