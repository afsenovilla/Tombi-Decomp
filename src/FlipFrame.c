// FUNC 80016984 192 MAIN0
// MATCHING 80016984 192
#include "globals.h"
extern unsigned short FRAME_BUFFER_INDEX;
extern char *CURRENT_OT, *prev;
extern void PutDispEnv(void *), PutDrawEnv(void *), f(void *), DrawOTag(void *), ClearOTagR(void *, int);
void FlipFrame(void)
{
    int n = 1 - FRAME_BUFFER_INDEX;
    int idx = (short)n * 0xd10;
    FRAME_BUFFER_INDEX = n;
    prev = CURRENT_OT;
    CURRENT_OT = OT_FRAMEBUFFER + idx;
    PutDispEnv(CURRENT_OT + 0xca0);
    PutDrawEnv(CURRENT_OT + 0xcb4);
    f(prev + 0xc9c);
    DrawOTag(prev + 0xc9c);
    ClearOTagR(CURRENT_OT, 0x328);
}
