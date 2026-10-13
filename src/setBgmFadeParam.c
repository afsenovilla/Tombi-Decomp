// FUNC 8001e2cc 156 MAIN0
// MATCHING 8001e2cc 156

#include "globals.h"
int setBgmFadeParam(unsigned short a)
{
    switch (a & 0xf00) {
    case 0:
        BGM_FADE_INTERVAL = a & 0xff;
        BGM_FADE_TIMER = 1;
        break;
    case 0x100:
        BGM_FADE_STEP = a & 0xff;
        if (BGM_FADE_STEP >= 0x80)
            BGM_FADE_STEP = BGM_FADE_STEP | 0xff00;
        break;
    case 0x200:
        BGM_FADE_TARGET = a & 0xff;
        break;
    }
    return 0;
}
